#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "node.h"

#include <QGraphicsItem>

PaintWindow::PaintWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::PaintWindow)
{
  ui->setupUi(this);
  QGraphicsScene* scene = new QGraphicsScene(this);

  Node* node = new Node;
  scene->addItem(node);

  Node* node2 = new Node;
  scene->addItem(node2);
  auto line = scene->addLine(100, 0, 100, 100);
  node->addAdjacentNode(node2, line);
  ui->graphicsView->setScene(scene);
}

PaintWindow::~PaintWindow()
{
  delete ui;
}
