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

private:
  Ui::PaintWindow *ui;
  QGraphicsScene* m_scene;
  const Repository* m_repo;
  std::map<std::string, TreeNode*> folders;
  std::map<std::string, TreeNode*> files;
  TreeNode* m_root;
};

#endif // PAINTWINDOW_H
