#include "contributer.h"
#include "treenode.h"
#include "leafnode.h"
#include "macros.h"


TreeNode* Contributer::findNodeByPath(const TreeNode* root, const std::string& path) const {
  auto firstslash = path.find_first_of('/');
  if (firstslash != std::string::npos) { // Found
    std::string firstfolder = path.substr(0, firstslash);
    std::set<Node*> children = root->getChildren();
    for (const auto& node : children) {
      TreeNode* tnode = static_cast<TreeNode*>(node);
      if (tnode->getText() == firstfolder) {
        std::string strippedPath = path.substr(firstslash + 1);
        return findNodeByPath(tnode, strippedPath);
      }
    }
  } else { // Not found == root
    std::set<LeafNode*> children = root->getLeafs();
    for (const auto& node : children) {
      if (node->getText() == path) {
        return node;
      }
    }
  }
  Q_ASSERT_X(false, root->getText().c_str(), path.c_str());
  return nullptr;
}

void Contributer::addFile(const std::string& path) {
  if (m_files.find(path) != m_files.end()) { // found
    ++m_files[path];
  } else {
    m_files[path] = 1;
  }
}

QGraphicsLineItem* Contributer::addAdjacentNode(Node* node) {
  auto ret = Node::addAdjacentNode(node);
  node->addNode(this);
  return ret;
}


void Contributer::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  QColor red = QColor::fromRgb(255, 0, 0);
  for (auto& leaf : m_leafs) {
    QPen pen = leaf->pen();
    pen.setColor(red);
    leaf->setPen(pen);
  }

  Node::mousePressEvent(event);
}

void Contributer::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  QColor black = QColor::fromRgb(0, 0, 0);
  for (auto& leaf : m_leafs) {
    QPen pen = leaf->pen();
    pen.setColor(black);
    leaf->setPen(pen);
  }


  Node::mouseReleaseEvent(event);
}
