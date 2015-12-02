#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include <algorithm>

class Node : public QGraphicsRectItem
{
public:
  explicit Node(QGraphicsScene* scene, const std::string& content, const QRectF& rect = QRectF(0, 0, 100, 30))
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


  void addChildNode(Node* node) {
    auto line = m_scene->addLine(0,0,0,0);
    addLine(line, true);
    node->addLine(line, false);
    node->setParent(this);
    m_nodes.append(node);
  }

  void addLine(QGraphicsLineItem *line, bool isPoint1) {
    m_lines.push_back(QPair<QGraphicsLineItem*, bool>(line, isPoint1));
  }

  QVariant itemChange(GraphicsItemChange change, const QVariant& value)
  {
    if (change == ItemPositionChange && scene()) {
      // value is the new position.
      QPointF newPos = value.toPointF();

      moveLines(newPos);
      m_text->setPos(newPos);
      if (getText() != "/")
        arrange();
    }
    return QGraphicsItem::itemChange(change, value);
  }

  void moveLines(QPointF newPos) {
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

  template <bool isRoot = false>
  void arrange() {
    constexpr float TAU = 2 * 3.1415926;
    if (isRoot) { // lay out in a circle
      int i = 0;
      const auto count = m_nodes.size();
      const auto degree = TAU / count;
      for (auto& node : m_nodes) {
        const qreal ypos = pos().y() + 300*cos(i * degree);
        const qreal xpos = pos().x() + 300*sin(i * degree);

        node->setPos(xpos, ypos);
        ++i;
      }
    } else { // lay out in a line and spread
      int i = 0;
      const QPointF parent_pos = m_parent->pos();
      const QPointF pos = this->pos();
      const int dist_x = std::min(abs(parent_pos.x() - pos.x()), 20);
      const int dist_y = std::min(abs(parent_pos.y() - pos.y()), 20);
      for (auto& node : m_nodes) {
        auto factor = i % 2 == 0 ? 1 : -1;
        int x = 2*pos.x() - parent_pos.x() + factor * dist_y*i;
        int y = 2*pos.y() - parent_pos.y() + factor * dist_x*i;
        node->setPos(x, y);
        ++i;
      }
    }
  }

  std::string getText() const {
    if (m_text == nullptr) return "";
    return m_text->toPlainText().toStdString();
  }

  void setParent(Node* parent) {
    m_parent = parent;
  }

private:
  // <Line, is_p1>
  QVector<QPair<QGraphicsLineItem*, bool>> m_lines;
  QGraphicsTextItem* m_text;
  QGraphicsScene* m_scene;
  QVector<Node*> m_nodes;
  Node* m_parent;

signals:

public slots:

};

#endif // NODE_H
