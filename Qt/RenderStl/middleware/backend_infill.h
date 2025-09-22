#ifndef __BACKEND_INFILL_H__
#define __BACKEND_INFILL_H__

#include "geometry.h"

#define BACKEND_INFILL_DEPTH            0.05f

struct backend_filler  {
    uint8_t filler_process_lines();
    void fill_loop(std::vector<v3>& loop);
    const v3 find_loop_centre(std::vector<v3>& loop);

    std::vector<std::vector<v3> > infill;
};

#endif /* __BACKEND_INFILL_H__ */
