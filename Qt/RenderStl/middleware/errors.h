#ifndef ERRORS_H
#define ERRORS_H


#include <QDebug>
#include "geometry.h"

#define SUCCESS                 0
#define ERROR_DEFAULT           10
#define ERROR_NO_FILENAME       11
#define ERROR_FILE_NOT_OPEN     12 /* Not correct permissions (r/w) */
#define ERROR_NULL_PTR          13
#define ERROR_NO_SHADERS        14
#define ERROR_FILE_READ         15
#define ERROR_FACETS_EMPTY      16
#define ERROR_FACETS_NOT_READ   17
#define ERROR_NO_INTERSECTION   18
#define ERROR_TRI_ON_PLANE      19
#define ERROR_TRI_SLICING_DONE  20


#define BACKEND_MAX(v,x,y)                                  \
{                                                           \
    v=std::max(x,y);                                        \
}

#define BACKEND_MIN(v,x,y)                                  \
{                                                           \
    v=std::min(x,y);                                        \
}

#define GUI_MESSAGE(s)                                      \
{                                                           \
    if (dataHandler && dataHandler->gui_message) {          \
        dataHandler->gui_message->set_gui_message(s);       \
    }                                                       \
}

static inline void
print_v3(v3 v) {
    qDebug() << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

static inline void
print_v3_vector(std::vector<v3>& list) {
    for (auto i = 0u; i < list.size(); ++i) {
        print_v3(list[i]);
    }
}

#endif // ERRORS_H
