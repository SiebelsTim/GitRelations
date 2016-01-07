#include <QObject>
#include <QGraphicsView>

/*!
  http://stackoverflow.com/questions/19113532/qgraphicsview-zooming-in-and-out-under-mouse-position-using-mouse-wheel \n
 * This class adds ability to zoom QGraphicsView using mouse wheel. The point under cursor
 * remains motionless while it's possible.
 *
 * Note that it becomes not possible when the scene's
 * size is not large enough comparing to the viewport size. QGraphicsView centers the picture
 * when it's smaller than the view. And QGraphicsView's scrolls boundaries don't allow to
 * put any picture point at any viewport position.
 *
 * When the user starts scrolling, this class remembers original scene position and
 * keeps it until scrolling is completed. It's better than getting original scene position at
 * each scrolling step because that approach leads to position errors due to before-mentioned
 * positioning restrictions.
 *
 * When zommed using scroll, this class emits zoomed() signal.
 *
 * Usage:
 *
 *   ```new Graphics_view_zoom(view);```
 *
 * The object will be deleted automatically when the view is deleted.
 *
 * You can set keyboard modifiers used for zooming using set_modified(). Zooming will be
 * performed only on exact match of modifiers combination. The default modifier is Ctrl.
 *
 * You can change zoom velocity by calling set_zoom_factor_base().
 * Zoom coefficient is calculated as zoom_factor_base^angle_delta
 * (see QWheelEvent::angleDelta).
 * The default zoom factor base is 1.0015.
 */
class GraphicsViewZoom : public QObject {
  Q_OBJECT
public:
  /**
   * @brief Creates the zoom object
   *
   * will be freed when view is destroyed
   * @param view view to zoom
   */
  GraphicsViewZoom(QGraphicsView* view);
  /**
   * @brief scales the QGraphicsView to factor
   * @param factor
   */
  void gentle_zoom(double factor);
  /**
   * @brief Set keyboard modifiers to trigger zoom in combination with mousewheel
   * @param modifiers defaults to ctrl
   */
  void set_modifiers(Qt::KeyboardModifiers modifiers);
  /**
   * @brief sets a base factor to zoom. Higher means faster zoom
   * @param value defaults to 1.0015
   */
  void set_zoom_factor_base(double value);

private:
  /**
   * @brief The view to zoom
   */
  QGraphicsView* m_view;
  /**
   * @brief Keyboard modifiers to trigger zoom
   */
  Qt::KeyboardModifiers m_modifiers;
  /**
   * @brief zoom base factor
   */
  double m_zoom_factor_base;
  /**
   * @brief mouse position changes zoom
   */
  QPointF m_target_scene_pos, m_target_viewport_pos;
  /**
   * @brief triggers zoom
   */
  bool eventFilter(QObject* object, QEvent* event);

signals:
  /**
   * @brief send after zoom is processed
   */
  void zoomed();
};
