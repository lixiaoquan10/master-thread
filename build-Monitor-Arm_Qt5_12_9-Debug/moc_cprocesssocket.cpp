/****************************************************************************
** Meta object code from reading C++ file 'cprocesssocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/business/cprocesssocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprocesssocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CprocessSocket_t {
    QByteArrayData data[11];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CprocessSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CprocessSocket_t qt_meta_stringdata_CprocessSocket = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CprocessSocket"
QT_MOC_LITERAL(1, 15, 14), // "slot_reconnect"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "slot_heartBeat"
QT_MOC_LITERAL(4, 46, 14), // "slot_readyRead"
QT_MOC_LITERAL(5, 61, 12), // "slot_connect"
QT_MOC_LITERAL(6, 74, 15), // "slot_disconnect"
QT_MOC_LITERAL(7, 90, 17), // "slot_stateChanged"
QT_MOC_LITERAL(8, 108, 28), // "QAbstractSocket::SocketState"
QT_MOC_LITERAL(9, 137, 5), // "state"
QT_MOC_LITERAL(10, 143, 28) // "slot_controlCloudTxtFileSize"

    },
    "CprocessSocket\0slot_reconnect\0\0"
    "slot_heartBeat\0slot_readyRead\0"
    "slot_connect\0slot_disconnect\0"
    "slot_stateChanged\0QAbstractSocket::SocketState\0"
    "state\0slot_controlCloudTxtFileSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CprocessSocket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    1,   54,    2, 0x0a /* Public */,
      10,    0,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,

       0        // eod
};

void CprocessSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CprocessSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_reconnect(); break;
        case 1: _t->slot_heartBeat(); break;
        case 2: _t->slot_readyRead(); break;
        case 3: _t->slot_connect(); break;
        case 4: _t->slot_disconnect(); break;
        case 5: _t->slot_stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 6: _t->slot_controlCloudTxtFileSize(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketState >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CprocessSocket::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CprocessSocket.data,
    qt_meta_data_CprocessSocket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CprocessSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CprocessSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CprocessSocket.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CprocessSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
