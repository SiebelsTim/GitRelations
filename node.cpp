#include "node.h"

#include <utility>
#include <QPainter>
#include "treenode.h"
#include "zoomgraphicsscene.h"

Node::Node(ZoomGraphicsScene* scene, const std::string& content, const QRectF& rect /* = QRectF(0, 0, 100, 30)*/)
  : QGraphicsRectItem(rect),
    m_scene(scene),
    m_text_str(content){
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

  m_text = new QGraphicsTextItem(this);
  m_text->setPlainText(content.c_str());
  m_text->setPos(rect.topLeft());
  m_scene->addItem(m_text);
  m_scene->addItem(this);
}


QGraphicsLineItem* Node::addAdjacentNode(Node* node) {
  if (containsChild(node) || node->containsChild(this)) return nullptr;
  auto line = m_scene->addLine(0,0,0,0);

  addNode(node);
  addLine(line, true, node);
  node->addLine(line, false, this);
  return line;
}

void Node::addNode(Node* node) {
  m_nodes.insert(node);
}

void Node::addLine(QGraphicsLineItem *line, bool isPoint1, Node* node) {
  m_lines[node] = std::make_pair(line, isPoint1);
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
    auto is_p1 = line_pair.second.second;
    auto line = line_pair.second.first;

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

QGraphicsLineItem* Node::getLine(Node* node) const {
  if (m_lines.find(node) == m_lines.end()) {
    return nullptr;
  }

  return m_lines.at(node).first;
}


std::string Node::getText() const {
  return m_text_str;
}

ZoomGraphicsScene* Node::getScene() const {
  return m_scene;
}

void Node::setVisible(bool visible) {
  for (std::pair<Node* const, std::pair<QGraphicsLineItem*, bool>>& line : m_lines) {
    line.second.first->setVisible(visible);
  }
  m_text->setVisible(visible);
  QGraphicsRectItem::setVisible(visible);
}


void Node::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  QColor red = QColor::fromRgb(255, 0, 0);
  for (auto& line : m_lines) {
    QPen pen = line.second.first->pen();
    if (m_line_colors.find(line.second.first) == m_line_colors.end()) { // only set once, else dblclicks would set it to red
      m_line_colors[line.second.first] = pen.color();
    }
    pen.setColor(red);
    line.second.first->setPen(pen);
  }

  QGraphicsRectItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  for (auto& line : m_lines) {
    QPen pen = line.second.first->pen();
    pen.setColor(m_line_colors[line.second.first]);
    line.second.first->setPen(pen);
  }

  QGraphicsRectItem::mouseReleaseEvent(event);
}
