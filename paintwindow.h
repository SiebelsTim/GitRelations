#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QItemSelection>
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
class QListWidgetItem;


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

  void layout(const char* algorithm);

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
  inline void layoutDot() {
    layout("dot");
  }
  inline void layoutCirco() {
    layout("circo");
  }
  inline void layoutFdp() {
    layout("fdp");
  }
  inline void layoutNeato() {
    layout("neato");
  }
  inline void layoutNop() {
    layout("nop");
  }
  inline void layoutNop1() {
    layout("nop1");
  }
  inline void layoutNop2() {
    layout("nop2");
  }
  inline void layoutOsage() {
    layout("osage");
  }
  inline void layoutPatchwork() {
    layout("patchwork");
  }
  inline void layoutSfdp() {
    layout("sfdp");
  }
  inline void layoutTwopi() {
    layout("twopi");
  }

  void setPos(Contributer* contrib, int x, int y);
  void userSelectionChanged();
};



class LayoutThread : public QThread {
  const PaintWindow* m_paintwindow;
  std::set<Contributer*> m_contribs;
  const char* m_algo;
public:
  LayoutThread(PaintWindow* paintwindow, std::set<Contributer*> contribs, const char* algorithm):
    QThread(), m_paintwindow(paintwindow), m_contribs(contribs), m_algo(algorithm){
    Q_ASSERT(paintwindow);
  }

  virtual void run() override;

};

#endif // PAINTWINDOW_H
