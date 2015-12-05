#ifndef CONTRIBUTER_H
#define CONTRIBUTER_H

#include "node.h"

class TreeNode;

class Contributer : public Node
{
public:

  explicit Contributer(QGraphicsScene* scene, const std::string& name): Node(scene, name) {}

  TreeNode* findNodeByPath(const TreeNode* root, const std::string& path) const;

  std::string getName() const {
    return getText();
  }

  void addFile(const std::string&) const;

  std::map<std::string, size_t> getFiles() const {
    return m_files;
  }

private:
  std::map<std::string, size_t> m_files;
};

#endif // CONTRIBUTER_H
