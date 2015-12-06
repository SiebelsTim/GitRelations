#ifndef CONTRIBUTER_H
#define CONTRIBUTER_H

#include "node.h"
#include <QListWidgetItem>

class TreeNode;

class Contributer : public Node, public QListWidgetItem
{
public:

  explicit Contributer(QGraphicsScene* scene, const std::string& name): Node(scene, name), QListWidgetItem(name.c_str()) {}

  TreeNode* findNodeByPath(const TreeNode* root, const std::string& path) const;

  std::string getName() const {
    return getText();
  }

  void addFile(const std::string&);

  std::map<std::string, size_t> getFiles() const {
    return m_files;
  }

private:
  std::map<std::string, size_t> m_files;
};

#endif // CONTRIBUTER_H
