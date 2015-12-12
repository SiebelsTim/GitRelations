#ifndef FORCELAYOUT_H
#define FORCELAYOUT_H

#include <set>
#include <string>

#include <QObject>

class Contributer;

class ForceLayout : public QObject
{
  Q_OBJECT
public:
  ForceLayout(const std::set<Contributer*>&);

  template<typename Function>
  void forEach(Function f);

  void exportToFile(const std::string& filename) const;

  void layout(const char* algo);

private:
  std::set<Contributer*> m_nodes;

signals:
  void posChanged(Contributer*, int x, int y);
};

#endif // FORCELAYOUT_H
