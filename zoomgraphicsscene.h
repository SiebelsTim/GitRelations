#ifndef ZOOMGRAPHICSSCENE_H
#define ZOOMGRAPHICSSCENE_H

#include <QGraphicsScene>

/**
 * @brief Wrapper around QGraphicsScene to save current zoom factor for
 *
 * Using the scene instead of view as we already pass this around often.
 */
class ZoomGraphicsScene : public QGraphicsScene
{
  Q_OBJECT
  using QGraphicsScene::QGraphicsScene;
public:
  /**
   * @brief sets current zoom factor
   * @param factor zoom factor
   */
  void setZoomFactor(const double factor) {
    m_zoom = factor;
  }

  /**
   * @brief fetches current zoom factor
   * @return
   */
  double getZoomFactor() const {
    return m_zoom;
  }

private:
  double m_zoom = 1.0;

};

#endif // ZOOMGRAPHICSSCENE_H
