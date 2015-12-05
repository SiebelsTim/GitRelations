#ifndef TREENODE_H
#define TREENODE_H
#include "node.h"

class TreeNode : public Node
{
  using Node::Node;
public:
  void addChildNode(TreeNode* node);
  void addAdjacentNode(Node* node) = delete;

  QVector<Node*> getChildren() const;

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

        static_cast<TreeNode*>(node)->setPos(xpos, ypos);
        ++i;
      }
    } else { // lay out in a line and spread
      int i = 0;
      const QPointF pos = this->pos();
      const auto count = m_nodes.count();
      const auto degree = (360.0f / 13) / count; // The 13 should be an 8 to be a quarter of a circle. But that's too much.
      for (auto& node : m_nodes) {
        double factor = i % 2 ? 1 : -1;
        int x = 2*pos.x();
        int y = 2*pos.y();
        QPointF newPos = QPointF(x, y) * QTransform().rotate(factor * degree * i);
        static_cast<TreeNode*>(node)->setPos(newPos);
        ++i;
      }

      // To get the proper line start, at top/bottom of the rectangle
      m_parent->moveLines(m_parent->pos());
    }

    if (m_nodes.size() > 0) {
      for (auto& node : m_nodes) {
        static_cast<TreeNode*>(node)->arrange();
      }
    }
  }
  void setParent(TreeNode* parent);
private:
  TreeNode* m_parent;

signals:

public slots:

};

#endif // TREENODE_H
