#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int WELCOMESCREEN = 0;
    static const int REPOSCREEN    = 1;
    static const int GRAPHSCREEN   = 2;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
  void on_open_repo_btn_clicked();

private:
  void setError(const char* msg);
  void clearError();
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
