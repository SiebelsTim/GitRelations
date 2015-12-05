#include "node.h"

Node::Node(QGraphicsScene* scene, const std::string& content, const QRectF& rect /* = QRectF(0, 0, 100, 30)*/)
  : QGraphicsRectItem(rect),
    m_scene(scene) {
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

  m_text = new QGraphicsTextItem(this);
  m_text->setPlainText(content.c_str());
  m_text->setPos(rect.topLeft());
  m_scene->addItem(m_text);
  m_scene->addItem(this);
}


void Node::addAdjacentNode(Node* node) {
  auto line = m_scene->addLine(0,0,0,0);
  addLine(line, true);
  node->addLine(line, false);
  addNode(node);
}

void Node::addNode(Node* node) {
  m_nodes.append(node);
}

void Node::addLine(QGraphicsLineItem *line, bool isPoint1) {
  m_lines.push_back(QPair<QGraphicsLineItem*, bool>(line, isPoint1));
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

  for (auto& line_pair : m_lines.toStdVector()) {
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

std::string Node::getText() const {
  if (m_text == nullptr) return "";
  return m_text->toPlainText().toStdString();
}
