#ifndef TREENODE_H
#define TREENODE_H
#include "node.h"

#include "contributer.h"
#include <set>
#include "macros.h"

class LeafNode;

/**
 * @brief The TreeNode class representing an arrangeable (sub)-Tree with leafs
 *
 * This is used to represent a folder in a filestructure
 */
class TreeNode : public Node
{
public:
  /**
   * @brief Creates a TreeNode
   * @param scene scene to draw on
   * @param content text to display
   * @param rect bounding rect
   */
  explicit TreeNode(ZoomGraphicsScene* scene,
                    const std::string& content,
                    const QRectF& rect = QRectF(0, 0, 100, 30))
    : Node(scene, content, rect){
  }

  /**
   * @brief rearranges subtree
   * @param event
   */
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

  /**
   * @brief Drags leafs with the node
   * @param change
   * @param value
   * @return
   */
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

  /**
   * @brief adds a leaf to this node
   * @param node node to add
   */
  void addChildNode(LeafNode* node);
  /**
   * @brief adds a node to this node displaying a hierarchy below it
   * @param node node to add
   */
  void addChildNode(TreeNode* node);
  /**
   * @brief We represent a tree. Adjacency is for graphs
   * @param node
   */
  void addAdjacentNode(Node* node) = delete;

  /**
   * @brief Returns all nodes in a direct hierarchy below this node
   * @return  set of nodes
   */
  std::set<Node*> getChildren() const;
  /**
   * @brief Returns all leafs on this node
   * @return set of leafs
   */
  std::set<LeafNode*> getLeafs() const;

  // This is here, because it fails to link, because <true> isn't generated
  /**
   * @brief arranges this subtree
   *
   * templates root to improve performance so we can avoid the branch
   */
  template <bool isRoot = false>
  void arrange() {
    constexpr float TAU = 2 * 3.1415926;
    auto count = m_nodes.size();
    const auto degree = (360.0f / 8) / count;

    const QPointF pos = this->pos();
    const auto circledegree = TAU / count;

    // Changes files around it
    itemChange(ItemPositionChange, pos);

    int i = 0;
    for (auto& node : m_nodes) {
      if (isRoot) { // in a circle
        const qreal ypos = pos.y() + 600*cos(i * circledegree);
        const qreal xpos = pos.x() + 600*sin(i * circledegree);

        static_cast<TreeNode*>(node)->setPos(xpos, ypos);
        ++i;
      } else { // in a line and spread
        float factor = i % 2 ? 1 : -1;

        auto treenode = static_cast<TreeNode*>(node);
        float stretch =
            std::min(
              std::max(
                (float)treenode->getChildren().size() / 10 + (float)treenode->getLeafs().size()/ 40,
                1.5f),
              2.5f);
        int x = stretch*pos.x();
        int y = stretch*pos.y();
        QPointF newPos = QPointF(x, y) * QTransform().rotate(factor * degree * i);
        node->setPos(newPos);
        ++i;
      }
    }

    if (m_nodes.size() > 0) {
      for (auto& node : m_nodes) {
        static_cast<TreeNode*>(node)->arrange();
      }
    }

  }

  /**
   * @brief sets the parent of this node
   * @param parent parent node
   */
  void setParent(TreeNode* parent);

  /**
   * @brief adds a Contributer to this node
   *
   * This means the contributer affected this file
   * @param contributer Contributer to add
   */
  void addContributer(Contributer* contributer) {
    m_contributers.insert(contributer);
  }

  /**
   * @brief Returns all contributers affecting this file
   * @return  set of contributers
   */
  std::set<Contributer*> getContributers() const {
    return m_contributers;
  }

private:
  /**
   * @brief Parent node
   */
  TreeNode* m_parent;
  /**
   * @brief contributers affecting this folder
   */
  std::set<Contributer*> m_contributers;
  /**
   * @brief file in this folder
   */
  std::set<LeafNode*> m_leafs;

};

#endif // TREENODE_H
