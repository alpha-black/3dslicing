#ifndef GUI_APPLICATION_H
#define GUI_APPLICATION_H

#include "middleware/shared_data_handler.h"
#include "frontend/qml_interface.h"
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QQuickView>

class GuiView : public QQuickView
{
    Q_OBJECT
public:
    GuiView() {
        setResizeMode(QQuickView::SizeRootObjectToView);
        setSource(QUrl("qrc:///main.qml"));
    }
};


class GuiGlobal :public QObject
{
    Q_OBJECT
public:
    GuiGlobal() : m_format (NULL), m_view(NULL), m_qml_handler(NULL) {}
    ~GuiGlobal() { guiCleanup(); }
    void guiAppInit();

public slots:
    void guiCleanup();

private:
    QSurfaceFormat *m_format;
    QQuickView *m_view;
    QMLInterfaceHandler *m_qml_handler;
    GuiMessage m_msg;
};

#endif // GUI_APPLICATION_H
