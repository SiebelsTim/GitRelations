#ifndef MACROS_H
#define MACROS_H

#include <ctime>

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

#define MEASURE(x)  time_t start=std::clock(); [&]{x}(); qDebug() << QString::number(std::clock() - start) << " elapsed\n";

#endif // MACROS_H
