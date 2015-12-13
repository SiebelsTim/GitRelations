#include "tooltip.h"
#include "contributer.h"

#include <QPainter>
#include <QPoint>

void Tooltip::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /* = 0 */) {
  if (m_contributer == nullptr) return;

  painter->fillRect(rect(), QBrush(QColor::fromRgb(100, 100, 100)));

  painter->drawText(QPoint(2,20), m_contributer->getName().c_str());
  painter->drawLine(2, 25, rect().width(), 25);

  size_t i = 1;
  for (auto& file : m_contributer->getFiles()) {
    std::string text = file.first;
    if (text.size() > 40) {
      text = std::string("...") + text.substr(text.size() - 40);
    }
    painter->drawText(QPoint(2, i*20+20), text.c_str());
    if (i >= maxFiles) {
      break;
    }
    ++i;
  }


  QGraphicsRectItem::paint(painter, option, widget);
}
