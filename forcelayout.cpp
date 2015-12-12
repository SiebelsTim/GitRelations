#include "forcelayout.h"

#include "contributer.h"
#include <fstream>

#include "graphviz/cgraph.h"
#include "graphviz/gvc.h"

#include <QDebug>

ForceLayout::ForceLayout(const std::set<Contributer*>& nodes):
  m_nodes(nodes)
{
}

template<typename Function>
void ForceLayout::forEach(Function f) {
  for (Contributer* node : m_nodes) {
    f(node);
  }
}

void ForceLayout::exportToFile(const std::string& filename) const {
  std::ofstream stream;
  stream.open(filename);
  if (!stream.is_open()) qCritical() << "Could not open stream";

  stream << "graph G {\n";

  std::set<Contributer*> visited;
  for(Contributer* contrib : m_nodes) {
    visited.insert(contrib);
    for (Contributer* contrib2 : contrib->getContributers()) {
      if (visited.count(contrib2)) continue;
      stream << '"' << contrib->getText() << "\" -- \"" << contrib2->getText() << "\"\n";
    }
  }

  stream << "}\n";

  stream.close();
}


// Holy shit libgraphviz is bad. char* instead of const char
// you need to gvRender() before positions are available
// gvLayout() isn't enough. Fuck you.
void ForceLayout::layout() {
  Agraph_t* G;
  GVC_t* gvc = gvContext();

  FILE* fp = fopen("dot.dot", "r");
  G = agread(fp, 0);

  gvLayout(gvc, G, "dot");
  gvRender(gvc, G, "dot", nullptr);

  gvRenderFilename(gvc, G, "png", "graph.png");

  Agnode_t* node;
  for (Contributer* contrib : m_nodes) {
    const char* name = contrib->getText().c_str();
    node = agnode(G, const_cast<char*>(name), 0);
    Q_ASSERT(node != nullptr);
    std::string posstr = agget(node, const_cast<char*>("pos"));
    auto commapos = posstr.find_first_of(',');
    int x = atoi(posstr.substr(0, commapos).c_str());
    int y = atoi(posstr.substr(commapos + 1).c_str());
    emit posChanged(contrib, x, y);
  }


  gvFreeLayout(gvc, G);
  agclose(G);

}
