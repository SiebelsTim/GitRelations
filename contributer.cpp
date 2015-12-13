#include "contributer.h"
#include "treenode.h"
#include "leafnode.h"
#include "macros.h"
#include "paintwindow.h"
#include "tooltip.h"


TreeNode* Contributer::findNodeByPath(const TreeNode* root, const std::string& path) const {
  auto firstslash = path.find_first_of('/');
  if (firstslash != std::string::npos) { // Found
    std::string firstfolder = path.substr(0, firstslash);
    std::set<Node*> children = root->getChildren();
    for (const auto& node : children) {
      TreeNode* tnode = static_cast<TreeNode*>(node);
      if (tnode->getText() == firstfolder) {
        std::string strippedPath = path.substr(firstslash + 1);
        return findNodeByPath(tnode, strippedPath);
      }
    }
  } else { // Not found == root
    std::set<LeafNode*> children = root->getLeafs();
    for (const auto& node : children) {
      if (node->getText() == path) {
        return node;
      }
    }
  }
  Q_ASSERT_X(false, root->getText().c_str(), path.c_str());
  return nullptr;
}

void Contributer::addFile(const std::string& path) {
  if (m_files.find(path) != m_files.end()) { // found
    ++m_files[path];
  } else {
    m_files[path] = 1;
  }
}

bool Contributer::hasFile(const std::string& file) const {
  return m_files.count(file);
}

QGraphicsLineItem* Contributer::addAdjacentNode(Contributer* node) {
  auto ret = Node::addAdjacentNode(node);
  if (ret == nullptr) return nullptr;
  node->addNode(this);

  node->addContributer(this);
  addContributer(node);

  return ret;
}

int Contributer::calculateStrength(const Contributer& contrib) const {
  int strength = 0;
  for (const std::pair<std::string, size_t>& file : m_files) {
    if (contrib.hasFile(file.first)) {
      strength += std::min(contrib.getFiles()[file.first], getFiles()[file.first]);
    }
  }

  return strength;
}

void Contributer::addContributer(Contributer* contrib) {
  m_contributers.insert(contrib);
}

bool Contributer::containsContributer(Contributer* contrib) const {
  return m_contributers.count(contrib);
}


void Contributer::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  QColor red = QColor::fromRgb(255, 0, 0);
  for (auto& leaf : m_leafs) {
    QPen pen = leaf->pen();
    pen.setColor(red);
    leaf->setPen(pen);
  }

  // Ugly.. QGraphicsItems are no qobjects
  PaintWindow* paintwindow = static_cast<PaintWindow*>(m_scene->parent());
  paintwindow->getTooltip()->setContributer(this);

  Node::mousePressEvent(event);
}

void Contributer::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  QColor black = QColor::fromRgb(0, 0, 0);
  for (auto& leaf : m_leafs) {
    QPen pen = leaf->pen();
    pen.setColor(black);
    leaf->setPen(pen);
  }


  Node::mouseReleaseEvent(event);
}
