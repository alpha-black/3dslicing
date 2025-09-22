/****************************************************************************
** Meta object code from reading C++ file 'gl_scene.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../RenderStl/frontend/gl_scene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gl_scene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OpenGLRender_t {
    const uint offsetsAndSize[6];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_OpenGLRender_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_OpenGLRender_t qt_meta_stringdata_OpenGLRender = {
    {
QT_MOC_LITERAL(0, 12), // "OpenGLRender"
QT_MOC_LITERAL(13, 8), // "renderGL"
QT_MOC_LITERAL(22, 0) // ""

    },
    "OpenGLRender\0renderGL\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenGLRender[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x0a,    1 /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void OpenGLRender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OpenGLRender *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->renderGL(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject OpenGLRender::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_OpenGLRender.offsetsAndSize,
    qt_meta_data_OpenGLRender,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_OpenGLRender_t
, QtPrivate::TypeAndForceComplete<OpenGLRender, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *OpenGLRender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGLRender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGLRender.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(this);
    return QObject::qt_metacast(_clname);
}

int OpenGLRender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_OpenGLQuickItem_t {
    const uint offsetsAndSize[34];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_OpenGLQuickItem_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_OpenGLQuickItem_t qt_meta_stringdata_OpenGLQuickItem = {
    {
QT_MOC_LITERAL(0, 15), // "OpenGLQuickItem"
QT_MOC_LITERAL(16, 11), // "xPosChanged"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 11), // "yPosChanged"
QT_MOC_LITERAL(41, 12), // "yZoomChanged"
QT_MOC_LITERAL(54, 15), // "resetPosChanged"
QT_MOC_LITERAL(70, 16), // "rotateObjChanged"
QT_MOC_LITERAL(87, 4), // "sync"
QT_MOC_LITERAL(92, 7), // "cleanup"
QT_MOC_LITERAL(100, 19), // "handleWindowChanged"
QT_MOC_LITERAL(120, 13), // "QQuickWindow*"
QT_MOC_LITERAL(134, 3), // "win"
QT_MOC_LITERAL(138, 4), // "xPos"
QT_MOC_LITERAL(143, 4), // "yPos"
QT_MOC_LITERAL(148, 5), // "yZoom"
QT_MOC_LITERAL(154, 8), // "resetPos"
QT_MOC_LITERAL(163, 9) // "rotateObj"

    },
    "OpenGLQuickItem\0xPosChanged\0\0yPosChanged\0"
    "yZoomChanged\0resetPosChanged\0"
    "rotateObjChanged\0sync\0cleanup\0"
    "handleWindowChanged\0QQuickWindow*\0win\0"
    "xPos\0yPos\0yZoom\0resetPos\0rotateObj"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenGLQuickItem[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       5,   72, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x06,    6 /* Public */,
       3,    0,   63,    2, 0x06,    7 /* Public */,
       4,    0,   64,    2, 0x06,    8 /* Public */,
       5,    0,   65,    2, 0x06,    9 /* Public */,
       6,    0,   66,    2, 0x06,   10 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   67,    2, 0x0a,   11 /* Public */,
       8,    0,   68,    2, 0x0a,   12 /* Public */,
       9,    1,   69,    2, 0x08,   13 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

 // properties: name, type, flags
      12, QMetaType::QReal, 0x00015103, uint(0), 0,
      13, QMetaType::QReal, 0x00015103, uint(1), 0,
      14, QMetaType::QReal, 0x00015103, uint(2), 0,
      15, QMetaType::Bool, 0x00015103, uint(3), 0,
      16, QMetaType::Int, 0x00015103, uint(4), 0,

       0        // eod
};

void OpenGLQuickItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OpenGLQuickItem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->xPosChanged(); break;
        case 1: _t->yPosChanged(); break;
        case 2: _t->yZoomChanged(); break;
        case 3: _t->resetPosChanged(); break;
        case 4: _t->rotateObjChanged(); break;
        case 5: _t->sync(); break;
        case 6: _t->cleanup(); break;
        case 7: _t->handleWindowChanged((*reinterpret_cast< std::add_pointer_t<QQuickWindow*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OpenGLQuickItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenGLQuickItem::xPosChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OpenGLQuickItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenGLQuickItem::yPosChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OpenGLQuickItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenGLQuickItem::yZoomChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OpenGLQuickItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenGLQuickItem::resetPosChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OpenGLQuickItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenGLQuickItem::rotateObjChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<OpenGLQuickItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->getXPos(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = _t->getYPos(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = _t->getYZoom(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->getResetPos(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->getRotateObj(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<OpenGLQuickItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setXPos(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setYPos(*reinterpret_cast< qreal*>(_v)); break;
        case 2: _t->setYZoom(*reinterpret_cast< qreal*>(_v)); break;
        case 3: _t->setResetPos(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setRotateObj(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject OpenGLQuickItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickItem::staticMetaObject>(),
    qt_meta_stringdata_OpenGLQuickItem.offsetsAndSize,
    qt_meta_data_OpenGLQuickItem,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_OpenGLQuickItem_t
, QtPrivate::TypeAndForceComplete<qreal, std::true_type>, QtPrivate::TypeAndForceComplete<qreal, std::true_type>, QtPrivate::TypeAndForceComplete<qreal, std::true_type>, QtPrivate::TypeAndForceComplete<bool, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<OpenGLQuickItem, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QQuickWindow *, std::false_type>


>,
    nullptr
} };


const QMetaObject *OpenGLQuickItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGLQuickItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGLQuickItem.stringdata0))
        return static_cast<void*>(this);
    return QQuickItem::qt_metacast(_clname);
}

int OpenGLQuickItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void OpenGLQuickItem::xPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void OpenGLQuickItem::yPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void OpenGLQuickItem::yZoomChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void OpenGLQuickItem::resetPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void OpenGLQuickItem::rotateObjChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
