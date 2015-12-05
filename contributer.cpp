#include "contributer.h"
#include "treenode.h"


TreeNode* Contributer::findNodeByPath(const TreeNode* root, const std::string& path) const {
  auto firstslash = path.find_first_of('/');
  if (firstslash != std::string::npos) { // Found
    std::string firstfolder = path.substr(0, firstslash);
    QVector<Node*> children = root->getChildren();
    for (const auto& node : children) {
      TreeNode* tnode = static_cast<TreeNode*>(node);
      if (tnode->getText() == firstfolder) {
        std::string strippedPath = path.substr(firstslash + 1);
        return findNodeByPath(tnode, strippedPath);
      }
    }
  } else { // Not found == root
    QVector<Node*> children = root->getChildren();
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
