#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

/**
 * @brief Very simply window having a file dialog to open a repository
 *
 * Opens a RepositoryWidget if given folder is a valid git repository
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  /**
   * @brief Creates the window
   * @param parent usually nullptr
   */
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
    /**
   * @brief opens file dialog
   */
  void on_open_repo_btn_clicked();

private:
  /**
   * @brief sets error message
   * @param msg message
   */
  void setError(const char* msg);
  /**
   * @brief clears the error message
   */
  void clearError();
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
