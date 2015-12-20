#ifndef ZOOMGRAPHICSSCENE_H
#define ZOOMGRAPHICSSCENE_H

#include <QGraphicsScene>

class ZoomGraphicsScene : public QGraphicsScene
{
  Q_OBJECT
  using QGraphicsScene::QGraphicsScene;
public:
  void setZoomFactor(const double factor) {
    m_zoom = factor;
  }

  double getZoomFactor() const {
    return m_zoom;
  }

private:
  double m_zoom;

};

#endif // ZOOMGRAPHICSSCENE_H
