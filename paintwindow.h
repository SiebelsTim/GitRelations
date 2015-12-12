#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QThread>
#include <set>

#include "FileStat.h"

namespace Ui {
class PaintWindow;
}

class QGrapicsScene;
class Repository;
class TreeNode;
class Contributer;
class LayoutThread;


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

  void drawFiles(Contributer* contrib, const std::vector<FileStat>& affectedFiles);
  void connectUsers();
  Contributer* addUser(const std::string& author, const std::vector<FileStat>& files);

  void calcStrengthLimits(std::set<Contributer*>) const;

private:
  Ui::PaintWindow *ui;
  QGraphicsScene* m_scene;
  QGraphicsScene* m_scene2;
  const Repository* m_repo;
  std::map<std::string, TreeNode*> folders;
  std::map<std::string, TreeNode*> files;

  std::map<std::string, Contributer*> m_users;

  LayoutThread* m_layout;

public slots:
  void setPos(Contributer* contrib, int x, int y);
};



class LayoutThread : public QThread {
  PaintWindow* m_paintwindow;
  std::set<Contributer*> m_contribs;
public:
  LayoutThread(PaintWindow* paintwindow, std::set<Contributer*> contribs):
    QThread(), m_paintwindow(paintwindow), m_contribs(contribs){
    Q_ASSERT(paintwindow);
  }

  virtual void run() override;

};

#endif // PAINTWINDOW_H
