#include "treenode.h"
#include "leafnode.h"

void TreeNode::addChildNode(TreeNode* node) {
  Node::addAdjacentNode(node);
  node->setParent(this);
}

void TreeNode::addChildNode(LeafNode* node) {
  node->setParent(this);
  m_leafs.insert(node);
}


void TreeNode::setParent(TreeNode* parent) {
  m_parent = parent;
}

std::set<Node*> TreeNode::getChildren() const {
  return m_nodes;
}

std::set<LeafNode*> TreeNode::getLeafs() const {
  return m_leafs;
}

void TreeNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
  if (LIKELY(getText() != "/")) arrange();
  else arrange<true>();

  Node::mouseDoubleClickEvent(event);
}

QVariant TreeNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemPositionChange && scene()) {
    // Move circles with me
    constexpr float TAU = 2 * 3.1415926;
    const auto width = this->rect().width();
    const auto height = this->rect().height();

    int leafcount = 0;
    int distance = width/2 + LEAF_SIZE / 2;

    auto leafs_per_circle = 8;
    auto leafdegree = TAU / leafs_per_circle;

    const QPointF pos = this->pos();
    for (auto& node : m_leafs) {
      if (UNLIKELY(leafcount == leafs_per_circle)) {
        distance += LEAF_SIZE;
        leafs_per_circle = leafs_per_circle + 5;
        leafdegree = TAU / leafs_per_circle;
        leafcount = 0;
      }
      const auto sinus = sin(leafdegree * leafcount);
      const auto cosinus = cos(leafdegree * leafcount);
      qreal x = pos.x() + width/2 - 15 + distance * sinus;
      qreal y = pos.y() + height/2 - 15 + distance * cosinus;
      node->setPos(x, y);
      ++leafcount;
    }
  }
  return Node::itemChange(change, value);
}
