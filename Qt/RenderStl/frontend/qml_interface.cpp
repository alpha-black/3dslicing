#include "qml_interface.h"
#include "middleware/errors.h"
#include "frontend/gl_scene.h"

#include <QDebug>

QMLInterfaceHandler::QMLInterfaceHandler()
{

}

/*****************************************************************
 * Signal Handlers, generated from QML
 *****************************************************************/

void
QMLInterfaceHandler::fileOpenedSignalHandler(QString fileName)
{
    if (dataHandler == NULL) {
        qDebug() << "Shared data handler is NULL. Something very wrong!";
        return;
    }

    /* Clear object matrix */
    dataHandler->obj_matrix.setToIdentity();
    dataHandler->readyToRender = false;
    dataHandler->update_obj_orientation = true;

    fileName = fileName.split(":")[1];
    dataHandler->fileName = fileName.toStdString();

    if (dataHandler->read_file() != SUCCESS) {
        qDebug() << "Error reading file";
        return;
    }
    GUI_MESSAGE("File successfully opened.");
    dataHandler->gl_window->update();    
}

void
QMLInterfaceHandler::generateGCodeSignalHandler(QString outFileName,
                                                int printer_id)
{
    if (dataHandler == NULL || dataHandler->readyToRender == false) {
        qDebug() << "Shared data handler is NULL. "
                    "Something very wrong!";
        GUI_MESSAGE("Please load a file.");
        return;
    }
#ifdef HAVE_EXT_BACKEND
    dataHandler->ext_args.printer_id = printer_id;
#endif

    outFileName = outFileName.split(":")[1];
    GUI_MESSAGE("Generating G-code...");    
    if (dataHandler->generateGCode(outFileName.toStdString()) != SUCCESS) {
        qDebug() << "Error in generating G Code.";
        return;
    }
    GUI_MESSAGE("G-code generated!");    
}

void
QMLInterfaceHandler::generateSupportSigHandler()
{
#if 0
    qDebug() << "Received Support request";
    if (dataHandler == NULL || dataHandler->facets == NULL ||
        dataHandler->readyToRender == false) {
        qDebug() << "Shared data handler is NULL. Or no file loaded.";
        return;
    }
    if (dataHandler->send_support_request() != SUCCESS) {
        qDebug() << "Support request failed";
        return;
    }
#endif
}

void
QMLInterfaceHandler::setSupportAndQualitySigH(int support_, int quality_)
{
    qDebug() << "Set support and quality.";
    if (dataHandler == NULL) {
        qDebug() << "Shared data handler is NULL. "
                    "Something very wrong!";
        return;
    }
#ifdef HAVE_EXT_BACKEND
    qDebug() << "Support " << support_ << " quality " << quality_;
    dataHandler->ext_args.quality_ = quality_;
    dataHandler->ext_args.support_ = support_;
    qDebug() << "Support and Q " << support_ << quality_;
    GUI_MESSAGE("Support and quality selections saved!");        
#endif
}
