#ifndef LEAFNODE_H
#define LEAFNODE_H

#include "treenode.h"

class QPainter;

/**
 * @brief The LeafNode class represents a Leaf node on a Tree
 *
 * Used for Files in a folder
 * The name is only displayed on hover
 */
class LeafNode : public TreeNode
{
public:
  /**
   * @brief Creates a Leaf node
   * @param scene scene to draw on
   * @param content string to draw inside
   * @param rect
   */
  explicit LeafNode(ZoomGraphicsScene* scene,
                    const std::string& content,
                    const QRectF& rect = QRectF(0, 0, 30, 30))
    : TreeNode(scene, content, rect){

    setAcceptHoverEvents(true);

    m_text->setVisible(false);
  }

  bool isVisible() const;

protected:

  /**
   * @brief Displays the content string
   * @param event
   */
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
  /**
   * @brief Hides the content string
   * @param event
   */
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;


  /**
   * @brief paints the leafnode
   * @param painter
   * @param option
   * @param widget
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
};

#endif // LEAFNODE_H
