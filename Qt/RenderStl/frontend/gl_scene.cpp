#include "middleware/gl_utilities.h"
#include "middleware/geometry.h"
#include "middleware/errors.h"
#include "gl_scene.h"

#include <QtQuick/qquickwindow.h>
#include <QQuickOpenGLUtils>
#include <QTime>
#include <QDebug>
#include <QOpenGLExtraFunctions>

extern SharedDataHandler *dataHandler;

/*****************************************************************
 * OpenGLQuickItem member functions
 *****************************************************************/

OpenGLQuickItem::OpenGLQuickItem()
    :xPos(0), yPos(0), m_renderer(0)
{
    connect(this, &QQuickItem::windowChanged,
            this, &OpenGLQuickItem::handleWindowChanged);

    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);
}

OpenGLQuickItem::~OpenGLQuickItem()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = NULL;
    }
}

void
OpenGLQuickItem::setXPos(qreal pos)
{
    if (xPos == pos)
        return;

    xPos = pos;
    emit xPosChanged();

    if (m_renderer) {
        m_renderer->setMousePos(xPos, yPos);
    }
    return;
}

void
OpenGLQuickItem::setYPos(qreal pos)
{
    if (pos == yPos)
        return;

    yPos = pos;
    emit yPosChanged();

    if (m_renderer) {
        m_renderer->setMousePos(xPos, yPos);
    }
}

void
OpenGLQuickItem::setYZoom(qreal y)
{
    if (yZoom == y)
        return;

    yZoom = y;
    emit yZoomChanged();

    if (m_renderer) {
        m_renderer->changeZoom(yZoom);
        if (window())
            window()->update();
    }
}

void
OpenGLQuickItem::setResetPos(bool val)
{
    if (resetPos != val) {
        resetPos = val;
        emit resetPosChanged();
    }

    if (val == true && m_renderer)
        m_renderer->resetMousePos();
}

void
OpenGLQuickItem::setRotateObj(int val)
{
    if (rotateObj != val) {
        rotateObj = val;
        emit rotateObjChanged();
    }
    if (m_renderer) {
        m_renderer->rotateObject(val);
    }
}

void OpenGLQuickItem::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing,
                this, &OpenGLQuickItem::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated,
                this, &OpenGLQuickItem::cleanup, Qt::DirectConnection);

        //win->setClearBeforeRendering(false);
    }
}


/*****************************************************************
 * OpenGL slots
 *****************************************************************/

void OpenGLQuickItem::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
}

void OpenGLQuickItem::sync()
{
    if (!m_renderer) {
        m_renderer = new OpenGLRender();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer,
                &OpenGLRender::renderGL, Qt::DirectConnection);
    }
    m_renderer->setWindow(window());
}


/*****************************************************************
 * OpenGLRender member functions
 *****************************************************************/

OpenGLRender::OpenGLRender()
{
    stlProgramID = gridProgramID = 0;
    MVP.setToIdentity(); View.setToIdentity(); Model.setToIdentity();
    m_window = NULL;

    xPos = 0; yPos = 0;
    xPosPrev = 0; yPosPrev = 0;

    horizontalAngle = -0.785398f;
    verticalAngle = 1.0472f;
    zCam = 60.0f;

    setCamPosition();

    draw_grid = false;
    //grid_position.reserve(32);
    bounding_box.reserve(48);

    obj_angle.rotate_obj_x = obj_angle.rotate_obj_y = \
    obj_angle.rotate_obj_z = 0.0f;
    obj_angle.scale = 1.0f;

    initGL();
}

OpenGLRender::~OpenGLRender()
{
    /* TODO - delete gl buffers, Vertex Array
     * and program */
     glDeleteVertexArrays(2, gl.static_vao);
     glDeleteVertexArrays(4, gl.dynamic_vao);
     glDeleteBuffers(2, gl.static_vertex_buffer);
     glDeleteBuffers(4, gl.dynamic_vertex_buffer);

     glDeleteProgram(gl.object_shader_program);

     bounding_box.clear(); bounding_box.shrink_to_fit();
}

void
OpenGLRender::setWindow(QQuickWindow *window)
{
    if (!window)
        return;
    m_window = window;

    if (dataHandler) {
        dataHandler->gl_window = m_window;
    }
}

void
OpenGLRender::setMousePos(qreal x, qreal y)
{
    xPosPrev = xPos; yPosPrev = yPos;
    xPos = x; yPos = y;

    m_window->update();
}

void
OpenGLRender::resetMousePos()
{
    xPos = yPos = xPosPrev = yPosPrev = 0.0f;
    return;
}

