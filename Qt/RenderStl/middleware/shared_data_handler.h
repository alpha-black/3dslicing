#ifndef SHARED_DATA_HANDLER_H
#define SHARED_DATA_HANDLER_H

#include "backend_main.h"
#include "geometry.h"
#include "utilities.h"
#include "errors.h"
#include "frontend/gui_messaging.h"
#include <fstream>
#include <QMatrix4x4>
#include <QQuickWindow>

struct SharedDataHandler
{
    SharedDataHandler () : 
        facets(NULL), 
        readyToRender(false),
        update_obj_orientation(false), 
        m_engine(NULL) {}
    ~SharedDataHandler() { 
        cleanup(); 
    }

    void cleanup();

    /* Stl file name */
    std::string fileName;

    /* Mesh */
    FacetList *facets;

    /* Some Stl file has been opened for rendering */
    bool readyToRender;

    /* Generate G code. Maybe call backend from
     * here. Now, call Slic3r from the command
     * prompt. */
    uint8_t generateGCode(std::string outFileName);
    uint8_t read_file();
    uint8_t send_support_request();

    QMatrix4x4 obj_matrix;
    void set_obj_metrics(QMatrix4x4 *obj_matrix);
    /* Update: Not used currently. Can be removed.
     * This is set only once. First time for orienting the
     * obj on the bed */
    bool update_obj_orientation;

    /* Pointer to the backend engine */
    backend_main *m_engine;

    /* GUI messaging */
    GuiMessage *gui_message;

    /* To redraw the GL canvas */
    QQuickWindow *gl_window;
};


#endif // SHARED_DATA_HANDLER_H
