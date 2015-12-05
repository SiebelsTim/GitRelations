#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include <algorithm>

class Node : public QGraphicsRectItem
{
public:
  explicit Node(QGraphicsScene* scene, const std::string& content, const QRectF& rect = QRectF(0, 0, 100, 30));

  void addAdjacentNode(Node* node);
  // This is the same as addAdjacentNode, but it only adds it to the vector
  void addNode(Node* node);
  void addLine(QGraphicsLineItem *line, bool isPoint1);
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
  void moveLines(QPointF newPos);

  bool containsChild(Node* node) const {
    return m_nodes.contains(node);
  }

  std::string getText() const;
protected:
  // <Line, is_p1>
  QVector<QPair<QGraphicsLineItem*, bool>> m_lines;
  QGraphicsTextItem* m_text;
  QGraphicsScene* m_scene;
  QVector<Node*> m_nodes;

signals:

public slots:

};

#endif // NODE_H
