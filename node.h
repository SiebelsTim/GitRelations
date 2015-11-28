#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>

class Node : public QGraphicsEllipseItem
{
public:
  explicit Node(const QRectF& rect) : QGraphicsEllipseItem(rect) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
  }

  explicit Node(): Node(QRectF(0, 0, 30, 30)) {
  }

  void addAdjacentNode(Node* node, QGraphicsLineItem* line) {
    addLine(line, true);
    node->addLine(line, false);

    // put the possibly wrong positioned line into the right position
    itemChange(ItemPositionChange, pos());
    node->itemChange(ItemPositionChange, node->pos());
  }

  void addLine(QGraphicsLineItem *line, bool isPoint1) {
    m_line = line;
    m_is_p1 = isPoint1;
  }

  QVariant itemChange(GraphicsItemChange change, const QVariant& value)
  {
    if (change == ItemPositionChange && scene()) {
      // value is the new position.
      QPointF newPos = value.toPointF();

      moveLineToCenter(newPos);
    }
    return QGraphicsItem::itemChange(change, value);
  }

  void moveLineToCenter(QPointF newPos) {
    // Converts the elipse position (top-left)
    // to its center position
    int xOffset = rect().x() + rect().width()/2;
    int yOffset = rect().y() + rect().height()/2;

    QPointF newCenterPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);

    // Move the required point of the line to the center of the elipse
    QPointF p1 = m_is_p1 ? newCenterPos : m_line->line().p1();
    QPointF p2 = m_is_p1 ? m_line->line().p2() : newCenterPos;

    m_line->setLine(QLineF(p1, p2));
  }

private:
  QGraphicsLineItem *m_line;
  bool m_is_p1;

signals:

public slots:

};

#endif // NODE_H
