#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include <set>

class Node : public QGraphicsRectItem
{
public:
  explicit Node(QGraphicsScene* scene,
                const std::string& content,
                const bool isLeaf = false,
                const QRectF& rect = QRectF(0, 0, 100, 30));

  void addAdjacentNode(Node* node);
  // This is the same as addAdjacentNode, but it only adds it to the vector
  void addNode(Node* node);
  void addLine(QGraphicsLineItem *line, bool isPoint1);
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
  void moveLines(QPointF newPos);

  void setIsLeaf(const bool isLeaf);
  bool isLeaf() const;

  // handles painting leaf as a circle
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  bool containsChild(Node* node) const {
    return m_nodes.count(node) != 0;
  }

  std::string getText() const;
protected:
  // <Line, is_p1>
  std::set<std::pair<QGraphicsLineItem*, bool>> m_lines;
  QGraphicsTextItem* m_text;
  QGraphicsScene* m_scene;
  std::set<Node*> m_nodes;
  bool m_is_leaf;

signals:

public slots:

};

#endif // NODE_H
