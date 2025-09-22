#ifndef GL_UTILITIES_H
#define GL_UTILITIES_H

#include <QOpenGLFunctions>
#include "middleware/geometry.h"

#define ROTATION_STEP   1.5708f
#define SCALE_STEP      2.0f//1.1f

enum {
    ROTATE_ON_X_UP = 0,
    ROTATE_ON_X_DOWN,
    ROTATE_ON_Y_UP,
    ROTATE_ON_Y_DOWN,
    ROTATE_ON_Z_UP,
    ROTATE_ON_Z_DOWN,
    SCALE_UP,
    SCALE_DOWN,
};

enum {
    GL_OBJ_BED = 0,
    GL_OBJ_FRAME,
    GL_OBJ_SLICE,
    GL_OBJ_INFILL,
    GL_OBJ_SUPPORT,
    GL_OBJ_BBOX,
    GL_OBJ_DEFAULT,
    GL_OBJ_DEBUG,
};

struct objectRotationAngles
{
    float rotate_obj_x, rotate_obj_y, rotate_obj_z;
    float scale;
};

struct grid_position
{
    std::vector<Vertex> bed;
    std::vector<Vertex> frame;
};


GLuint LoadShaders (const char *vertex_file_path,
                    const char *fragment_file_path);

uint8_t
get_grid_data(struct grid_position *grid);

uint8_t
get_bounding_box(std::vector<v3> *bounding_box);

void
rotate_on_x(QMatrix4x4 *rotation_matrix, float angle);

void
rotate_on_y(QMatrix4x4 *rotation_matrix, float angle);

void
rotate_on_z(QMatrix4x4 *rotation_matrix, float angle);

void
scale_up_down(QMatrix4x4 &scale_matrix, uint8_t val, float *scale);

uint8_t
rotate_object(QMatrix4x4 *model_cord, int axis_dir,
              objectRotationAngles *angles);
uint8_t
rotate_object_on_arb_axis(v3 axis, float theta);
#endif // GL_UTILITIES_H
