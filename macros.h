#ifndef MACROS_H
#define MACROS_H

#include <ctime>

class TreeNode;

/**
 * Branchless sign calculation
 * @param val value to find sign for
 * @return 1 if positive, -1 if negative, 0 if zero
 */
template <typename T>
inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

#define MEASURE(x)  time_t start=std::clock(); [&]{x}(); qDebug() << QString::number(std::clock() - start) << " elapsed\n";

#define LEAF_SIZE 30 // px

/**
 * @brief root file node
 */
extern TreeNode* g_root;

#endif // MACROS_H
