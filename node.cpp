#include "node.h"

#include <QPainter>
#include "treenode.h"

Node::Node(QGraphicsScene* scene, const std::string& content, const bool isLeaf /* = false*/, const QRectF& rect /* = QRectF(0, 0, 100, 30)*/)
  : QGraphicsRectItem(rect),
    m_scene(scene) {
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

  m_text = new QGraphicsTextItem(this);
  m_text->setPlainText(content.c_str());
  m_text->setPos(rect.topLeft());
  m_scene->addItem(m_text);
  m_scene->addItem(this);

  setIsLeaf(isLeaf);
}


void Node::addAdjacentNode(Node* node) {
  if (containsChild(node) || node->containsChild(this)) return;
  auto line = m_scene->addLine(0,0,0,0);
  addLine(line, true);
  node->addLine(line, false);
  addNode(node);

  if (isLeaf() || node->isLeaf()) {
    line->setVisible(false);
  }
}

void Node::addNode(Node* node) {
  m_nodes.insert(node);
}

void Node::addLine(QGraphicsLineItem *line, bool isPoint1) {
  m_lines.emplace(line, isPoint1);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemPositionChange && scene()) {
    // value is the new position.
    QPointF newPos = value.toPointF();

    moveLines(newPos);
    m_text->setPos(newPos);
  }
  return QGraphicsItem::itemChange(change, value);
}

void Node::moveLines(QPointF newPos) {
  // Converts the elipse position (top-left)
  // to its center position
  int xOffset = rect().x() + rect().width()/2;

  for (auto& line_pair : m_lines) {
    auto is_p1 = line_pair.second;
    auto line = line_pair.first;

    QPointF other_pnt = is_p1 ? line->line().p2() : line->line().p1();

    QPointF newLinePos;
    if (other_pnt.y() > newPos.y()) {
      newLinePos = QPointF(newPos.x() + xOffset, newPos.y() + rect().height());
    } else {
      newLinePos = QPointF(newPos.x() + xOffset, newPos.y());
    }

    // Move the required point of the line to the center of the elipse
    QPointF p1 = is_p1 ? newLinePos : line->line().p1();
    QPointF p2 = is_p1 ? line->line().p2() : newLinePos;

    line->setLine(QLineF(p1, p2));
  }
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  if (m_is_leaf) {
    painter->drawEllipse(rect());
  } else {
    QGraphicsRectItem::paint(painter, option, widget);
  }
}

std::string Node::getText() const {
  if (m_text == nullptr) return "";
  return m_text->toPlainText().toStdString();
}

void Node::setIsLeaf(const bool isLeaf) {
  if (isLeaf && !m_is_leaf) {
    setRect(0, 0, LEAF_SIZE, LEAF_SIZE);
    m_text->setVisible(false);
    for (auto& line : m_lines) {
      line.first->setVisible(false);
    }
  } else if(!isLeaf && m_is_leaf) {
    setRect(0, 0, 100, 30);
    m_text->setVisible(true);
  }
  m_is_leaf = isLeaf;
  update();
}

bool Node::isLeaf() const {
  return m_is_leaf;
}
