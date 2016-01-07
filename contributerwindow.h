#ifndef CONTRIBUTERWINDOW_H
#define CONTRIBUTERWINDOW_H

#include <QMainWindow>

#include "Commit.h"

class Contributer;
class Repository;

namespace Ui {
class ContributerWindow;
}

/**
 * @brief This window displays informations about a contributer
 *
 * - Related Contributers
 * - Affected files
 * - Graphs \n
 * -- Commits per hour of day \n
 * -- Commits per day \n
 */
class ContributerWindow : public QMainWindow
{
  Q_OBJECT

public:
  /**
   * @brief Creates the ContributerWindow
   * @param contrib contributer to display data for
   * @param repo repository to check contributer on
   * @param parent parent window (i.e. PaintWindow)
   */
  explicit ContributerWindow(Contributer* contrib, const Repository* repo, QWidget *parent = 0);
  ~ContributerWindow();
  /**
   * @brief changes contributer and updates data
   * @param contrib new Contributer
   */
  void setContributer(Contributer* contrib);

protected:
  /**
   * @brief initializes and fills table
   */
  void load();
  /**
   * @brief loads currently selected plot
   */
  void loadPlot();
  /**
   * @brief loads commits by day plot
   */
  void loadCommitsInTimespan();
  /**
   * @brief loads commits per hour of day plot
   */
  void loadCommitsPerHourOfDay();
  /**
   * @brief checks if a given commit belongs to current Contributer
   * @param commit commit to check for
   * @return true if current Contributer wrote given commit
   */
  bool commitBelongsToContributer(const Commit& commit) const;

private slots:
  /**
   * @brief updates plot if index is 2
   * @param index
   */
  void on_tabWidget_currentChanged(int index);

  /**
   * @brief display different plot
   * @param index
   */
  void on_comboBox_currentIndexChanged(int index);

private:
  Ui::ContributerWindow *ui;
  Contributer* m_contrib;
  const Repository* m_repo;
};

#endif // CONTRIBUTERWINDOW_H
