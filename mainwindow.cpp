#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include <QDebug>
#include "Repository.h"
#include "GitException.h"
#include "repositorywidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clearError();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setError(const char* msg) {
  ui->error_txt->setText(msg);
  ui->error_txt->setVisible(true);
}

void MainWindow::clearError() {
  ui->error_txt->setVisible(false);
}

void MainWindow::on_open_repo_btn_clicked()
{
  clearError();
  QFileDialog folder;
  folder.setFileMode(QFileDialog::DirectoryOnly);
  if (folder.exec()) {
    QString path = folder.selectedFiles()[0];
    try {
      const Repository* repo = new Repository(path.toStdString());
      auto reposcreen = new RepositoryWidget(repo, this);
      reposcreen->show();
    } catch (GitException e) {
      setError(e.what());
    }
  }
}
