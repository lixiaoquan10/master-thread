/****************************************************************************
** Meta object code from reading C++ file 'cprocessmqtt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/business/cprocessmqtt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprocessmqtt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CprocessMqtt_t {
    QByteArrayData data[15];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CprocessMqtt_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CprocessMqtt_t qt_meta_stringdata_CprocessMqtt = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CprocessMqtt"
QT_MOC_LITERAL(1, 13, 15), // "connectToBroker"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "host"
QT_MOC_LITERAL(4, 35, 4), // "port"
QT_MOC_LITERAL(5, 40, 14), // "publishMessage"
QT_MOC_LITERAL(6, 55, 5), // "topic"
QT_MOC_LITERAL(7, 61, 7), // "message"
QT_MOC_LITERAL(8, 69, 16), // "subscribeToTopic"
QT_MOC_LITERAL(9, 86, 20), // "updateLogStateChange"
QT_MOC_LITERAL(10, 107, 24), // "QMqttClient::ClientState"
QT_MOC_LITERAL(11, 132, 5), // "state"
QT_MOC_LITERAL(12, 138, 18), // "brokerDisconnected"
QT_MOC_LITERAL(13, 157, 21), // "handleMessageReceived"
QT_MOC_LITERAL(14, 179, 14) // "QMqttTopicName"

    },
    "CprocessMqtt\0connectToBroker\0\0host\0"
    "port\0publishMessage\0topic\0message\0"
    "subscribeToTopic\0updateLogStateChange\0"
    "QMqttClient::ClientState\0state\0"
    "brokerDisconnected\0handleMessageReceived\0"
    "QMqttTopicName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CprocessMqtt[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x0a /* Public */,
       5,    2,   49,    2, 0x0a /* Public */,
       8,    1,   54,    2, 0x0a /* Public */,
       9,    1,   57,    2, 0x08 /* Private */,
      12,    0,   60,    2, 0x08 /* Private */,
      13,    2,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    6,    7,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 14,    7,    6,

       0        // eod
};

void CprocessMqtt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CprocessMqtt *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectToBroker((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 1: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->subscribeToTopic((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->updateLogStateChange((*reinterpret_cast< QMqttClient::ClientState(*)>(_a[1]))); break;
        case 4: _t->brokerDisconnected(); break;
        case 5: _t->handleMessageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QMqttTopicName(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttClient::ClientState >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttTopicName >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CprocessMqtt::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CprocessMqtt.data,
    qt_meta_data_CprocessMqtt,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CprocessMqtt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CprocessMqtt::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CprocessMqtt.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CprocessMqtt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
