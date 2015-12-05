#include "treenode.h"


void TreeNode::addChildNode(TreeNode* node) {
  Node::addAdjacentNode(node);
  node->setParent(this);
}


void TreeNode::setParent(TreeNode* parent) {
  m_parent = parent;
}

QVector<Node*> TreeNode::getChildren() const {
  return m_nodes;
}
