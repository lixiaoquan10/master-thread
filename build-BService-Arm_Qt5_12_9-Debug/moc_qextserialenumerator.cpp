/****************************************************************************
** Meta object code from reading C++ file 'qextserialenumerator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/serial/qextserialenumerator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qextserialenumerator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QextSerialEnumerator_t {
    QByteArrayData data[6];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QextSerialEnumerator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QextSerialEnumerator_t qt_meta_stringdata_QextSerialEnumerator = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QextSerialEnumerator"
QT_MOC_LITERAL(1, 21, 16), // "deviceDiscovered"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 12), // "QextPortInfo"
QT_MOC_LITERAL(4, 52, 4), // "info"
QT_MOC_LITERAL(5, 57, 13) // "deviceRemoved"

    },
    "QextSerialEnumerator\0deviceDiscovered\0"
    "\0QextPortInfo\0info\0deviceRemoved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QextSerialEnumerator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       5,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void QextSerialEnumerator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QextSerialEnumerator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceDiscovered((*reinterpret_cast< const QextPortInfo(*)>(_a[1]))); break;
        case 1: _t->deviceRemoved((*reinterpret_cast< const QextPortInfo(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QextSerialEnumerator::*)(const QextPortInfo & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QextSerialEnumerator::deviceDiscovered)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QextSerialEnumerator::*)(const QextPortInfo & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QextSerialEnumerator::deviceRemoved)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QextSerialEnumerator::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_QextSerialEnumerator.data,
    qt_meta_data_QextSerialEnumerator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QextSerialEnumerator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QextSerialEnumerator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QextSerialEnumerator.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QextSerialEnumerator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QextSerialEnumerator::deviceDiscovered(const QextPortInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QextSerialEnumerator::deviceRemoved(const QextPortInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
