#ifndef CONTRIBUTER_H
#define CONTRIBUTER_H

#include "node.h"
#include <QListWidgetItem>

class TreeNode;
class LeafNode;

class Contributer : public Node, public QListWidgetItem
{
public:

  explicit Contributer(QGraphicsScene* scene, const std::string& name): Node(scene, name), QListWidgetItem(name.c_str()) {}

  TreeNode* findNodeByPath(const TreeNode* root, const std::string& path) const;
  QGraphicsLineItem* addAdjacentNode(Node* node);

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  std::string getName() const {
    return getText();
  }

  void addFile(const std::string&);
  bool hasFile(const std::string& str) const;
  std::map<std::string, size_t> getFiles() const {
    return m_files;
  }

  void addLeaf(LeafNode* node) {
    m_leafs.push_back(node);
  }

  std::vector<LeafNode*> getLeafs() const {
    return m_leafs;
  }

  int calculateStrength(const Contributer& contrib) const;

private:
  std::map<std::string, size_t> m_files;
  std::vector<LeafNode*> m_leafs;
};

#endif // CONTRIBUTER_H
