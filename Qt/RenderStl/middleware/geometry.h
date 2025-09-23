#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cstring>
#include <cmath>
#include <vector>
#include <QDebug>

struct v3 {
    float x;
    float y;
    float z;

    v3 (float xx = 0, float yy = 0, float zz = 0) :
        x(xx), y(yy), z(zz) {}

    v3 (const v3& c) { x = c.x; y = c.y; z = c.z; }

    void set (const v3 c) {
        x = c.x; y = c.y; z = c.z;
    }

    void set (float xx, float yy, float zz) {
        x = xx; y = yy; z = zz;
    }

    float dot_product (const v3 c) {
        return (c.x * x + c.y * y + c.z * z);
    }

    static float dot_product (const v3 c1, const v3 c2) {
        return (c1.x * c2.x + c1.y * c2.y + c1.z * c2.z);
    }

    static v3 cross_product (const v3 u, const v3 v) {
        double x, y, z;
        x = u.y*v.z - u.z*v.y;
        y = u.z*v.x - u.x*v.z;
        z = u.x*v.y - u.y*v.x;
        return v3((float)x, (float)y, (float)z);
    }

    v3 subtract (const v3 c) {
        v3 d;
        d.x = x - c.x; d.y = y - c.y; d.z = z - c.z;
        return d;
    }

    static v3 subtract (const v3 c1, const v3 c2) {
      return v3(c1.x - c2.x, c1.y - c2.y, c1.z - c2.z);
    }

    void add (const v3 c) {
        x = x + c.x; y = y + c.y; z = z + c.z;
    }

    static v3 add (const v3 v1, const v3 v2) {
        return v3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    static bool equal(const v3 v1, const v3 v2) {
      return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
    }

    void scale(const float c) {
        x *= c; y *= c; z *=c;
    }

    static v3 scale(v3 v, const float c) {
      return v3(v.x * c, v.y * c, v.z *c);
    }

    void normalize() {
        double factor = std::sqrt(x*x+y*y+z*z);
        x = (float)(x/factor); y = (float)(y/factor);
        z = (float)(z/factor);
    }

    float get_mod() {
        return (std::sqrt(x*x+y*y+z*z));
    }

    static std::pair<v3, v3> make_pair(v3 c1, v3 c2) {
        if (c1.x > c2.x)
            return std::make_pair(c1, c2);
        else if (c1.x < c2.x)
            return std::make_pair(c2, c1);

        if (c1.y > c2.y)
            return std::make_pair(c1, c2);
        else if (c1.y < c2.y)
            return std::make_pair(c2, c1);

        if (c1.z > c2.z)
            return std::make_pair(c1, c2);
        else
            return std::make_pair(c2, c1);
    }

    static float distance(const v3 c1, const v3 c2) {
        float diff_x = c1.x - c2.x;
        float diff_y = c1.y - c2.y;
        float diff_z = c1.z - c2.z;
        return (std::sqrt((diff_x * diff_x) + (diff_y * diff_y) + \
            (diff_z * diff_z)));
    }

};

struct v4 {
    float x;
    float y;
    float z;
    float w;

    v4 (float xx = 0.0f, float yy = 0.0f, float zz = 0.0f, float ww = 0.0f) :
        x(xx), y(yy), z(zz), w(ww) {}

    v4 (v3 v) {
        x = v.x; y = v.y; z = v.z; w = 1;
    }

    v3 get_vec3() {
        return v3(x, y, z);
    }

    void normalize() {
        double factor = std::sqrt(x*x+y*y+z*z+w*w);
        x = (float)(x/factor); y = (float)(y/factor);
        z = (float)(z/factor); w = (float)(w/factor);
    }
};


struct mat4 {
    float m[4][4];

    mat4 () {
        std::memset((void *)&m[0][0], 0, sizeof(float) * 16);
    }

    mat4 (float *data) {
        std::memcpy((void *)&m[0][0], (void *)data, sizeof(float) * 16);
    }

    v4 mat_mul_v4(v4 v) {
        v4 ret;
        ret.x  = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
        ret.y  = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
        ret.z  = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
        ret.w  = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
        return ret;
    }
};

struct Vertex {
    v3 position, normal;
};

class FacetList {
private:
    std::vector<Vertex> vertices;
    float min_x, max_x, min_y, max_y, min_z, max_z;
    float max_area;
    v3 max_area_normal;
    unsigned int max_area_facet_index; int count;
public:
    FacetList () : min_x(99.99f), max_x(-99.99f), min_y(99.99f), max_y(-99.99f),
        min_z(99.99f), max_z(-99.99f), max_area(0.0f), max_area_facet_index(-1),
        count(-1) {
            max_area_normal.set(0.0f, 0.0f, 0.0f);
        }

    ~FacetList() {}

    uint8_t normalize_vertices();
    void get_min_xyz (float *x, float *y, float *z);
    void get_max_xyz (float *x, float *y, float *z);
    float get_max_area() { return max_area; }
    v3 get_max_area_normal() { return max_area_normal; }
    void set_max_area_normal(const v3 c) { max_area_normal.set(c); }
    void normalize_max_area_normal() { max_area_normal.normalize(); }
    unsigned int get_max_area_facet_index() { return max_area_facet_index*3; }
    void find_max_area_normal(Vertex c1, Vertex c2, Vertex c3);
    bool vertices_in_range();
    void reserve_list_space (int n) { vertices.reserve (n); }
    void clear_list() { vertices.clear(); }
    void shrink_list() { vertices.shrink_to_fit(); }
    int get_vertices_size () { return vertices.size(); }
    void *get_vertices_addr() { return (void *)vertices.data(); }
    void add_vertex(Vertex v);
    void dump_vertices ();
    void erase_facet_by_index(unsigned int i) { vertices.erase(vertices.begin() + i); }
    Vertex get_vertex_from_index(unsigned int index) { return vertices[index]; }
    std::vector<Vertex>& get_vertices() { return vertices; }
};

#endif /* __GEOMETRY_H__ */
