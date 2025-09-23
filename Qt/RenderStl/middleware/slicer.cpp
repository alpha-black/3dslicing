#include "slicer.h"
#include "geometry.h"
#include "errors.h"
#include "shared_data_handler.h"
#include <QMatrix4x4>
#include <map>

extern SharedDataHandler *dataHandler;

slicingPlane::slicingPlane() : distance(0.0f)
{
    distance = 0.0f;
    set_slicing_plane();
}

void
slicingPlane::transform_slicing_plane()
{
    if (!dataHandler)
        return;

    mat4 transformation_mat(dataHandler->obj_matrix.data());

    /* Always take the original plane. Not the previously transformed one */
    y_axis.set(transformation_mat.mat_mul_v4(v4(y_axis)).get_vec3());
    y_axis.normalize();
    //qDebug() << "Transformed y axis "; print_v3(y_axis);
}

v3
slicingPlane::intersect_line (v3 c1, v3 c2)
{
    /* Equation of plane: (p - p0).n = 0; p0 is a point on the plane.
     * n is the normal vector. Equation of a line,
     * p = dl + l0; l0 is a point on the line. l is the vector in the
     * direction of the line.
     * Soliving these two equations, d = ((p0 - l0).n)/(l.n); */

    //qDebug() << "Input to intersect_line"; print_v3(c1);
    //print_v3(c2);

    /* A point on the slicing plane */
    v3 p0(0.0f, distance, 0.0f);

    /* Transform this plane */
    mat4 transformation_mat(dataHandler->obj_matrix.data());
    p0.set(transformation_mat.mat_mul_v4(v4(p0)).get_vec3());

    //qDebug() << "Point on the slicing plane";
    //print_v3(p0);

    /* Vector in the direction of the line */
    v3 l = c1.subtract(c2); //l.normalize();

    //qDebug() << "Vector in the direction of line"; print_v3(l);

    v3 temp1 = p0.subtract(c1);
    float numberator = temp1.dot_product(y_axis);
    float denominator = l.dot_product(y_axis);
    float d = numberator/denominator;

    //qDebug() << "d = " << d;

    v3 intersection_p(l);
    intersection_p.scale(d);
    intersection_p.add(c1);

    //qDebug() << "Intersection point"; print_v3(intersection_p);

    return intersection_p;
}

uint8_t
slicingPlane::intersect_facet(v3 vert1, v3 vert2, v3 vert3,
                              v3 *line_p1, v3 *line_p2)
{
    /* Distance to the three vertices */
    float d1 = vert1.dot_product (y_axis) - distance;
    float d2 = vert2.dot_product (y_axis) - distance;
    float d3 = vert3.dot_product (y_axis) - distance;

    //qDebug() << "Distances " << d1 << d2 << d3;

    /* Triangle lies on the slicing plane */
    if (d1 == 0 && d2 == 0 && d3 == 0) {
        //qDebug() << "Triangle lies on the slicing plane";
        return ERROR_TRI_ON_PLANE;
    }

    /* The triangle lies either completely above or below
     * the slicing plane */
    if (d1 < 0 && d2 < 0 && d3 < 0) {
        //qDebug() << "Facet slicing done";
        return ERROR_TRI_SLICING_DONE;
    }

    if (d1 > 0 && d2 > 0 && d3 > 0) {
        //qDebug() << "Triangle is completely above the slicing plane";
        return ERROR_NO_INTERSECTION;
    }

    if (d1 == 0) {
        if (d2 == 0) {
            //qDebug() << "Two points on the plane";
            line_p1->set(vert1); line_p2->set(vert2);
            return SUCCESS;
        } else if (d3 == 0) {
            //qDebug() << "Two points on the plane";
            line_p1->set(vert1); line_p2->set(vert3);
            return SUCCESS;
        } else if (d2 * d3 < 0) {
            //qDebug() << "One vertex on the plane. Other two on either side.";
            line_p1->set(vert1);
            line_p2->set(intersect_line (vert2, vert3));
            return SUCCESS;
        } else {
            //qDebug() << "Only one vertex on the plane. No intersections";
            return ERROR_NO_INTERSECTION;
        }
    }

    if (d2 == 0) {
        if (d3 == 0) {
            //qDebug() << "Two points on the plane";
            line_p1->set(vert2); line_p2->set(vert3);
            return SUCCESS;
        } else if (d1 * d3 < 0) {
            //qDebug() << "One vertex on the plane. Other two on either side.";
            line_p1->set(vert2);
            line_p2->set(intersect_line (vert1, vert3));
            return SUCCESS;
        }
        else {
            //qDebug() << "Only one vertex on the plane. No intersections";
            return ERROR_NO_INTERSECTION;
        }
    }

    if (d3 == 0) {
        if (d1 * d2 < 0) {
            //qDebug() << "One vertex on the plane. Other two on either side.";
            line_p1->set(vert3);
            line_p2->set(intersect_line (vert1, vert2));
            return SUCCESS;
        }
        else {
            //qDebug() << "Only one vertex on the plane. No intersections";
            return ERROR_NO_INTERSECTION;
        }
    }

    //qDebug() << "Proper intersection";
    if (d1 * d2 < 0) {
        line_p1->set(intersect_line (vert1, vert2));
        if (d2 * d3 < 0) {
            line_p2->set(intersect_line (vert2, vert3));
        } else {
            line_p2->set(intersect_line (vert1, vert3));
        }
        return SUCCESS;
    } else {
        line_p1->set(intersect_line (vert1, vert3));
        line_p2->set(intersect_line (vert2, vert3));
        return SUCCESS;
    }
    return SUCCESS;
}
