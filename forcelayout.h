#ifndef FORCELAYOUT_H
#define FORCELAYOUT_H

#include <set>
#include <string>

#include <QObject>
#include <QPointF>

class Contributer;

/**
 * @brief Layouts a set of contributers
 *
 * TODO: Rename as this is not using a force based approach
 */
class ForceLayout : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief Initializes layouting object
   * @param contribs set of contributers to layout
   */
  ForceLayout(const std::set<Contributer*>& contribs);

  /**
   * @brief exports dot format to filename
   *
   * Prints an error if file could not be opened.
   * @param filename filename
   */
  void exportToFile(const std::string& filename) const;

  /**
   * @brief Layouts the contributers
   *
   * Sends posChanged signal
   * @param algo dot | circo | fdp | neato | nop | nop1 | nop2 | osage | patchwork | sfdp | twopi
   */
  void layout(const char* algo);

protected:
  /**
   * @brief Calls Function f on every contributer
   * @param f function to call
   */
  template<typename Function>
  void forEach(Function f);

private:
  std::set<Contributer*> m_nodes;

signals:
  /**
   * @brief signals that the position of contributer changed to pos
   * @param pos
   * @param contributer
   */
  void posChanged(Contributer* contributer, QPointF pos);
};

#endif // FORCELAYOUT_H
