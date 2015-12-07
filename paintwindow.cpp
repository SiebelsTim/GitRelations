#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "treenode.h"
#include "graphicsviewzoom.h"
#include "contributer.h"

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
    std::vector<std::string> affectedfiles = commit.getAffectedFiles();
    drawFiles(affectedfiles);
    addUser(commit.author(), affectedfiles);
  }

  // TODO: Thread?
  connectUsers();

  m_root->arrange<true>();
  m_root->itemChange(Node::ItemPositionChange, m_root->pos());
}

void PaintWindow::connectUsers() {
  for (auto& file : files) {
    auto contributers = file.second->getContributers();
    for (Contributer* contributer : contributers) {
      for (Contributer* contributer2 : contributers) {
        contributer->addAdjacentNode(contributer2);
      }
    }
  }
}

void PaintWindow::drawFiles(const std::vector<std::string>& affectedfiles) {
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

inline Contributer* PaintWindow::addUser(const Signature& author, const std::vector<std::string>& files) {
  Contributer* contrib;
  if (m_users.find(author.name()) != m_users.end()) { // Found
    contrib = m_users[author.name()];
  } else {
    contrib = new Contributer(m_scene2, author.name());
    m_users[author.name()] = contrib;
    ui->userList->addItem(contrib);
  }

  for (const auto& file : files) {
    contrib->addFile(file);
    if (this->files.find(file) != this->files.end())
      this->files[file]->addContributer(contrib);
  }

  return contrib;
}

PaintWindow::~PaintWindow()
{
  delete ui;
}
