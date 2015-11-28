#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

class Node : public QGraphicsEllipseItem
{
public:
  explicit Node(QGraphicsScene* scene, const std::string& content, const QRectF& rect = QRectF(0, 0, 100, 100))
    : QGraphicsEllipseItem(rect),
      m_scene(scene) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    m_text = new QGraphicsTextItem(this);
    m_text->setPlainText(content.c_str());
    m_text->setPos(rect.topLeft());
    m_scene->addItem(m_text);
    m_scene->addItem(this);
  }


  void addAdjacentNode(Node* node) {
    auto line = m_scene->addLine(0,0,0,0);
    addLine(line, true);
    node->addLine(line, false);

    // put the possibly wrong positioned line into the right position
    itemChange(ItemPositionChange, pos());
    node->itemChange(ItemPositionChange, node->pos());
  }

  void addLine(QGraphicsLineItem *line, bool isPoint1) {
    m_lines.push_back(QPair<QGraphicsLineItem*, bool>(line, isPoint1));
  }

  QVariant itemChange(GraphicsItemChange change, const QVariant& value)
  {
    if (change == ItemPositionChange && scene()) {
      // value is the new position.
      QPointF newPos = value.toPointF();

      moveLinesToCenter(newPos);
      m_text->setPos(newPos);
    }
    return QGraphicsItem::itemChange(change, value);
  }

  void moveLinesToCenter(QPointF newPos) {
    // Converts the elipse position (top-left)
    // to its center position
    int xOffset = rect().x() + rect().width()/2;
    int yOffset = rect().y() + rect().height()/2;

    QPointF newCenterPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);

    for (auto& line_pair : m_lines.toStdVector()) {
      auto is_p1 = line_pair.second;
      auto line = line_pair.first;

      // Move the required point of the line to the center of the elipse
      QPointF p1 = is_p1 ? newCenterPos : line->line().p1();
      QPointF p2 = is_p1 ? line->line().p2() : newCenterPos;

      line->setLine(QLineF(p1, p2));
    }
  }

  std::string getText() const {
    if (m_text == nullptr) return "";
    return m_text->toPlainText().toStdString();
  }

private:
  // <Line, is_p1>
  QVector<QPair<QGraphicsLineItem*, bool>> m_lines;
  QGraphicsTextItem* m_text;
  QGraphicsScene* m_scene;

signals:

public slots:

};

#endif // NODE_H