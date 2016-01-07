#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include <set>

class ZoomGraphicsScene;

/**
 * @brief The Node class representing a drawable node
 *
 * A Node contains a text and is bound by a rectangle.
 * A Node can have neighbors which are connected via a line.
 */
class Node : public QGraphicsRectItem
{
public:
  /**
   * @brief Creates a Node
   * @param scene GraphicsScene to draw on
   *              This is not a QGraphicsScene as we need info wether to display leafs when zoomed out
   * @param content Text to be displayed inside the rectangle
   * @param rect rectangle size
   */
  explicit Node(ZoomGraphicsScene* scene,
                const std::string& content,
                const QRectF& rect = QRectF(0, 0, 100, 30));

  /**
   * @brief adds an adjacent node connected via a line
   * @param node adjacent node
   * @return the created line between both nodes
   */
  QGraphicsLineItem* addAdjacentNode(Node* node);
  /**
   * @brief adds a Node to the adjacency list. Does not add a line
   *
   * This is the same as addAdjacentNode(Node*) but it only adds it to the vector
   * @param node adjacent node
   */
  void addNode(Node* node);
  /**
   * @brief gets the line between this node and a given node
   * @param node node to search for
   * @return line item or nullptr
   */
  QGraphicsLineItem* getLine(Node* node) const;
  /**
   * @brief gets the Scene this node is drawn on
   * @return never null
   */
  ZoomGraphicsScene* getScene() const;

  /**
   * @brief sets visibility of the node
   * @param visible false to hide
   */
  virtual void setVisible(bool visible);

  /**
   * @brief Checks if given node is adjacent to this node
   * @param node node to check for
   * @return true if adjacent
   */
  bool containsChild(Node* node) const {
    return m_nodes.count(node) != 0;
  }

  /**
   * @brief Returns the string inside the node
   * @return string
   */
  std::string getText() const;

protected:
  /**
   * @brief adds a given line between two nodes
   * @param line line to display
   * @param isPoint1 need to be false on one node, and true on the other
   * @param node other node
   */
  void addLine(QGraphicsLineItem *line, bool isPoint1, Node* node);
  /**
   * @brief moves the line and centered text
   * @param change
   * @param value
   * @return
   */
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
  /**
   * @brief Moves the line connecting this node
   * @param newPos new center position
   */
  void moveLines(QPointF newPos);

  /**
   * @brief Highlights all incident edges and affected files
   * @param event
   */
  virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
  /**
   * @brief Removes highlight
   * @param event
   */
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

protected:
  /**
   * @brief
   *
   * Format: <Line, is_p1>
   *
   * is_p1 is needed to connect and move the correct end
   */
  std::map<Node*, std::pair<QGraphicsLineItem*, bool>> m_lines;
  /**
   * @brief Contains colors for m_lines, lazily filled in mousePressEvent \n
   * We need this so we can reset from red when clicking a node
   */
  std::map<QGraphicsLineItem*, QColor> m_line_colors;
  /**
   * @brief Drawable text item
   */
  QGraphicsTextItem* m_text;
  /**
   * @brief Scene to draw on
   */
  ZoomGraphicsScene* m_scene;
  /**
   * @brief Contains adjacent nodes \n
   * ATTENTION: This is not doubly linked due to arrange()
   */
  std::set<Node*> m_nodes;
  /**
   * @brief Contains the text to display inside the rectangle
   */
  std::string m_text_str;

};

#endif // NODE_H
