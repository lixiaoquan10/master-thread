/****************************************************************************
** Meta object code from reading C++ file 'cprocessserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/business/cprocessserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprocessserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CprocessServer_t {
    QByteArrayData data[13];
    char stringdata0[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CprocessServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CprocessServer_t qt_meta_stringdata_CprocessServer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CprocessServer"
QT_MOC_LITERAL(1, 15, 19), // "slot_readClientData"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 24), // "slot_handleNewConnection"
QT_MOC_LITERAL(4, 61, 17), // "slot_serverlisten"
QT_MOC_LITERAL(5, 79, 23), // "slot_handleDisconnected"
QT_MOC_LITERAL(6, 103, 13), // "onSocketError"
QT_MOC_LITERAL(7, 117, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(8, 146, 5), // "error"
QT_MOC_LITERAL(9, 152, 14), // "slot_heartBeat"
QT_MOC_LITERAL(10, 167, 18), // "handleBytesWritten"
QT_MOC_LITERAL(11, 186, 5), // "bytes"
QT_MOC_LITERAL(12, 192, 29) // "slot_controlMasterTxtFileSize"

    },
    "CprocessServer\0slot_readClientData\0\0"
    "slot_handleNewConnection\0slot_serverlisten\0"
    "slot_handleDisconnected\0onSocketError\0"
    "QAbstractSocket::SocketError\0error\0"
    "slot_heartBeat\0handleBytesWritten\0"
    "bytes\0slot_controlMasterTxtFileSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CprocessServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    1,   58,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,
      10,    1,   62,    2, 0x0a /* Public */,
      12,    0,   65,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   11,
    QMetaType::Void,

       0        // eod
};

void CprocessServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CprocessServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_readClientData(); break;
        case 1: _t->slot_handleNewConnection(); break;
        case 2: _t->slot_serverlisten(); break;
        case 3: _t->slot_handleDisconnected(); break;
        case 4: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->slot_heartBeat(); break;
        case 6: _t->handleBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 7: _t->slot_controlMasterTxtFileSize(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CprocessServer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CprocessServer.data,
    qt_meta_data_CprocessServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CprocessServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CprocessServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CprocessServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CprocessServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
