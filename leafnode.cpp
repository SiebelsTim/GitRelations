#include "leafnode.h"

#include <QPainter>

void LeafNode::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {
  painter->setPen(pen());
  painter->drawEllipse(rect());
}


void LeafNode::hoverEnterEvent(QGraphicsSceneHoverEvent*) {
  m_text->setVisible(true);
}

void LeafNode::hoverLeaveEvent(QGraphicsSceneHoverEvent*) {
  m_text->setVisible(false);
}
