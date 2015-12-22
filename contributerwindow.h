#ifndef CONTRIBUTERWINDOW_H
#define CONTRIBUTERWINDOW_H

#include <QMainWindow>

class Contributer;

namespace Ui {
class ContributerWindow;
}

class ContributerWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit ContributerWindow(Contributer* contrib, QWidget *parent = 0);
  ~ContributerWindow();

  void load();
  void setContributer(Contributer* contrib);

private:
  Ui::ContributerWindow *ui;
  Contributer* m_contrib;
};

#endif // CONTRIBUTERWINDOW_H
