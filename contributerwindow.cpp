#include "contributerwindow.h"
#include "ui_contributerwindow.h"

#include "contributer.h"
#include "Repository.h"
#include "Commit.h"
#include "CommitIterator.h"
#include "Signature.h"

#include "qcustomplot.h"

ContributerWindow::ContributerWindow(Contributer* contrib, const Repository* repo, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ContributerWindow),
  m_contrib(contrib),
  m_repo(repo)
{
  Q_ASSERT(contrib);
  Q_ASSERT(repo);
  ui->setupUi(this);
  load();
}

void ContributerWindow::load() {
  ui->username->setText(m_contrib->getName().c_str());
  std::map<std::string, size_t> files = m_contrib->getFiles();
  this->setWindowTitle(m_contrib->getName().c_str());

  ui->tableWidget->setColumnCount(2);
  ui->tableWidget->setRowCount(m_contrib->getContributers().size() + files.size() + 2);
  ui->tableWidget->horizontalHeader()->hide();
  int row = 0;

  auto contribheader = new QTableWidgetItem("Related Contributers");
  contribheader->setBackgroundColor(QColor::fromRgb(150, 150, 150));
  contribheader->setTextColor(QColor::fromRgb(255, 255, 255));
  ui->tableWidget->setItem(row++, 0, contribheader);

  for (auto contributer : m_contrib->getContributers()) {
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(contributer->getName().c_str()));
    ++row;
  }

  auto fileheader = new QTableWidgetItem("Affected Files");
  fileheader->setBackgroundColor(QColor::fromRgb(150, 150, 150));
  fileheader->setTextColor(QColor::fromRgb(255, 255, 255));
  ui->tableWidget->setItem(row, 0, fileheader);
  auto fileheadersize = new QTableWidgetItem("Number of Commits");
  fileheadersize->setBackgroundColor(QColor::fromRgb(150, 150, 150));
  fileheadersize->setTextColor(QColor::fromRgb(255, 255, 255));
  ui->tableWidget->setItem(row++, 1, fileheadersize);

  for (auto file : files) {
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(file.first.c_str()));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(file.second)));
    ++row;
  }

  ui->tableWidget->resizeColumnsToContents();
}


void ContributerWindow::loadCommitsInTimespan() {
  ui->headline->setText("Commits");

  QMap<double, double> values;

  for (const auto& commit : m_repo->iter()) {
    if (!commitBelongsToContributer(commit)) continue;

    time_t time = commit.time();
    tm* tm = gmtime(&time);
    // Get only full day
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    time_t date = mktime(tm);
    values[date]++;
  }

  // Set day before and after to 0 if not existing
  for (auto& key : values.keys()) {
    if (!values.contains(key + 60*60*24)) {
      values[key + 60*60*24] = 0;
    }
    if (!values.contains(key - 60*60*24)) {
      values[key - 60*60*24] = 0;
    }
  }

  ui->plot->clearGraphs();
  ui->plot->addGraph();
  ui->plot->graph(0)->setData(
        values.keys().toVector(), values.values().toVector());
  ui->plot->rescaleAxes();
  ui->plot->xAxis->setLabel("Time");
  ui->plot->yAxis->setLabel("Commit count");
  ui->plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->plot->xAxis->setDateTimeFormat("MMMM\nyyyy");
  ui->plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  ui->plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  ui->plot->graph(0)->setBrush(QBrush(QColor::fromRgbF(0.3, 0.3, 1.0, 0.5)));
  ui->plot->replot();
}

void ContributerWindow::loadCommitsPerHourOfDay() {
  ui->headline->setText("Commits by hour of day");

  QMap<double, double> values;
  // Insert 0 for every hour, so we can see spikes
  for (int i = 0; i < 24; ++i) {
    values.insert(i, 0);
  }

  for (const auto& commit : m_repo->iter()) {
    if (!commitBelongsToContributer(commit)) continue;

    time_t time = commit.time();
    tm* tm = gmtime(&time);
    values[tm->tm_hour]++;
  }

  ui->plot->clearGraphs();
  ui->plot->addGraph();
  ui->plot->graph(0)->setData(
        values.keys().toVector(), values.values().toVector());
  ui->plot->rescaleAxes();
  ui->plot->xAxis->setRange(0, 23);
  ui->plot->xAxis->setLabel("Hour of day");
  ui->plot->yAxis->setLabel("Commit count");
  ui->plot->xAxis->setTickLabelType(QCPAxis::ltNumber);
  ui->plot->xAxis->setTickStep(1.0);
  ui->plot->graph(0)->setBrush(QBrush(QColor::fromRgbF(0.3, 0.3, 1.0, 0.5)));
  ui->plot->replot();
}

bool ContributerWindow::commitBelongsToContributer(const Commit& commit) const {
  return commit.author().name() == m_contrib->getName();
}

void ContributerWindow::setContributer(Contributer* contrib) {
  Q_ASSERT(contrib);
  m_contrib = contrib;
  ui->tableWidget->setRowCount(0); // This empties the table
  load();
  if (ui->tabWidget->currentIndex() == 1) {
    on_comboBox_currentIndexChanged(ui->comboBox->currentIndex());
  }
}

ContributerWindow::~ContributerWindow()
{
  delete ui;
}

void ContributerWindow::on_tabWidget_currentChanged(int index)
{
  if (index == 1) {
    on_comboBox_currentIndexChanged(ui->comboBox->currentIndex());
  }
}

void ContributerWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
      loadCommitsInTimespan();
      break;
    case 1:
      loadCommitsPerHourOfDay();
      break;
    }
}
