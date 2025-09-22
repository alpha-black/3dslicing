#include <fstream>
#include <QDebug>
#include <QMatrix4x4>

#include "middleware/gl_utilities.h"
#include "middleware/errors.h"
#include "middleware/utilities.h"
#ifdef HAVE_EXT_BACKEND
#include "middleware/shared_data_handler.h"
#endif /* HAVE_EXT_BACKEND */

extern SharedDataHandler *dataHandler;

GLuint
LoadShaders (const char *vertex_file_name, const char *fragment_file_name)
{
    char fragment_file_path[256];
    char vertex_file_path[256];
    std::string file_path = get_shaders_path();

    strcpy(vertex_file_path, file_path.c_str());
    strcat(vertex_file_path, vertex_file_name);
    strcpy(fragment_file_path, file_path.c_str());
    strcat(fragment_file_path, fragment_file_name);

    /* Create the shaders */
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    /* Read the Vertex Shader code from the file */
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream (vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    } else {
        qDebug() << "Impossible to open " << vertex_file_path
                  << " Are you in the right directory?";

        getchar();
        return 0;
    }

    /* Read the Fragment Shader code from the file */
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream (fragment_file_path, std::ios::in);

    if (FragmentShaderStream.is_open()){
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close ();
    }
    GLint Result = GL_FALSE;
    int InfoLogLength;

    /* Compile Vertex Shader */
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    /* Check Vertex Shader */
    glGetShaderiv (VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv (VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage (InfoLogLength+1);
        glGetShaderInfoLog (VertexShaderID, InfoLogLength, NULL,
            &VertexShaderErrorMessage[0]);
        qDebug() << &VertexShaderErrorMessage[0];
    }

    /* Compile Fragment Shader */
    char const * FragmentSourcePointer = FragmentShaderCode.c_str ();
    glShaderSource (FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader (FragmentShaderID);

    /* Check Fragment Shader */
    glGetShaderiv (FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv (FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage (InfoLogLength+1);
        glGetShaderInfoLog (FragmentShaderID, InfoLogLength, NULL,
            &FragmentShaderErrorMessage[0]);
        qDebug() << &FragmentShaderErrorMessage[0];
    }

    /* Link the program */
    GLuint ProgramID = glCreateProgram ();
    glAttachShader (ProgramID, VertexShaderID);
    glAttachShader (ProgramID, FragmentShaderID);
    glLinkProgram (ProgramID);

    /* Check the program */
    glGetProgramiv (ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv (ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage (InfoLogLength+1);
        glGetProgramInfoLog (ProgramID, InfoLogLength, NULL,
            &ProgramErrorMessage[0]);
        qDebug() << &ProgramErrorMessage[0];
    }

    glDetachShader (ProgramID, VertexShaderID);
    glDetachShader (ProgramID, FragmentShaderID);

    glDeleteShader (VertexShaderID);
    glDeleteShader (FragmentShaderID);

    return ProgramID;
}

uint8_t
get_grid_data(struct grid_position *grid)
{
    if (grid == NULL)
        return ERROR_NULL_PTR;

    Vertex v;
    v.normal.set(0.0f, 1.0f, 0.0f);
    v.position.set(-2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);

    v.position.set(-2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);

    /* Bed thickness */
    v.normal.set(0.0f, -1.0f, 0.0f);
    v.position.set(-2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);

    v.position.set(-2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);

    /* Sides - Side 1 */
    v.normal.set(0.0f, 0.0f, -1.0f);
    v.position.set(-2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);

    v.position.set(2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);

    /* Side 2 */
    v.normal.set(1.0f, 0.0f, 0.0f);
    v.position.set(2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);

    v.position.set(2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);


    /* Side 3 */
    v.normal.set(0.0f, 0.0f, 1.0f);
    v.position.set(2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);

    v.position.set(-2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);


    /* Side 4 */
    v.normal.set(-1.0f, 0.0f, 0.0f);
    v.position.set(-2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, -0.10f, 1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);

    v.position.set(-2.10f, -0.10f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, 0.0f, -1.50f);
    grid->bed.push_back(v);
    v.position.set(-2.10f, 0.0f, 1.50f);
    grid->bed.push_back(v);

    /* 2d frames. No thickness */
    Vertex c1;
    c1.normal.set(0.0f, 0.0f, 0.0f);
    c1.position.set(-2.10f, 0.0f, -1.50f);
    grid->frame.push_back(c1);
    c1.position.set(-2.10f, 3.0f, -1.50f);
    grid->frame.push_back(c1);
    c1.position.set(-2.0f, 3.0f, -1.50f);
    grid->frame.push_back(c1);

    c1.position.set(-2.10f, 0.0f, -1.50f);
    grid->frame.push_back(c1);
    c1.position.set(-2.0f, 3.0f, -1.50f);
    grid->frame.push_back(c1);
    c1.position.set(-2.0f, 0.0f, -1.50f);
    grid->frame.push_back(c1);


    c1.position.set(2.10f, 0.0f, -1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.10f, 3.0f, -1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.0f, 3.0f, -1.05f);
    grid->frame.push_back(c1);

    c1.position.set(2.10f, 0.0f, -1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.0f, 3.0f, -1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.0f, 0.0f, -1.05f);
    grid->frame.push_back(c1);


    c1.position.set(2.10f, 0.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.10f, 3.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.0f, 3.0f, 1.05f);
    grid->frame.push_back(c1);

    c1.position.set(2.10f, 0.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.0f, 3.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(2.0f, 0.0f, 1.05f);
    grid->frame.push_back(c1);


    c1.position.set(-2.10f, 0.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(-2.10f, 3.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(-2.0f, 3.0f, 1.05f);
    grid->frame.push_back(c1);

    c1.position.set(-2.10f, 0.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(-2.0f, 3.0f, 1.05f);
    grid->frame.push_back(c1);
    c1.position.set(-2.0f, 0.0f, 1.05f);
    grid->frame.push_back(c1);

    /*
    c1.position.set(-2.10f, 3.0f, -1.50f);
    grid->frame.push_back(c1);
    c1.position.set(2.10f, 3.0f, -1.50f);
    grid->frame.push_back(c1);
    c1.position.set(2.10f, 3.0f, 1.50f);
    grid->frame.push_back(c1);

    c1.position.set(-2.10f, 3.0f, -1.50f);
    grid->frame.push_back(c1);
    c1.position.set(2.10f, 3.0f, 1.50f);
    grid->frame.push_back(c1);
    c1.position.set(-2.10f, 3.0f, 1.50f);
    grid->frame.push_back(c1);
    */
    return SUCCESS;
}

uint8_t
get_bounding_box(std::vector<v3> *bounding_box)
{
    if (bounding_box == NULL || dataHandler == NULL ||
        dataHandler->facets == NULL) {
        return ERROR_NULL_PTR;
    }

    if (dataHandler->readyToRender == false)
        return ERROR_FACETS_NOT_READ;

    float max_x = 0.0f, max_y = 0.0f, max_z = 0.0f;
    dataHandler->facets->get_max_xyz(&max_x, &max_y, &max_z);

    float min = 0.0;

    /* Ones along the xyz axis */
    v3 cord(min, min, min);
    bounding_box->push_back(cord);
    cord.set(max_x, min, min);
    bounding_box->push_back(cord);

    cord.set(min, min, min);
    bounding_box->push_back(cord);
    cord.set(min, max_y, min);
    bounding_box->push_back(cord);

    cord.set(min, min, min);
    bounding_box->push_back(cord);
    cord.set(min, min, max_z);
    bounding_box->push_back(cord);

    /* Back face */
    cord.set(min, max_y, min);
    bounding_box->push_back(cord);
    cord.set(max_x, max_y, min);
    bounding_box->push_back(cord);


    cord.set(max_x, max_y, min);
    bounding_box->push_back(cord);
    cord.set(max_x, min, min);
    bounding_box->push_back(cord);


    /* Front face */
    cord.set(min, min, max_z);
    bounding_box->push_back(cord);
    cord.set(min, max_y, max_z);
    bounding_box->push_back(cord);

    cord.set(min, min, max_z);
    bounding_box->push_back(cord);
    cord.set(max_x, min, max_z);
    bounding_box->push_back(cord);


    cord.set(min, max_y, max_z);
    bounding_box->push_back(cord);
    cord.set(max_x, max_y, max_z);
    bounding_box->push_back(cord);


    cord.set(max_x, max_y, max_z);
    bounding_box->push_back(cord);
    cord.set(max_x, min, max_z);
    bounding_box->push_back(cord);


    cord.set(max_x, min, max_z);
    bounding_box->push_back(cord);
    cord.set(max_x, min, min);
    bounding_box->push_back(cord);


    /* Top lines */
    cord.set(max_x, max_y, max_z);
    bounding_box->push_back(cord);
    cord.set(max_x, max_y, min);
    bounding_box->push_back(cord);

    cord.set(min, max_y, max_z);
    bounding_box->push_back(cord);
    cord.set(min, max_y, min);
    bounding_box->push_back(cord);

    return SUCCESS;
}

void
rotate_on_x(QMatrix4x4 *rotation_matrix, float angle)
{
    rotation_matrix->setColumn(0, QVector4D(1, 0, 0, 0));
    rotation_matrix->setColumn(1, QVector4D(0, std::cos(angle),
    std::sin(angle), 0));
    rotation_matrix->setColumn(2, QVector4D(0, std::sin(angle) * -1,
    std::cos(angle), 0));
    rotation_matrix->setColumn(3, QVector4D(0, 0, 0, 1));
}

void
rotate_on_y(QMatrix4x4 *rotation_matrix, float angle)
{
    rotation_matrix->setColumn(0, QVector4D(std::cos(angle), 0,
    std::sin(angle) *  -1, 0));
    rotation_matrix->setColumn(1, QVector4D(0, 1, 0, 0));
    rotation_matrix->setColumn(2, QVector4D(std::sin(angle), 0,
    std::cos(angle), 0));
    rotation_matrix->setColumn(3, QVector4D(0, 0, 0, 1));
}

void
rotate_on_z(QMatrix4x4 *rotation_matrix, float angle)
{
    rotation_matrix->setColumn(0, QVector4D(std::cos(angle),
    std::sin(angle), 0, 0));
    rotation_matrix->setColumn(1, QVector4D(std::sin(angle) * -1,
    std::cos(angle), 0, 0));
    rotation_matrix->setColumn(2, QVector4D(0, 0, 1, 0));
    rotation_matrix->setColumn(3, QVector4D(0, 0, 0, 1));
}

void
scale_up_down(QMatrix4x4 &scale_matrix, uint8_t val, float *scale)
{
    float scale_factor = 0.0f;
    if (val == SCALE_UP) {
        scale_factor = SCALE_STEP;
        *scale = scale_factor;
    } else {
        scale_factor = 1.0f/SCALE_STEP;
        *scale = scale_factor;
    }
    qDebug() << "Current scaling " << *scale;
    scale_matrix.setColumn(0, QVector4D(scale_factor, 0.0f, 0.0f, 0.0f));
    scale_matrix.setColumn(1, QVector4D(0.0f, scale_factor, 0.0f, 0.0f));
    scale_matrix.setColumn(2, QVector4D(0.0f, 0.0f, scale_factor, 0.0f));
    scale_matrix.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
}

uint8_t
rotate_object(QMatrix4x4 *model_cord, int axis_dir, objectRotationAngles *angles)
{
    if (!model_cord || !angles)
        return ERROR_NULL_PTR;

    QMatrix4x4 translation_matrix;

    switch (axis_dir) {
    case ROTATE_ON_X_UP:
        angles->rotate_obj_x += ROTATION_STEP;
        rotate_on_x(&translation_matrix, ROTATION_STEP);
        break;
    case ROTATE_ON_X_DOWN:
        angles->rotate_obj_x -= ROTATION_STEP;
        rotate_on_x(&translation_matrix, ROTATION_STEP * -1);
        break;
    case ROTATE_ON_Y_UP:
        angles->rotate_obj_y += ROTATION_STEP;
        rotate_on_y(&translation_matrix, ROTATION_STEP);
        break;
    case ROTATE_ON_Y_DOWN:
        angles->rotate_obj_y -= ROTATION_STEP;
        rotate_on_y(&translation_matrix, ROTATION_STEP * -1);
        break;
    case ROTATE_ON_Z_UP:
        angles->rotate_obj_z += ROTATION_STEP;
        rotate_on_z(&translation_matrix, ROTATION_STEP);
        break;
    case ROTATE_ON_Z_DOWN:
        angles->rotate_obj_z -= ROTATION_STEP;
        rotate_on_z(&translation_matrix, ROTATION_STEP * -1);
        break;
    case SCALE_UP:
    case SCALE_DOWN:
        scale_up_down(translation_matrix, axis_dir, &angles->scale);
        break;
    default:
        break;
    }
    *model_cord = translation_matrix * (*model_cord);

    /* For ext-backend */
#ifdef HAVE_EXT_BACKEND
    if (dataHandler != NULL) {
        if (axis_dir == ROTATE_ON_Y_UP || axis_dir == ROTATE_ON_Y_DOWN) {
            dataHandler->ext_args.rotate_y = angles->rotate_obj_y;
        }
        else if (axis_dir == SCALE_UP || axis_dir == SCALE_DOWN) {
            dataHandler->ext_args.scale = angles->scale;
        }
    }
#endif

    return SUCCESS;
}

static inline void get_rotation_mat(QMatrix4x4& mat, v3 axis, float theta)
{
    float c = std::cos(theta);
    float s = std::sin(theta);

    mat.setColumn(0, QVector4D((axis.x * axis.x * (1 - c)) + c,
        axis.x * axis.y * (1-c) + (axis.z * s),
        axis.z * axis.x * (1-c) - (axis.y * s), 0));
    mat.setColumn(1, QVector4D(axis.x * axis.y * (1-c) - axis.z * s,
        axis.y * axis.y * (1-c) + c,
        axis.z * axis.y * (1-c) + axis.x * s, 0));
    mat.setColumn(2, QVector4D(axis.x * axis.z * (1-c) + axis.y * s,
        axis.y * axis.z * (1-c) - axis.x * s,
        axis.z * axis.z * (1-c) + c, 0));
    mat.setColumn(3, QVector4D(0, 0, 0, 1));
}

uint8_t
rotate_object_on_arb_axis(v3 axis, float theta)
{
    if (!dataHandler) {
        qDebug() << "Null pointer while rotating object about some arbitary "
        "axis for placing obj on bed.";
        return ERROR_NULL_PTR;
    }
    qDebug() << "Entering rotate_object_on_arb_axis.";
    QMatrix4x4 rot_matrix;
    get_rotation_mat(rot_matrix, axis, theta);

#if 0
    /* Test: Apply the transformation to the bounding box. See if it lies
     * below the xz plane. If it does, add 180 degrees to the rotation angle */
    std::vector<v3> bb; bb.reserve(48);
    get_bounding_box(&bb);

    for (auto i = 0u; i < bb.size(); ++i) {
        QVector4D vector(bb[i].x, bb[i].y, bb[i].z, 1.0f);
        QVector4D ret = rot_matrix * vector;

        if (ret.y() < -0.001f) {
            qDebug() << "Adding 180 degrees to rotation.";
            theta += 3.14f;
            get_rotation_mat(rot_matrix, axis, theta);
            break;
        }
    }
    bb.clear(); bb.shrink_to_fit();
#endif

    dataHandler->obj_matrix  = rot_matrix * dataHandler->obj_matrix;
    dataHandler->update_obj_orientation = true;
    return SUCCESS;
}
