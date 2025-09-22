#ifndef GL_SCENE_H
#define GL_SCENE_H

#include <QQuickItem>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QTime>
#include <QDebug>

#include "middleware/gl_utilities.h"

#define ATTRIB_POSITION         0
#define ATTRIB_NORMAL           1

#define MOUSE_SPEED_HORIZONTAL      0.05f
#define MOUSE_SPEED_VERTICAL        0.025f

struct glGeneratedIndices
{
    GLuint static_vao[2], dynamic_vao[4];
    GLuint static_vertex_buffer[2], dynamic_vertex_buffer[4];
    GLuint object_shader_program;

    /* MVP - Model, View, Projection. Handlers and
     * Matricies for shaders and pre-computation.
     * MVP Matrix is used by both the shaders. */
    GLuint grid_mvp_id, select_obj;
    GLuint obj_mvp_id, obj_model_to_world, obj_world_to_view;
};

class OpenGLRender : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
    public:
        OpenGLRender();
        ~OpenGLRender();
        void setWindow(QQuickWindow *window);
        void setMousePos(qreal x, qreal y);
        void resetMousePos();
        void changeZoom(qreal y);
        void setCamPosition();
        void rotateObject(int val);
        uint8_t initGL();
    public slots:
        void renderGL();
    private:
        /* Pointer to the window. set from QuickItem */
        QQuickWindow *m_window;

        /* Mouse positions. xPos and yPos store the
         * difference in dist., not the absolute
         * coordinates */
        qreal xPos, yPos;

        /* Store the previous x y coordinates.
         * Useful in finding the distance moved */
        qreal xPosPrev, yPosPrev;

        /* Camera variable. Set when zoomed in or out */
        float zCam;

        glGeneratedIndices gl;

        /* Different shaders for the grid and
         * the object */
        GLuint stlProgramID;
        GLuint gridProgramID;

        QMatrix4x4 MVP, Model, View;
        QVector3D camPosition;

        /* Computed angles of movement on user mouse
         * movement */
        float horizontalAngle;
        float verticalAngle;

        /* Object rotation angles */
        objectRotationAngles obj_angle;

        /* Grid */
        struct grid_position grid;
        bool draw_grid;

        std::vector<v3> bounding_box;
};

class OpenGLQuickItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal xPos READ getXPos WRITE setXPos NOTIFY xPosChanged)
    Q_PROPERTY(qreal yPos READ getYPos WRITE setYPos NOTIFY yPosChanged)
    Q_PROPERTY(qreal yZoom READ getYZoom WRITE setYZoom NOTIFY yZoomChanged)
    Q_PROPERTY(bool resetPos READ getResetPos WRITE setResetPos NOTIFY resetPosChanged)
    Q_PROPERTY(int rotateObj READ getRotateObj WRITE setRotateObj NOTIFY rotateObjChanged)

    public:
        OpenGLQuickItem();
        ~OpenGLQuickItem();
        qreal getXPos() { return xPos; }
        void setXPos(qreal pos);
        qreal getYPos() { return yPos; }
        void setYPos(qreal pos);
        qreal getYZoom() { return yZoom; }
        void setYZoom(qreal y);
        void setResetPos(bool val);
        bool getResetPos() { return resetPos; }
        void setRotateObj(int val);
        int getRotateObj() { return rotateObj; }
    signals:
        void xPosChanged();
        void yPosChanged();
        void yZoomChanged();
        void resetPosChanged();
        void rotateObjChanged();

    public slots:
        void sync();
        void cleanup();

    private slots:
        void handleWindowChanged(QQuickWindow *win);

    private:
        qreal xPos;
        qreal yPos;
        qreal yZoom;
        bool resetPos;
        int rotateObj;
        /* Intanstiate and destroy OpenGL renderer
         * class from here. */
        OpenGLRender *m_renderer;
};

#endif // GL_SCENE_H
