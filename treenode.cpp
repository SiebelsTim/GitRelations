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
