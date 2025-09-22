#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QSurfaceFormat>
#include <QQmlContext>
#include <QDebug>

#include "frontend/qml_interface.h"
#include "frontend/gui_application.h"
#include "frontend/gl_scene.h"

extern SharedDataHandler *dataHandler;

void
GuiGlobal::guiAppInit()
{
    qDebug() << "Calling GUI App Init.";

    m_format = new QSurfaceFormat;
    m_format->setDepthBufferSize(24);
    m_format->setStencilBufferSize(8);
    m_format->setVersion(3, 4);
    m_format->setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(*m_format);

    qmlRegisterType<OpenGLQuickItem>("OpenGLUnderQML", 1, 0, "OpenGLQuickItem");

    /* Do not change the order. Set message property before setting source */
    m_view = new QQuickView;
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->rootContext()->setContextProperty("msg", &m_msg);
    m_view->setSource(QUrl("qrc:///main.qml"));
    m_view->show();
    
    /* Set the messaging class. This will be used from everywhere to send
     * messages to the GUI */
    dataHandler->gui_message = &m_msg;

    m_qml_handler = new QMLInterfaceHandler;
    m_qml_handler->setDataHandler(dataHandler);

    qDebug() << "Connecting slot to QML signals.";
    /* When STL file is opened in GUI */
    QObject::connect(m_view->rootObject(), SIGNAL(fileOpenedSignal(QString)),
                     m_qml_handler, SLOT(fileOpenedSignalHandler(QString)));
    /* To save a GCode file */
    QObject::connect(m_view->rootObject(), SIGNAL(generateGCodeSignal(QString,int)),
                     m_qml_handler, SLOT(generateGCodeSignalHandler(QString,int)));
    QObject::connect(m_view->rootObject(), SIGNAL(generateSupportSignal()),
                     m_qml_handler, SLOT(generateSupportSigHandler()));
    QObject::connect(m_view->rootObject(), SIGNAL(setSupportAndQuality(int, int)),
                     m_qml_handler, SLOT(setSupportAndQualitySigH(int, int)));
}

void
GuiGlobal::guiCleanup()
{
    qDebug() << "Cleanup and Quit.";

    /* Call other cleanups from here. */
    if (dataHandler) {
        dataHandler->cleanup ();
        delete dataHandler;
        dataHandler = NULL;
    }

    if (m_format) {
        delete m_format;
        m_format = NULL;
    }

    if (m_view) {
        delete m_view;
        m_view = NULL;
    }

    if (m_qml_handler) {
        delete m_qml_handler;
        m_qml_handler = NULL;
    }
}
