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
class Tooltip;
class LayoutThread;
class QListWidgetItem;
class StrengthsThread;
class ZoomGraphicsScene;
class ContributerWindow;


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
  void initMenu();
private:
  Ui::PaintWindow *ui;
  ZoomGraphicsScene* m_scene;
  ZoomGraphicsScene* m_scene2;
  const Repository* m_repo;
  ContributerWindow* m_contribwindow = nullptr;

  std::map<std::string, TreeNode*> folders;
  std::map<std::string, TreeNode*> files;

  std::map<std::string, Contributer*> m_users;

  LayoutThread* m_layout;
  StrengthsThread* m_strengths;

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

  void setPos(Contributer* contrib, QPointF pos);
  void userSelectionChanged();
  void relation(Contributer*, Contributer*, int strength);

  void clickContributers(bool checked);
  void clickFiles(bool checked);
private slots:
  void on_userList_doubleClicked(const QModelIndex &index);
  void on_userListCheckbox_stateChanged(int arg1);
};



class LayoutThread : public QThread {
  Q_OBJECT

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

class StrengthsThread : public QThread {
  Q_OBJECT

  std::set<Contributer*> m_contribs;
  const char* m_algo;
public:
  StrengthsThread(std::set<Contributer*> contribs):
    QThread(), m_contribs(contribs){}

  virtual void run() override;

signals:
  void relation(Contributer*, Contributer*, int strength);

};

#endif // PAINTWINDOW_H
