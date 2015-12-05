#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "treenode.h"
#include "graphicsviewzoom.h"

#include "CommitIterator.h"
#include "Commit.h"
#include "Repository.h"
#include "Diff.h"
#include "Signature.h"

#include <QGraphicsItem>
#include <QDebug>

PaintWindow::PaintWindow(QWidget *parent, const Repository* repo) :
  QMainWindow(parent),
  ui(new Ui::PaintWindow),
  m_repo(repo)
{
  ui->setupUi(this);
  m_scene = new QGraphicsScene(this);
  ui->graphicsView->setScene(m_scene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  new GraphicsViewZoom(ui->graphicsView);

  m_scene2 = new QGraphicsScene(this);
  ui->graphicsView_2->setScene(m_scene2);
  ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
  new GraphicsViewZoom(ui->graphicsView_2);

  m_root = new TreeNode(m_scene, "/");

  for (const auto& commit : m_repo->iter()) {
    std::set<std::string> affectedfiles = commit.getAffectedFiles();
    drawFiles(affectedfiles);
    addUser(commit.author());
  }

  m_root->arrange<true>();
  // set correct position of lines here now.
  // addAdjacentNode used to do this, but it will do some unneseccary computations
  for (const auto& folder : folders) {
    folder.second->arrange();
  }

  for (const auto& file : files) {
    file.second->arrange();
  }
  m_root->itemChange(Node::ItemPositionChange, m_root->pos());
}

void PaintWindow::drawFiles(const std::set<std::string>& affectedfiles) {
  for (const std::string& file : affectedfiles) {
    std::string rootpath;
    std::string filename;
    splitFile(file, &rootpath, &filename);

    if (files.find(file) != files.end()) { // found
      break;
    }

    auto node = new TreeNode(m_scene, filename);
    files[file] = node;
    if (rootpath == "/") {
      m_root->addChildNode(node);
    } else {
      createFoldersRecursively(rootpath, &folders, m_root);
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

inline void PaintWindow::addUser(const Signature& author) {
  if (m_users.find(author.name()) != m_users.end()) return;  // Found
  auto node = new Node(m_scene2, author.name());
  m_users[author.name()] = node;
}

PaintWindow::~PaintWindow()
{
  delete ui;
}
