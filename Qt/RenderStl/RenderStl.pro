TEMPLATE = app

QT += quick opengl gui qml quickwidgets core
CONFIG += debug
RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

#QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

# Default rules for deployment.
include(deployment.pri)

SOURCES += main.cpp \
    middleware/gl_utilities.cpp \
    frontend/gl_scene.cpp \
    frontend/qml_interface.cpp \
    frontend/gui_application.cpp \
    middleware/shared_data_handler.cpp \
    middleware/stl_parser.cpp \
    middleware/geometry.cpp \
    middleware/slicer.cpp \
    middleware/backend_main.cpp \
    middleware/backend_infill.cpp \
    middleware/utilities.cpp

HEADERS += \
    middleware/gl_utilities.h \
    frontend/gl_scene.h \
    frontend/qml_interface.h \
    frontend/gui_application.h \
    frontend/gui_messaging.h \    
    middleware/shared_data_handler.h \
    middleware/geometry.h \
    middleware/errors.h \
    middleware/stl_parser.h \
    middleware/slicer.h \
    middleware/backend_main.h \
    middleware/backend_infill.h \
    middleware/utilities.h

Shaders.path = Contents/Resources/Shaders/
Shaders.files += \
    fragment_shader.fs \
    vertex_shader.vs    \

ICON = assets/icons/MHI_3d.icns

QMAKE_BUNDLE_DATA += Shaders ICON

CONFIG(release, debug|release):message(Release build!)
CONFIG(debug, debug|release):message(Debug build!)

DESTDIR = bin/
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
