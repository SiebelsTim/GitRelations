#ifndef CONTRIBUTER_H
#define CONTRIBUTER_H

#include "node.h"
#include <QListWidgetItem>

class TreeNode;
class LeafNode;

/**
 * @brief The Contributer class representing a drawable node
 */
class Contributer : public Node, public QListWidgetItem
{
public:

  /**
   * @brief Creates a contributer
   * @param scene scene to draw on
   * @param name name of the contributer
   */
  explicit Contributer(ZoomGraphicsScene* scene, const std::string& name): Node(scene, name), QListWidgetItem(name.c_str()) {}

  /**
   * @brief Finds a node by path
   * @param root rootnode
   * @param path path from root
   * @return TreeNode or nullptr if not found
   */
  TreeNode* findNodeByPath(const TreeNode* root, const std::string& path) const;
  /**
   * @brief Specialcased to Contributer*
   */
  QGraphicsLineItem* addAdjacentNode(Node* node) = delete;
  /**
   * @brief adds a relation to an other contributer
   * @param node contributer
   * @return Line it generated
   */
  QGraphicsLineItem* addAdjacentNode(Contributer* node);

  /**
   * @brief adds a contributer to the relation set
   * @param contrib Contributer to add
   */
  void addContributer(Contributer* contrib);
  /**
   * @brief checks if given contributer is in relation set
   * @param contrib contributer to check on
   * @return true if found
   */
  bool containsContributer(Contributer* contrib) const;

  // TODO: check if this is working via Node class
  virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  /**
   * @brief Returns the name of the contributer
   *
   * Alias to getText()
   * @return  name
   */
  std::string getName() const {
    return getText();
  }

  /**
   * @brief adds a file to affected files set
   *
   * @param str path from git root
   */
  void addFile(const std::string& str);
  /**
   * @brief checks if filepath is affected by this contributer
   * @param str filepath
   * @return true if affected
   */
  bool hasFile(const std::string& str) const;
  /**
   * @brief Returns a map of files
   * @return Format: <filepath, number of commits affected>
   */
  std::map<std::string, size_t> getFiles() const {
    return m_files;
  }

  /**
   * @brief adds a file to the contributer from the Filetree
   * @param node File to add
   */
  void addLeaf(LeafNode* node) {
    m_leafs.push_back(node);
  }

  /**
   * @brief Returns all affected file from the filetree
   * @return vector of files
   */
  std::vector<LeafNode*> getLeafs() const {
    return m_leafs;
  }

  /**
   * @brief Returns all contributers this contributer is in relation to
   * @return set of Contributers
   */
  std::set<Contributer*> getContributers() const {
    return m_contributers;
  }

  /**
   * @brief calculates the strength of the relation between two contributers
   * @param contrib Contributer
   * @return strength
   */
  int calculateStrength(const Contributer& contrib) const;

private:
  /**
   * @brief filepaths and commit count
   *
   * This is slightly redudant, but m_files save the complete path whereas m_leafs only contain the current file name
   */
  std::map<std::string, size_t> m_files;
  /**
   * @brief files in filetree
   */
  std::vector<LeafNode*> m_leafs;
  /**
   * @brief adjacent contributers
   */
  std::set<Contributer*> m_contributers;
};

#endif // CONTRIBUTER_H
