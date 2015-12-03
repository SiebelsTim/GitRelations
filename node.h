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

  void addChildNode(Node* node);
  void addLine(QGraphicsLineItem *line, bool isPoint1);
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
  void moveLines(QPointF newPos);


  // This is here, because it fails to link, because <true> isn't generated
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

    if (m_nodes.size() > 0) {
      for (auto& node : m_nodes) {
        node->arrange();
      }
    }
  }

  std::string getText() const;
  void setParent(Node* parent);
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
