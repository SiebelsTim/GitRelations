#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "treenode.h"
#include "leafnode.h"
#include "graphicsviewzoom.h"
#include "contributer.h"

#include "CommitIterator.h"
#include "Commit.h"
#include "CommitX.h"
#include "FileStat.h"
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

  std::vector<CommitX*> commits = m_repo->getAllCommitsX();
  for (const auto& commit : commits) {
    std::vector<FileStat> affectedfiles = commit->getFiles();
    drawFiles(affectedfiles);
    addUser(commit->getAuthor(), affectedfiles);
  }

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

void PaintWindow::drawFiles(const std::vector<FileStat>& affectedfiles) {
  for (const FileStat& filestat : affectedfiles) {
    std::string file = filestat.filename;
    std::string rootpath;
    std::string filename;
    splitFile(file, &rootpath, &filename);

    if (files.find(file) != files.end()) { // found
      break;
    }

    auto node = new LeafNode(m_scene, filename);
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

PaintWindow::~PaintWindow()
{
  delete ui;
}
