#include "contributer.h"
#include "treenode.h"


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
    std::set<Node*> children = root->getChildren();
    for (const auto& node : children) {
      TreeNode* tnode = static_cast<TreeNode*>(node);
      if (tnode->getText() == path) {
        return tnode;
      }
    }
  }
  Q_ASSERT(false);
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
