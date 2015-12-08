#include "treenode.h"


void TreeNode::addChildNode(TreeNode* node) {
  Node::addAdjacentNode(node);
  node->setParent(this);
}


void TreeNode::setParent(TreeNode* parent) {
  m_parent = parent;
}

std::set<Node*> TreeNode::getChildren() const {
  return m_nodes;
}

void TreeNode::hoverEnterEvent(QGraphicsSceneHoverEvent*) {
  m_text->setVisible(true);
}

void TreeNode::hoverLeaveEvent(QGraphicsSceneHoverEvent*) {
  if (isLeaf()) m_text->setVisible(false);
}

void TreeNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
  if (UNLIKELY(getText() != "/")) arrange();
  Node::mouseDoubleClickEvent(event);
}

QVariant TreeNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemPositionChange && scene()) {
    // Move circles with me
    constexpr float TAU = 2 * 3.1415926;
    const auto width = this->rect().width();
    const auto height = this->rect().height();

    int count = 0;
    for (auto& node : m_nodes) {
      if (!node->isLeaf()) ++count;
    }
    int leafcount = 0;
    int distance = width/2;

    auto leafs_per_circle = 2;
    auto leafdegree = TAU / leafs_per_circle;

    const QPointF pos = this->pos();
    for (auto& node : m_nodes) {
      if (node->isLeaf()) {
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
  }
  return Node::itemChange(change, value);
}
