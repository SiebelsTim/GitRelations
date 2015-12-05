#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <set>

namespace Ui {
class PaintWindow;
}

class QGrapicsScene;
class Repository;
class TreeNode;
class Node;
class Signature;

class PaintWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit PaintWindow(QWidget *parent, const Repository* repo);
  ~PaintWindow();

  void splitFile(const std::string& file, std::string* root, std::string* filename) const;
  void createFoldersRecursively(const std::string& rootdir,
                                             std::map<std::string, TreeNode*>* folders,
                                             TreeNode* rootnode);

  void drawFiles(const std::set<std::string>& affectedFiles);
  void addUser(const Signature& author);

private:
  Ui::PaintWindow *ui;
  QGraphicsScene* m_scene;
  QGraphicsScene* m_scene2;
  const Repository* m_repo;
  std::map<std::string, TreeNode*> folders;
  std::map<std::string, TreeNode*> files;
  TreeNode* m_root;

  std::map<std::string, Node*> m_users;
};

#endif // PAINTWINDOW_H
