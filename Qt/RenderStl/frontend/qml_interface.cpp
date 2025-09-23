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
QMLInterfaceHandler::generateGCodeSignalHandler(QString outFileName)
{
    if (dataHandler == NULL || dataHandler->readyToRender == false) {
        qDebug() << "Shared data handler is NULL. "
                    "Something very wrong!";
        GUI_MESSAGE("Please load a file.");
        return;
    }

    outFileName = outFileName.split(":")[1];
    GUI_MESSAGE("Generating G-code...");    
    if (dataHandler->generateGCode(outFileName.toStdString()) != SUCCESS) {
        qDebug() << "Error in generating G Code.";
        return;
    }
    GUI_MESSAGE("G-code generation not implemented!");
}

void
QMLInterfaceHandler::generateSupportSigHandler()
{
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
}
