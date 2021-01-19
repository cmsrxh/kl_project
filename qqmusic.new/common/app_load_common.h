#ifndef APP_LOAD_COMMON_H
#define APP_LOAD_COMMON_H

#include "common_func/common_func.h"
#include "util/list_table.h"
#include "util/vector_table.h"

#define QStringFromByteString(str) \
    QString::fromUtf8((str).string(), (str).size())

#define QStringFromCString(str) \
    QString::fromUtf8(str)


#endif // APP_LOAD_COMMON_H
