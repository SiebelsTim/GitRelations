#ifndef TREENODE_H
#define TREENODE_H
#include "node.h"

#include "contributer.h"
#include <set>
#include "macros.h"

class LeafNode;

class TreeNode : public Node
{
public:
  explicit TreeNode(QGraphicsScene* scene,
                    const std::string& content,
                    const QRectF& rect = QRectF(0, 0, 100, 30))
    : Node(scene, content, rect){
  }

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

  void addChildNode(LeafNode* node);
  void addChildNode(TreeNode* node);
  void addAdjacentNode(Node* node) = delete;

  std::set<Node*> getChildren() const;
  std::set<LeafNode*> getLeafs() const;

  // This is here, because it fails to link, because <true> isn't generated
  template <bool isRoot = false>
  void arrange() {
    constexpr float TAU = 2 * 3.1415926;
    auto count = m_nodes.size();
    const auto degree = (360.0f / 8) / count;

    const QPointF pos = this->pos();
    const auto circledegree = TAU / count;

    // Changes files around it
    itemChange(ItemPositionChange, pos);

    int i = 0;
    for (auto& node : m_nodes) {
      if (isRoot) { // in a circle
        const qreal ypos = pos.y() + 600*cos(i * circledegree);
        const qreal xpos = pos.x() + 600*sin(i * circledegree);

        static_cast<TreeNode*>(node)->setPos(xpos, ypos);
        ++i;
      } else { // in a line and spread
        float factor = i % 2 ? 1 : -1;

        auto treenode = static_cast<TreeNode*>(node);
        float stretch =
            std::min(
              std::max(
                (float)treenode->getChildren().size() / 10 + (float)treenode->getLeafs().size()/ 40,
                1.5f),
              2.5f);
        int x = stretch*pos.x();
        int y = stretch*pos.y();
        QPointF newPos = QPointF(x, y) * QTransform().rotate(factor * degree * i);
        node->setPos(newPos);
        ++i;
      }
    }

    if (m_nodes.size() > 0) {
      for (auto& node : m_nodes) {
        static_cast<TreeNode*>(node)->arrange();
      }
    }

  }


  void setParent(TreeNode* parent);

  void addContributer(Contributer* contributer) {
    m_contributers.insert(contributer);
  }

  std::multiset<Contributer*> getContributers() const {
    return m_contributers;
  }

private:
  TreeNode* m_parent;
  std::multiset<Contributer*> m_contributers;
  std::set<LeafNode*> m_leafs;

};

#endif // TREENODE_H