void
OpenGLRender::rotateObject(int val)
{
    if (!m_window) {
        return;
    }

    if (rotate_object(&Model, val, &obj_angle) == SUCCESS) {
        m_window->update();
    }
    if (dataHandler)
        dataHandler->set_obj_metrics(&Model);
}

void
OpenGLRender::changeZoom(qreal y)
{
    float zCamPrev = zCam;
    zCam += y/2;

    if (zCam < 10 || zCam > 80) {
        zCam = zCamPrev;
    }
}

void
OpenGLRender::setCamPosition()
{
    int radius = 8;
    float verticalAnglePrev = verticalAngle;

    if (xPos > xPosPrev)
        horizontalAngle -= MOUSE_SPEED_HORIZONTAL;
    else if (xPos < xPosPrev)
        horizontalAngle += MOUSE_SPEED_HORIZONTAL;
    if (yPos > yPosPrev)
        verticalAngle += MOUSE_SPEED_VERTICAL;
    else if (yPos < yPosPrev)
        verticalAngle -= MOUSE_SPEED_VERTICAL;

    if (verticalAngle > 1.57f || verticalAngle < -0.785f)
        verticalAngle = verticalAnglePrev;

    camPosition.setX(radius * cos(verticalAngle) * sin(horizontalAngle));
    camPosition.setY(radius * sin(verticalAngle));
    camPosition.setZ(radius * cos(verticalAngle) * cos(horizontalAngle));
}

uint8_t
OpenGLRender::initGL()
{
    initializeOpenGLFunctions();

    /*******************
      Load the shaders
     *******************/
    gl.object_shader_program = LoadShaders ("vertex_shader.vs",
                                            "fragment_shader.fs");
    if (gl.object_shader_program == 0) {
        qDebug() << "Error loading object shader.";
        return ERROR_NO_SHADERS;
    }

    gl.obj_mvp_id = glGetUniformLocation (gl.object_shader_program, "MVP");
    gl.grid_mvp_id = glGetUniformLocation (gl.object_shader_program, "MVP_grid");
    gl.select_obj = glGetUniformLocation (gl.object_shader_program, "select_obj");
    gl.obj_model_to_world = glGetUniformLocation (gl.object_shader_program,
                                                  "model_to_world");
    gl.obj_world_to_view = glGetUniformLocation (gl.object_shader_program,
                                                 "world_to_view");

    /**************************************
     Create VAO and buffers for vertex data
     **************************************/
    glGenVertexArrays(2, gl.static_vao);
    glGenVertexArrays(4, gl.dynamic_vao);

    /* Two vertex buffers. Static and dynamic */
    glGenBuffers(2, gl.static_vertex_buffer);
    glGenBuffers(4, gl.dynamic_vertex_buffer);

    /* Static */
    if (get_grid_data(&grid) == SUCCESS) {
        glBindVertexArray(gl.static_vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, gl.static_vertex_buffer[0]);
        glEnableVertexAttribArray(ATTRIB_POSITION);
        glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
        glEnableVertexAttribArray(ATTRIB_NORMAL);
        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
        glBindVertexArray(0);

        /* glBindVertexArray(gl.static_vao[1]);
        glBindBuffer(GL_ARRAY_BUFFER, gl.static_vertex_buffer[1]);
        glEnableVertexAttribArray(ATTRIB_POSITION);
        glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
        glBindVertexArray(0); */
        draw_grid = true;
    }

    /* For the object */
    glBindVertexArray(gl.dynamic_vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[0]);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid *)offsetof(Vertex, position));
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid *)offsetof(Vertex, normal));
    glBindVertexArray(0);

    /* For the bounding box */
    glBindVertexArray(gl.dynamic_vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[1]);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    /* For infill */
    glBindVertexArray(gl.dynamic_vao[2]);
    glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[2]);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glBindVertexArray(0);

    /* For debugging */
    glBindVertexArray(gl.dynamic_vao[3]);
    glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[3]);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glBindVertexArray(0);

    return SUCCESS;
}

void
OpenGLRender::renderGL()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    /* Enable depth test */
    glEnable(GL_DEPTH_TEST  | GL_LINE_SMOOTH);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
    /* Trying to recognize color under the cursor on click */
    GLubyte *data = (GLubyte *)malloc(3 * m_window->height() * m_window->width());
    glReadPixels(0, 0, m_window->height(), m_window->width(), GL_RGB, GL_UNSIGNED_BYTE, data);
    for (auto i = 0; i < m_window->height() * m_window->width(); i+=3) {
        if (data[i] != 76)
        qDebug() << data[i] << data[i+1] << data[i+2];
    }
    delete(data);
