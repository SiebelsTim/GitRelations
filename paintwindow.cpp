#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "node.h"
#include "graphicsviewzoom.h"

#include "CommitIterator.h"
#include "Commit.h"
#include "Repository.h"
#include "Diff.h"

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
  new GraphicsViewZoom(ui->graphicsView);

  auto root = new Node(m_scene, "/");

  std::map<std::string, Node*> folders;
  std::map<std::string, Node*> files;

  for (const auto& commit : m_repo->iter()) {
    std::set<std::string> affectedfiles = commit.getAffectedFiles();
    for (const std::string& file : affectedfiles) {
      std::string rootpath;
      std::string filename;
      splitFile(file, &rootpath, &filename);

      if (files.find(file) != files.end()) { // found
        break;
      }

      auto node = new Node(m_scene, filename);
      files[file] = node;
      if (rootpath == "/") {
        root->addAdjacentNode(node);
      } else {
        createFoldersRecursively(rootpath, &folders, root);
        // now every subfolder should exist
        size_t last_slash = rootpath.find_last_of('/');
        std::string subfolder;
        if (last_slash != std::string::npos) {
          subfolder = rootpath.substr(last_slash + 1);
        } else {
          subfolder = rootpath;
        }
        Q_ASSERT_X(folders[subfolder] != nullptr, rootpath.c_str(), subfolder.c_str());
        (folders[subfolder])->addAdjacentNode(node);
      }
    }
  }
}

inline void PaintWindow::splitFile(const std::string& file, std::string* root, std::string* filename) {
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
                                                  std::map<std::string, Node*>* folders,
                                                  Node* rootnode) {
  size_t first_slash = rootdir.find_first_of('/');
  std::string first;
  if (first_slash == std::string::npos) {
    first = rootdir;
  } else {
    first = rootdir.substr(0, first_slash);
  }

  Node* node;
  if (folders->find(first) == folders->end()) { // Not Found
    node = new Node(m_scene, first);
    (*folders)[first] = node;
    rootnode->addAdjacentNode(node);
  } else {
    node = (*folders)[first];
  }

  if (first != rootdir)
    createFoldersRecursively(rootdir.substr(first_slash + 1), folders, node);
}

PaintWindow::~PaintWindow()
{
  delete ui;
}
