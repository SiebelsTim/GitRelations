#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QGraphicsRectItem>
#include "contributer.h"

static constexpr size_t maxFiles = 20;

/**
 * @brief The Tooltip class representing a tooltip inside a graphicsscene for a contributer
 * @deprecated Not used anymore \n Replaced by ContributerWindow
 */
class Tooltip : public QGraphicsRectItem
{
public:
  explicit Tooltip(Contributer* contributer = nullptr):
    QGraphicsRectItem(), m_contributer(contributer) {
    setFlag(GraphicsItemFlag::ItemIsMovable);
    setRect(0, 0, 350, 0);
  }

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  void setContributer(Contributer* contrib) {
    m_contributer = contrib;
    auto rect = this->rect();
    auto files = m_contributer->getFiles();
    int height = (std::min(files.size(), maxFiles) + 2) * 20;
    setRect(rect.x(), rect.y(), rect.width(), height);

    update();
  }

  Contributer* getContributer() const {
    return m_contributer;
  }

private:
  Contributer* m_contributer;

};

#endif // TOOLTIP_H
