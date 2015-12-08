#ifndef TREENODE_H
#define TREENODE_H
#include "node.h"

#include "contributer.h"
#include <set>
#include "macros.h"

class TreeNode : public Node
{
public:
  explicit TreeNode(QGraphicsScene* scene,
                const std::string& content,
                const bool isLeaf = false,
                const QRectF& rect = QRectF(0, 0, 100, 30))
    : Node(scene, content, isLeaf, rect){

    setAcceptHoverEvents(true);
  }

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;

  void addChildNode(TreeNode* node);
  void addAdjacentNode(Node* node) = delete;

  std::set<Node*> getChildren() const;

  // This is here, because it fails to link, because <true> isn't generated
  template <bool isRoot = false>
  void arrange() {
    constexpr float TAU = 2 * 3.1415926;
    const auto width = this->rect().width();
    const auto height = this->rect().height();

    int i = 0;
    int count = 0;
    for (auto& node : m_nodes) {
      if (!node->isLeaf()) ++count;
    }
    int leafcount = 0;
    int distance = width/2; // init to LEAF_SIZE in first loop
    const auto degree = (360.0f / 4) / count;

    auto leafs_per_circle = 2;
    auto leafdegree = TAU / leafs_per_circle;

    const QPointF pos = this->pos();
    const auto circledegree = TAU / count;
    for (auto& node : m_nodes) {
      if (node->isLeaf()) {
        if (UNLIKELY(leafcount == leafs_per_circle)) {
          distance += LEAF_SIZE;
          leafs_per_circle = leafs_per_circle + 5;
          leafdegree = TAU / leafs_per_circle;
          leafcount = 0;
        }
        const auto sinus = sin(leafdegree * leafcount);
        const auto cosinus = cos(leafdegree * leafcount);
        qreal x = pos.x() + width/2 - 15 + distance * sinus;
        qreal y = pos.y() + height/2 - 15 + distance * cosinus;
        node->setPos(x, y);
        ++leafcount;
      } else {
        if (isRoot) { // in a circle
          const qreal ypos = pos.y() + 600*cos(i * circledegree);
          const qreal xpos = pos.x() + 600*sin(i * circledegree);

          static_cast<TreeNode*>(node)->setPos(xpos, ypos);
          ++i;
        } else { // in a line and spread
          double factor = i % 2 ? 1 : -1;
          int x = 2*pos.x();
          int y = 2*pos.y();
          QPointF newPos = QPointF(x, y) * QTransform().rotate(factor * degree * i);
          node->setPos(newPos);
          ++i;
        }
      }
    }

    if (m_nodes.size() > 0) {
      for (auto& node : m_nodes) {
        static_cast<TreeNode*>(node)->arrange();
      }
    }

  }


  void setParent(TreeNode* parent);

  void addContributer(Contributer* contributer) {
    m_contributers.insert(contributer);
  }

  std::set<Contributer*> getContributers() const {
    return m_contributers;
  }
private:
  TreeNode* m_parent;
  std::set<Contributer*> m_contributers;

signals:

public slots:

};

#endif // TREENODE_H
