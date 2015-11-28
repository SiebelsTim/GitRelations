#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "node.h"

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
        // TODO: add subfolders
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
    *filename = file.substr(last_slash);
    *root = file.substr(0, last_slash);
  }
}

PaintWindow::~PaintWindow()
{
  delete ui;
}
