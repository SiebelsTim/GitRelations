#ifndef REPOSITORYWIDGET_H
#define REPOSITORYWIDGET_H

#include <QMainWindow>

namespace Ui {
class RepositoryWidget;
}

class Repository;

class RepositoryWidget : public QMainWindow
{
  Q_OBJECT

public:
  explicit RepositoryWidget(const Repository* repo, QWidget *parent = 0);
  ~RepositoryWidget();
  void loadFacts();

private slots:
  void on_pushButton_clicked();

private:
  const Repository* m_repo;
  Ui::RepositoryWidget *ui;
};

#endif // REPOSITORYWIDGET_H
