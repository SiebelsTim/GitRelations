#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "treenode.h"
#include "leafnode.h"
#include "graphicsviewzoom.h"
#include "contributer.h"
#include "macros.h"
#include "forcelayout.h"
#include "tooltip.h"
#include "zoomgraphicsscene.h"

#include "CommitIterator.h"
#include "Commit.h"
#include "CommitX.h"
#include "FileStat.h"
#include "Repository.h"
#include "Diff.h"
#include "Signature.h"

#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QDebug>

PaintWindow::PaintWindow(QWidget *parent, const Repository* repo) :
  QMainWindow(parent),
  ui(new Ui::PaintWindow),
  m_repo(repo)
{
  ui->setupUi(this);

  QMenu* layoutMenu = ui->menubar->addMenu("Layout");
  layoutMenu->addAction("dot", this, SLOT(layoutDot()));
  layoutMenu->addAction("circo", this, SLOT(layoutCirco()));
  layoutMenu->addAction("fdp", this, SLOT(layoutFdp()));
  layoutMenu->addAction("neato", this, SLOT(layoutNeato()));
  layoutMenu->addAction("nop", this, SLOT(layoutNop()));
  layoutMenu->addAction("nop1", this, SLOT(layoutNop1()));
  layoutMenu->addAction("nop2", this, SLOT(layoutNop2()));
  layoutMenu->addAction("osage", this, SLOT(layoutOsage()));
  layoutMenu->addAction("patchwork", this, SLOT(layoutPatchwork()));
  layoutMenu->addAction("sfdp", this, SLOT(layoutSfdp()));
  layoutMenu->addAction("twopi", this, SLOT(layoutTwopi()));

  ui->userList->setSelectionMode(QAbstractItemView::MultiSelection);
  connect(ui->userList->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
        this, SLOT(userSelectionChanged()));

  m_scene = new ZoomGraphicsScene(this);
  ui->graphicsView->setScene(m_scene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  new GraphicsViewZoom(ui->graphicsView);

  m_scene2 = new ZoomGraphicsScene(this);
  ui->graphicsView_2->setScene(m_scene2);
  ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
  new GraphicsViewZoom(ui->graphicsView_2);
  m_tooltip = new Tooltip;
  m_scene2->addItem(m_tooltip);

  g_root = new TreeNode(m_scene, "/");

  std::vector<CommitX*> commits = m_repo->getAllCommitsX();
  for (const auto& commit : commits) {
    std::vector<FileStat> affectedfiles = commit->getFiles();
    Contributer* user = addUser(commit->getAuthor(), affectedfiles);
    drawFiles(user, affectedfiles);
  }

  connectUsers();

  g_root->arrange<true>();
  g_root->itemChange(Node::ItemPositionChange, g_root->pos());

  // deallocate vector
  for (CommitX* commit : commits) {
    delete commit;
  }
  commits.clear();
}

void PaintWindow::connectUsers() {
  std::set<Contributer*> contribs;
  for (auto& file : files) {
    auto contributers = file.second->getContributers();
    for (Contributer* contributer : contributers) {
      contribs.insert(contributer);
      for (Contributer* contributer2 : contributers) {
        if (contributer == contributer2) {
          continue;
        }
        contributer->addAdjacentNode(contributer2);
      }
    }
  }

  ui->userList->model()->sort(0);

  m_layout = new LayoutThread(this, contribs, "dot");
  m_layout->start();

  m_strengths = new StrengthsThread(contribs);
  connect(m_strengths, &StrengthsThread::relation,
          this, &PaintWindow::relation);
  m_strengths->start();
}


void PaintWindow::drawFiles(Contributer* contrib, const std::vector<FileStat>& affectedfiles) {
  for (const FileStat& filestat : affectedfiles) {
    std::string file = filestat.filename;
    std::string rootpath;
    std::string filename;
    splitFile(file, &rootpath, &filename);

    if (files.find(file) != files.end()) { // found
      continue;
    }

    auto node = new LeafNode(m_scene, filename);
    files[file] = node;
    if (rootpath == "/") {
      g_root->addChildNode(node);
      contrib->addLeaf(node);
    } else {
      createFoldersRecursively(rootpath, &folders, g_root);
      // now every subfolder should exist
      size_t last_slash = rootpath.find_last_of('/');
      std::string subfolder;
      if (last_slash != std::string::npos) {
        subfolder = rootpath.substr(last_slash + 1);
      } else {
        subfolder = rootpath;
      }
      Q_ASSERT_X(folders[subfolder] != nullptr, rootpath.c_str(), subfolder.c_str());
      (folders[subfolder])->addChildNode(node);
      contrib->addLeaf(node);
    }
  }
}

inline void PaintWindow::splitFile(const std::string& file, std::string* root, std::string* filename) const {
  size_t last_slash = file.find_last_of('/');
  if (last_slash == std::string::npos) {
    *filename = file;
    *root = "/";
  } else {
    *filename = file.substr(last_slash + 1);
    *root = file.substr(0, last_slash);
  }
}

inline void PaintWindow::createFoldersRecursively(const std::string& rootdir,
                                                  std::map<std::string, TreeNode*>* folders,
                                                  TreeNode* rootnode) {
  size_t first_slash = rootdir.find_first_of('/');
  std::string first;
  if (first_slash == std::string::npos) {
    first = rootdir;
  } else {
    first = rootdir.substr(0, first_slash);
  }

  TreeNode* node;
  if (folders->find(first) == folders->end()) { // Not Found
    node = new TreeNode(m_scene, first);
    (*folders)[first] = node;
    rootnode->addChildNode(node);
  } else {
    node = (*folders)[first];
  }

  if (first != rootdir)
    createFoldersRecursively(rootdir.substr(first_slash + 1), folders, node);
}

inline Contributer* PaintWindow::addUser(const std::string& author, const std::vector<FileStat>& files) {
  Contributer* contrib;
  if (m_users.find(author) != m_users.end()) { // Found
    contrib = m_users[author];
  } else {
    contrib = new Contributer(m_scene2, author);
    m_users[author] = contrib;
    ui->userList->addItem(contrib);
  }

  for (const auto& filestat : files) {
    contrib->addFile(filestat.filename);
    if (this->files.find(filestat.filename) != this->files.end())
      this->files[filestat.filename]->addContributer(contrib);
  }

  return contrib;
}


void PaintWindow::setPos(Contributer *contrib, QPointF pos) {
  // FIXME: Free?
  QTimeLine* timer = new QTimeLine(2000);
  timer->setFrameRange(0, 100);
  QGraphicsItemAnimation* ani = new QGraphicsItemAnimation;
  ani->setItem(contrib);
  ani->setTimeLine(timer);

  QPointF startPos = contrib->pos();
  QPointF diff = pos - startPos;

  for (int i = 0; i < 200; ++i) {
    ani->setPosAt(i/200.0, QPointF(startPos.x() + i/200.0*diff.x(), startPos.y() + i/200.0*diff.y()));
  }

  timer->start();
}

void PaintWindow::setCurrentContributer(Contributer *contrib) {
  m_tooltip->setContributer(contrib);
}

void PaintWindow::userSelectionChanged() {
  QList<QListWidgetItem*> selected = ui->userList->selectedItems();

  if (selected.empty()) { // make all visible
    for (auto& contrib : m_users) {
      contrib.second->setVisible(true);
    }
    return;
  }

  std::set<Contributer*> visible;
  for (auto& item : selected) {
    Contributer* contrib = static_cast<Contributer*>(item);
    contrib->setVisible(true);
    visible.insert(contrib);
  }

  for (auto& contributer : m_users) {
    if (visible.count(contributer.second) == 0) {
      contributer.second->setVisible(false);
    }
  }
}


void PaintWindow::layout(const char* algorithm) {
  std::set<Contributer*> contribs;
  for (auto& file : files) {
    auto contributers = file.second->getContributers();
    for (Contributer* contributer : contributers) {
      contribs.insert(contributer);
      for (Contributer* contributer2 : contributers) {
        if (contributer == contributer2) {
          continue;
        }
        contributer->addAdjacentNode(contributer2);
      }
    }
  }

  m_layout->wait();
  delete m_layout;
  m_layout = new LayoutThread(this, contribs, algorithm);
  m_layout->start();
}

void PaintWindow::relation(Contributer* contrib1, Contributer* contrib2, int strength) {
  QGraphicsLineItem* line = contrib1->getLine(contrib2);
  Q_ASSERT(line != nullptr);
  QPen pen = line->pen();
  if (strength == 0) {
    pen.setColor(QColor::fromRgbF(0, 0, 0, 0.5));
  } else {
    pen.setWidth(strength);
  }
  line->setPen(pen);
}

PaintWindow::~PaintWindow()
{
  // We need terminate here because we may stuck in the library function
  m_layout->terminate();
  m_layout->wait();
  m_strengths->wait();
  delete ui;
  delete g_root;
  g_root = nullptr;
  delete m_layout;
  delete m_strengths;
  m_strengths = nullptr;
}

void LayoutThread::run() {
   ForceLayout layout(m_contribs);
   connect(&layout, &ForceLayout::posChanged,
           m_paintwindow, &PaintWindow::setPos);
   layout.exportToFile("dot.dot");
   qDebug() << "Exported";
   layout.layout(m_algo);
   qDebug() << "layout finished";
}

void StrengthsThread::run() {
    qDebug() << "Start strengh calculation";
    int max = 0;
    int min = INT_MAX;
    std::map<std::pair<Contributer*, Contributer*>, int> strengths;
    // get max and min strength
    for (Contributer* contrib : m_contribs) {
      for (Contributer* contrib2 : contrib->getContributers()) {
        Q_ASSERT(contrib2->containsContributer(contrib) && contrib->containsContributer(contrib2));
        const auto strength = contrib->calculateStrength(*contrib2);
        strengths[std::make_pair(contrib, contrib2)] = strength;
        max = std::max(max, strength);
        min = std::min(min, strength);
      }
    }

    for (auto& relation : strengths) {
      Contributer* contrib1 = relation.first.first;
      Contributer* contrib2 = relation.first.second;
      int strength = relation.second;
      int normalized = (float)strength / max * 10;
      emit this->relation(contrib1, contrib2, normalized);
    }

    qDebug() << "finished strength calculation";
}

