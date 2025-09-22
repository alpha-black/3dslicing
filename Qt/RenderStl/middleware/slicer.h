#ifndef __GENERATE_SUPPORT_H__
#define __GENERATE_SUPPORT_H__

#include "geometry.h"
#include <QVector3D>

//#define SLICING_DEPTH       0.015f
#define SLICING_DEPTH           0.1f

class slicingPlane {
private:
    /* Slicing plane is the transformed y axis and distance
     * from the origin. Its actually the normal vector to the plane. */
    v3 y_axis;
    float distance;
public:
    slicingPlane();
    void set_slicing_plane() {
        y_axis.set(0.0f, 1.0f, 0.0f);
    }
    void reset_slicing_plane() {
        distance = 0.0f; set_slicing_plane();
    }
    void set_slicing_distance(float d) { distance = d; }
    void transform_slicing_plane();
    void set_distance (float d) { distance = d; }
    float get_distance () { return distance; }
    v3 intersect_line (v3 c1, v3 c2);
    uint8_t intersect_facet(v3 vert1, v3 vert2, v3 vert3,
                            v3 *line_p1, v3 *line_p2);
};

#endif /* __GENERATE_SUPPORT_H__ */
