#include "repositorywidget.h"
#include "ui_repositorywidget.h"
#include "Repository.h"
#include "Commit.h"
#include "CommitIterator.h"
#include "Signature.h"

#include "paintwindow.h"

#include <QListWidget>

RepositoryWidget::RepositoryWidget(const Repository* repo, QWidget *parent) :
  QMainWindow(parent),
  m_repo(repo),
  ui(new Ui::RepositoryWidget)
{
  Q_ASSERT(repo);
  ui->setupUi(this);
  setWindowTitle(m_repo->getName().c_str());
  loadFacts();
}

RepositoryWidget::~RepositoryWidget()
{
  delete ui;
  delete m_repo;
}

void RepositoryWidget::loadFacts() {
  QVector<QString> contributers;
  int number_of_commits = 0;
  for (const auto& commit : m_repo->iter()) {
    ++number_of_commits;
    QString contributer = commit.author().mail().c_str();
    if (!contributers.contains(contributer)) {
      contributers.append(contributer);
    }
  }
  int number_of_contributers = contributers.size();
  ui->facts->addItem(QString().sprintf("Number of Commits: %d", number_of_commits));
  ui->facts->addItem(QString().sprintf("Number of Contributers: %d", number_of_contributers));
}

void RepositoryWidget::on_pushButton_clicked()
{
  uint since = 0;
  uint until = 0;
  if (ui->SinceChkBox->isChecked()) {
    since = ui->SinceDate->dateTime().toTime_t();
  }
  if (ui->UntilChkBox->isChecked()) {
    until = ui->UntilDate->dateTime().toTime_t();
  }

  auto paint = new PaintWindow(this, m_repo, std::make_pair(since, until));
  paint->showMaximized();
}

void RepositoryWidget::on_SinceChkBox_toggled(bool checked)
{
    ui->SinceDate->setEnabled(checked);
}

void RepositoryWidget::on_UntilChkBox_toggled(bool checked)
{
    ui->UntilDate->setEnabled(checked);
}
