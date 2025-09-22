#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QSurfaceFormat>
#include <QDebug>

#include "frontend/gui_application.h"
#include "middleware/backend_main.h"

SharedDataHandler *dataHandler;

int main(int argc, char *argv[])
{    
    QGuiApplication app(argc, argv);
    dataHandler = new SharedDataHandler;

    GuiGlobal *guiApp = new GuiGlobal;
    guiApp->guiAppInit();

    /* Initialize the backend */
    backend_main *p_engine = new backend_main;
    dataHandler->m_engine = p_engine;

    QObject::connect(&app, SIGNAL(aboutToQuit()),
                     guiApp, SLOT(guiCleanup()));

    app.exec();
    app.setWindowIcon(QIcon("icons/RenderStl.ico"));

    return 1;
}
