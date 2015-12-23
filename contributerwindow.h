#ifndef CONTRIBUTERWINDOW_H
#define CONTRIBUTERWINDOW_H

#include <QMainWindow>

#include "Commit.h"

class Contributer;
class Repository;

namespace Ui {
class ContributerWindow;
}

class ContributerWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit ContributerWindow(Contributer* contrib, const Repository* repo, QWidget *parent = 0);
  ~ContributerWindow();

  void load();
  void loadPlot();
  void loadCommitsInTimespan();
  void loadCommitsPerHourOfDay();
  void setContributer(Contributer* contrib);
  bool commitBelongsToContributer(const Commit& commit) const;

private slots:
  void on_tabWidget_currentChanged(int index);

private:
  Ui::ContributerWindow *ui;
  Contributer* m_contrib;
  const Repository* m_repo;
};

#endif // CONTRIBUTERWINDOW_H
