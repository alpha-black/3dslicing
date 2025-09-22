#include "errors.h"
#include "geometry.h"

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

void
FacetList::get_min_xyz (float *x, float *y, float *z)
{
    *x = min_x;
    *y = min_y;
    *z = min_z;
}

void
FacetList::get_max_xyz (float *x, float *y, float *z)
{
    *x = max_x;
    *y = max_x;
    *z = max_z;
}

bool
FacetList::vertices_in_range() {
    if (max_x < 1.0 && min_x > -1.0 &&
            max_y < 1.0 && min_y > -1.0 &&
            max_z < 1.0 && min_z > -1.0) {
        return true;
    } else {
        return false;
    }
}

uint8_t
FacetList::normalize_vertices()
{
    float scale_x = max_x - min_x;
    float scale_y = max_y - min_y;
    float scale_z = max_z - min_z;
    float scale = std::max(scale_x, scale_y);
    scale = std::max(scale, scale_z);

    /* Use omp pararllel */
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i].position.x = (vertices[i].position.x - min_x)/scale;
        vertices[i].position.y = (vertices[i].position.y - min_y)/scale;
        vertices[i].position.z = (vertices[i].position.z - min_z)/scale;
        vertices[i].normal.normalize();
    }

    /* Set min and max again. Its going to be 0 to 1 */
    max_x = (max_x - min_x)/scale;
    max_y = (max_y - min_y)/scale;
    max_z = (max_z - min_z)/scale;
    min_x = min_y = min_z = 0.0f;

    return SUCCESS;
}

void
FacetList::find_max_area_normal(Vertex c1, Vertex c2, Vertex c3)
{
    /* Area = 1/2 * (A * B); A and B are
    2 sides of the triangle. Not dividing by 2 */
    v3 A = c1.position.subtract(c2.position);
    v3 B = c1.position.subtract(c3.position);
    v3 A_B = v3::cross_product(A, B);
    float area = A_B.get_mod();
    //static int count = -1;
    count++;
    if (area > max_area) {
        max_area = area;
        max_area_normal.set(c1.normal);
        max_area_facet_index = count;
    }
    return;
}

void
FacetList::add_vertex(Vertex f)
{
    vertices.push_back(f);

    if (f.position.x < min_x) {
        min_x = f.position.x;
    } else if (f.position.x > max_x) {
        max_x = f.position.x;
    }
    if (f.position.y < min_y) {
        min_y = f.position.y;
    } else if (f.position.y > max_y) {
        max_y = f.position.y;
    }
    if (f.position.z < min_z) {
        min_z = f.position.z;
    } else if (f.position.z > max_z) {
        max_z = f.position.z;
    }
}

void
FacetList::dump_vertices () {
    std::cout << "Dumping facets" << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
        //qDebug() << "N - " << vertices[i].normal.x << vertices[i].normal.y << vertices[i].normal.z;
        qDebug() << "V - "; print_v3(vertices[i].position);
    }
}
