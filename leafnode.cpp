#include "leafnode.h"

#include "zoomgraphicsscene.h"

#include <QPainter>
#include <QGraphicsView>

void LeafNode::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {
  if (getScene()->getZoomFactor() < 0.835382868) return;

  painter->setPen(pen());
  painter->drawEllipse(rect());
}


void LeafNode::hoverEnterEvent(QGraphicsSceneHoverEvent*) {
  m_text->setVisible(true);
}

void LeafNode::hoverLeaveEvent(QGraphicsSceneHoverEvent*) {
  m_text->setVisible(false);
}
