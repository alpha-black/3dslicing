#ifndef __BACKEND_MAIN_H__
#define __BACKEND_MAIN_H__

#include "slicer.h"
#include "backend_infill.h"

#define BACKEND_INFILL_CENTRE           0.5f
#define BACKEND_LOOP_FORM_DIST          1.5f

#define BACKEND_GENERATE_SUPPORT        0
#define BACKEND_SLICE                   1
#define BACKEND_GENERATE_GCODE          2

struct backend_main {
public:
    backend_main() : sPlane(NULL), init_done(false), slicing_done(false) {}
    ~backend_main() { backend_cleanup(); }

    void backend_init();
    void backend_cleanup();
    void reset_slicer();
    uint8_t backend_place_obj_on_bed(FacetList *facets);
    void backend_remove_duplicate();
    void backend_process_lines_on_a_plane(std::vector<v3>& lines);
    uint8_t backend_add_infill();
    uint8_t backend_find_overhangs();
    uint8_t backend_generate_gcode();
    uint8_t backend_slice();
    uint8_t backend_receive_request(uint8_t request);
    void *get_lines_on_plane_addr(unsigned i);
    unsigned get_number_of_lines_on_plane(unsigned i);
    unsigned get_slicing_plane_count();
    void *get_loop_infill_addr(unsigned i);
    unsigned get_loop_infill_count(unsigned i);
    unsigned get_infill_count();

    /* Pointers and backpointers */
    slicingPlane *sPlane;

    backend_filler filler;
    /* Flags */
    bool init_done;
    bool slicing_done;
    /* After slicing, store the lines here. Every std::vetor<v3> is a loop
     * if there are no loops, they are just lines. */
    std::vector<std::vector<v3> > lines;
};


struct lines_compare {
    bool operator() (std::pair<v3,v3>p1, std::pair<v3,v3>p2) const {
        float mod1 = std::get<0>(p1).get_mod() + std::get<1>(p1).get_mod();
        float mod2 = std::get<0>(p2).get_mod() + std::get<1>(p2).get_mod();
        return (mod1 < mod2);
    }
};

struct v3_compare {
    bool operator() (const v3 c1, const v3 c2) const {
        if (c1.x != c2.x)
            return (c1.x > c2.x);
        else if (c2.y != c2.z)
            return (c1.y > c2.y);
        else
            return (c1.z > c2.z);
    }
};

#endif /* __BACKEND_MAIN_H__ */
