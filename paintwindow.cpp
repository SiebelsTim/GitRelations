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

  Node* node3 = new Node;
  scene->addItem(node3);
  auto line = scene->addLine(100, 0, 100, 100);
  auto line2 = scene->addLine(100, 0, 100, 100);
  node->addAdjacentNode(node2, line);
  node->addAdjacentNode(node3, line2);
  ui->graphicsView->setScene(scene);
}

PaintWindow::~PaintWindow()
{
  delete ui;
}