#endif

    /*****************
     * Computation of M, V and P
     ****************/
    /* Angles computed from mouse movement. Convert from
     * spherical coordinates to cartesian. The idea is
     * to move the camera on a sphere around the center.
     * The radius of the sphere is controlled with zoom
     * or mouse wheel. These angles are camera angles. */
    setCamPosition();

    QMatrix4x4 Projection;
    Projection.perspective (zCam, 4.0f / 3.0f, 0.1f, 100.0f);

    View.setToIdentity();
    View.lookAt(camPosition, QVector3D(0,0,0), QVector3D(0,1,0));

    QMatrix4x4 MVP_grid;
    /* Grid is already in world coordinates */
    MVP_grid = Projection * View;

    /* Check if Model needs to be updated */
    if (dataHandler->update_obj_orientation) {
        dataHandler->update_obj_orientation = false;
        Model = dataHandler->obj_matrix;
    }

    /* Model matrix is set on rotation */
    MVP = Projection * View * Model;

    glUseProgram (gl.object_shader_program);
    glUniformMatrix4fv (gl.obj_mvp_id, 1, GL_FALSE, MVP.data());
    glUniformMatrix4fv (gl.obj_model_to_world, 1, GL_FALSE, Model.data());
    glUniformMatrix4fv (gl.obj_world_to_view, 1, GL_FALSE, View.data());
    glUniformMatrix4fv (gl.grid_mvp_id, 1, GL_FALSE, MVP_grid.data());

    if (draw_grid) {
        glUniform1i (gl.select_obj, GL_OBJ_BED);
        glBindVertexArray(gl.static_vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, gl.static_vertex_buffer[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * grid.bed.size(),
                     grid.bed.data(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, grid.bed.size());
        glBindVertexArray(0);
    }

    if (dataHandler && dataHandler->m_engine &&
        dataHandler->m_engine->slicing_done) {
        glUniform1i (gl.select_obj, GL_OBJ_SLICE);

        glBindVertexArray(gl.dynamic_vao[1]);
        glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[1]);
        for (auto i = 0u; i < dataHandler->m_engine->get_slicing_plane_count();
            ++i) {
            glBufferData(GL_ARRAY_BUFFER,
                dataHandler->m_engine->get_number_of_lines_on_plane(i) * \
                sizeof(v3), dataHandler->m_engine->get_lines_on_plane_addr(i),
                GL_STATIC_DRAW);
            glDrawArrays(GL_LINES, 0,
                dataHandler->m_engine->get_number_of_lines_on_plane(i));
        }
        glBindVertexArray(0);

        glUniform1i (gl.select_obj, GL_OBJ_INFILL);
        glBindVertexArray(gl.dynamic_vao[2]);
        glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[2]);
        for (auto i = 0u; i < dataHandler->m_engine->get_infill_count();
            ++i) {
            glBufferData(GL_ARRAY_BUFFER,
                 dataHandler->m_engine->get_loop_infill_count(i) * sizeof(v3),
                 dataHandler->m_engine->get_loop_infill_addr(i),
                 GL_STATIC_DRAW);
            glDrawArrays(GL_LINES, 0,
                dataHandler->m_engine->get_loop_infill_count(i));
        }
        glBindVertexArray(0);
    } else if (dataHandler && dataHandler->readyToRender) {
        glUniform1i (gl.select_obj, GL_OBJ_DEFAULT);
        glBindVertexArray(gl.dynamic_vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[0]);
        glBufferData(GL_ARRAY_BUFFER,
                dataHandler->facets->get_vertices_size() * sizeof(Vertex),
                dataHandler->facets->get_vertices_addr(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, dataHandler->facets->get_vertices_size());
        glBindVertexArray(0);

        /* Bounding box. TODO do not fill and clear everytime */
        if (get_bounding_box(&bounding_box) == SUCCESS) {
            glUniform1i (gl.select_obj, GL_OBJ_BBOX);

            glBindVertexArray(gl.dynamic_vao[1]);
            glBindBuffer(GL_ARRAY_BUFFER, gl.dynamic_vertex_buffer[1]);
            glBufferData(GL_ARRAY_BUFFER, bounding_box.size() * sizeof(v3),
                         bounding_box.data(), GL_STATIC_DRAW);
            glDrawArrays(GL_LINES, 0, bounding_box.size());
            glBindVertexArray(0);
            bounding_box.clear(); bounding_box.shrink_to_fit();
        }
    }
    glUseProgram(0);

    // m_window->resetOpenGLState();
    QQuickOpenGLUtils::resetOpenGLState();
}
