#ifndef LEAFNODE_H
#define LEAFNODE_H

#include "treenode.h"

class QPainter;

class LeafNode : public TreeNode
{
public:
  explicit LeafNode(ZoomGraphicsScene* scene,
                    const std::string& content,
                    const QRectF& rect = QRectF(0, 0, 30, 30))
    : TreeNode(scene, content, rect){

    setAcceptHoverEvents(true);

    m_text->setVisible(false);
  }


  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;


  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
};

#endif // LEAFNODE_H
