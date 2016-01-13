#ifndef REPOSITORYWIDGET_H
#define REPOSITORYWIDGET_H

#include <QMainWindow>

namespace Ui {
class RepositoryWidget;
}

class Repository;

/**
 * @brief The RepositoryWidget class displaying very small information about the repository
 *
 * So you can decide if you want to open the graphs window
 */
class RepositoryWidget : public QMainWindow
{
  Q_OBJECT

public:
  /**
   * @brief Creates the window
   * @param repo for this repo. Never null
   * @param parent parent window (i.e. MainWindow)
   */
  explicit RepositoryWidget(const Repository* repo, QWidget *parent = 0);
  /**
   * @brief Don't copy widget as we can't copy a Repository object
   */
  RepositoryWidget(const RepositoryWidget&) = delete;
  /**
   * @brief Don't copy widget as we can't copy a Repository object
   */
  RepositoryWidget(const RepositoryWidget&&) = delete;
  /**
   * @brief frees the Repository
   */
  ~RepositoryWidget();

protected:
  /**
   * @brief loads an display very small facts about this repository
   */
  void loadFacts();

private slots:
  /**
   * @brief Opens the graphs window
   */
  void on_pushButton_clicked();

  /**
   * @brief Enables/Disables SinceDate
   * @param checked
   */
  void on_SinceChkBox_toggled(bool checked);
  /**
   * @brief Enables/Disables UntilDate
   * @param checked
   */
  void on_UntilChkBox_toggled(bool checked);

private:
  const Repository* m_repo;
  Ui::RepositoryWidget *ui;
};

#endif // REPOSITORYWIDGET_H
