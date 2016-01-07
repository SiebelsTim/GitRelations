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

/**
 * @brief The PaintWindow class displays the graphs
 */
class PaintWindow : public QMainWindow
{
  Q_OBJECT

public:
  /**
   * @brief creates the graphs window
   * @param parent i.e. RepositoryWidget
   * @param repo Repository to display data for. Never null
   */
  explicit PaintWindow(QWidget *parent, const Repository* repo);
  ~PaintWindow();

  /**
   * @brief Adds and draws all files for a commits by a contributer
   *
   * Checks if files already exist
   * @param contrib Contributer who affected the files
   * @param affectedFiles the files
   */
  void drawFiles(Contributer* contrib, const std::vector<FileStat>& affectedFiles);
  /**
   * @brief Adds a relation to all contributers
   */
  void connectUsers();
  /**
   * @brief adds a user to the Contributers Scene
   * @param author name
   * @param files affected files
   * @return created Contributer.
   */
  Contributer* addUser(const std::string& author, const std::vector<FileStat>& files);

  /**
   * @brief layouts the contributers by a given algorithm
   *
   * libgraphviz does this for us :)
   * @param algorithm See LayoutThread for possible values
   */
  void layout(const char* algorithm);

protected:

  /**
   * @brief splits a filename into root folder and rest
   * @param file file to split
   * @param root Sets the value this pointer points to to the root folder
   * @param filename sets the value this pointer points to to the rest of the filename
   */
  void splitFile(const std::string& file, std::string* root, std::string* filename) const;
  /**
   * @brief creates TreeNodes recursively for a given path
   * @param rootdir a directory to create all subfolders to
   * @param folders Is filled with added folders
   * @param rootnode root node to search root dir's node for
   */
  void createFoldersRecursively(const std::string& rootdir,
                                std::map<std::string, TreeNode*>* folders,
                                TreeNode* rootnode);

private:
  /**
   * @brief Adds menuentries
   */
  void initMenu();
private:
  Ui::PaintWindow *ui;
  /**
   * @brief file scene
   */
  ZoomGraphicsScene* m_scene;
  /**
   * @brief contributer scene
   */
  ZoomGraphicsScene* m_scene2;
  const Repository* m_repo;
  /**
   * @brief ContributerWindow to reuse
   */
  ContributerWindow* m_contribwindow = nullptr;

  /**
   * @brief path to TreeNode mapping for folders
   */
  std::map<std::string, TreeNode*> folders;
  /**
   * @brief path to TreeNode mapping for files
   */
  std::map<std::string, TreeNode*> files;

  /**
   * @brief name to Contributer node mapping
   */
  std::map<std::string, Contributer*> m_users;

  /**
   * @brief Thread layouting contributers
   */
  LayoutThread* m_layout;
  /**
   * @brief Thread calculating strengths
   */
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

  /**
   * @brief sets and animates a positionchange for a contributer
   * @param contrib Contributer
   * @param pos new position
   */
  void setPos(Contributer* contrib, QPointF pos);
  /**
   * @brief Selection of list changed. Display appropriate contributers only.
   */
  void userSelectionChanged();
  /**
   * @brief Changes the strengths of a relation
   * @param strength new strength
   */
  void relation(Contributer*, Contributer*, int strength);

  /**
   * @brief displays contributersscene if checked
   * @param checked
   */
  void clickContributers(bool checked);
  /**
   * @brief displays filescene if checked
   * @param checked
   */
  void clickFiles(bool checked);
private slots:
  /**
   * @brief opens ContributerWindow
   * @param index index clicked
   */
  void on_userList_doubleClicked(const QModelIndex &index);
  /**
   * @brief Toggles display contributers only vs contributers and related contributers
   */
  void on_userListCheckbox_stateChanged(int);
};


/**
 * @brief Thread that does layouting and sends signals to PaintWindow
 *
 * Uses ForceLayout internally
 */
class LayoutThread : public QThread {
  Q_OBJECT

  const PaintWindow* m_paintwindow;
  std::set<Contributer*> m_contribs;
  const char* m_algo;
public:
  /**
   * @brief Creates the thread
   * @param paintwindow paintwindow to send data to
   * @param contribs contributers
   * @param algorithm dot | circo | fdp | neato | nop | nop1 | nop2 | osage | patchwork | sfdp | twopi
   */
  LayoutThread(PaintWindow* paintwindow, std::set<Contributer*> contribs, const char* algorithm):
    QThread(), m_paintwindow(paintwindow), m_contribs(contribs), m_algo(algorithm){
    Q_ASSERT(paintwindow);
  }

  virtual void run() override;
};

/**
 * @brief Calculates strengths and sends signals to PaintWindow
 */
class StrengthsThread : public QThread {
  Q_OBJECT

  std::set<Contributer*> m_contribs;
  const char* m_algo;
public:
  /**
   * @brief Creates the Thread
   * @param contribs contributers to calculate relations to
   */
  StrengthsThread(std::set<Contributer*> contribs):
    QThread(), m_contribs(contribs){}

  virtual void run() override;

signals:
  /**
   * @brief relation strength changed
   * @param strength new strength
   */
  void relation(Contributer*, Contributer*, int strength);

};

#endif // PAINTWINDOW_H
