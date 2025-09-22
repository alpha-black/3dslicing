/****************************************************************************
** Meta object code from reading C++ file 'qml_interface.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../RenderStl/frontend/qml_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qml_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QMLInterfaceHandler_t {
    const uint offsetsAndSize[22];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_QMLInterfaceHandler_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_QMLInterfaceHandler_t qt_meta_stringdata_QMLInterfaceHandler = {
    {
QT_MOC_LITERAL(0, 19), // "QMLInterfaceHandler"
QT_MOC_LITERAL(20, 23), // "fileOpenedSignalHandler"
QT_MOC_LITERAL(44, 0), // ""
QT_MOC_LITERAL(45, 8), // "fileName"
QT_MOC_LITERAL(54, 26), // "generateGCodeSignalHandler"
QT_MOC_LITERAL(81, 11), // "outFileName"
QT_MOC_LITERAL(93, 10), // "printer_id"
QT_MOC_LITERAL(104, 25), // "generateSupportSigHandler"
QT_MOC_LITERAL(130, 24), // "setSupportAndQualitySigH"
QT_MOC_LITERAL(155, 8), // "support_"
QT_MOC_LITERAL(164, 8) // "quality_"

    },
    "QMLInterfaceHandler\0fileOpenedSignalHandler\0"
    "\0fileName\0generateGCodeSignalHandler\0"
    "outFileName\0printer_id\0generateSupportSigHandler\0"
    "setSupportAndQualitySigH\0support_\0"
    "quality_"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QMLInterfaceHandler[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x0a,    1 /* Public */,
       4,    2,   41,    2, 0x0a,    3 /* Public */,
       7,    0,   46,    2, 0x0a,    6 /* Public */,
       8,    2,   47,    2, 0x0a,    7 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,

       0        // eod
};

void QMLInterfaceHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QMLInterfaceHandler *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->fileOpenedSignalHandler((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->generateGCodeSignalHandler((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->generateSupportSigHandler(); break;
        case 3: _t->setSupportAndQualitySigH((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject QMLInterfaceHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_QMLInterfaceHandler.offsetsAndSize,
    qt_meta_data_QMLInterfaceHandler,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QMLInterfaceHandler_t
, QtPrivate::TypeAndForceComplete<QMLInterfaceHandler, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *QMLInterfaceHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QMLInterfaceHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QMLInterfaceHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QMLInterfaceHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
