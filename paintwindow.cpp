#include "paintwindow.h"
#include "ui_paintwindow.h"

#include <QGraphicsItem>

PaintWindow::PaintWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::PaintWindow)
{
  ui->setupUi(this);
  QGraphicsScene* scene = new QGraphicsScene(this);
  QBrush greenBrush(Qt::green);
  QBrush blueBrush(Qt::blue);
  QPen outlinePen(Qt::black);
  outlinePen.setWidth(2);
  auto rect = scene->addRect(0, 0, 100, 100, outlinePen, blueBrush);
  rect->setFlag(QGraphicsItem::ItemIsMovable);
  ui->graphicsView->setScene(scene);
}

PaintWindow::~PaintWindow()
{
  delete ui;
}
