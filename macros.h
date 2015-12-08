#ifndef MACROS_H
#define MACROS_H

#include <ctime>

template <typename T>
inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

#define MEASURE(x)  time_t start=std::clock(); [&]{x}(); qDebug() << QString::number(std::clock() - start) << " elapsed\n";

#define LEAF_SIZE 30

#endif // MACROS_H
