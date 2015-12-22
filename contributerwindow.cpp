#include "contributerwindow.h"
#include "ui_contributerwindow.h"

#include "contributer.h"

ContributerWindow::ContributerWindow(Contributer* contrib, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ContributerWindow),
  m_contrib(contrib)
{
  Q_ASSERT(contrib);
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

void ContributerWindow::setContributer(Contributer* contrib) {
  Q_ASSERT(contrib);
  m_contrib = contrib;
  ui->tableWidget->setRowCount(0); // This empties the table
  load();
}

ContributerWindow::~ContributerWindow()
{
  delete ui;
}
