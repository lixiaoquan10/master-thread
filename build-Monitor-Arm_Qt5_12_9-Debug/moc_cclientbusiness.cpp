/****************************************************************************
** Meta object code from reading C++ file 'cclientbusiness.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/business/cclientbusiness.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cclientbusiness.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CClientBusiness_t {
    QByteArrayData data[33];
    char stringdata0[404];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CClientBusiness_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CClientBusiness_t qt_meta_stringdata_CClientBusiness = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CClientBusiness"
QT_MOC_LITERAL(1, 16, 17), // "notifyInformation"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 4), // "type"
QT_MOC_LITERAL(4, 40, 11), // "information"
QT_MOC_LITERAL(5, 52, 13), // "commandResult"
QT_MOC_LITERAL(6, 66, 11), // "commandType"
QT_MOC_LITERAL(7, 78, 15), // "CMsgStructBase*"
QT_MOC_LITERAL(8, 94, 7), // "msgData"
QT_MOC_LITERAL(9, 102, 18), // "requestVerifyAdmin"
QT_MOC_LITERAL(10, 121, 12), // "statusRecord"
QT_MOC_LITERAL(11, 134, 10), // "StatusInfo"
QT_MOC_LITERAL(12, 145, 10), // "statusInfo"
QT_MOC_LITERAL(13, 156, 3), // "opr"
QT_MOC_LITERAL(14, 160, 18), // "slotCheckLoginInfo"
QT_MOC_LITERAL(15, 179, 13), // "mainPowerStop"
QT_MOC_LITERAL(16, 193, 18), // "slot_saveLoginData"
QT_MOC_LITERAL(17, 212, 17), // "slot_flashControl"
QT_MOC_LITERAL(18, 230, 21), // "slot_linkageComStatus"
QT_MOC_LITERAL(19, 252, 17), // "DeviceResetStatus"
QT_MOC_LITERAL(20, 270, 24), // "DeviceDefaultResetStatus"
QT_MOC_LITERAL(21, 295, 19), // "objectStatusChanged"
QT_MOC_LITERAL(22, 315, 8), // "CObject*"
QT_MOC_LITERAL(23, 324, 6), // "object"
QT_MOC_LITERAL(24, 331, 6), // "status"
QT_MOC_LITERAL(25, 338, 8), // "oldValue"
QT_MOC_LITERAL(26, 347, 8), // "newValue"
QT_MOC_LITERAL(27, 356, 4), // "time"
QT_MOC_LITERAL(28, 361, 7), // "timeOut"
QT_MOC_LITERAL(29, 369, 8), // "recvData"
QT_MOC_LITERAL(30, 378, 8), // "nMsgType"
QT_MOC_LITERAL(31, 387, 11), // "QByteArray&"
QT_MOC_LITERAL(32, 399, 4) // "data"

    },
    "CClientBusiness\0notifyInformation\0\0"
    "type\0information\0commandResult\0"
    "commandType\0CMsgStructBase*\0msgData\0"
    "requestVerifyAdmin\0statusRecord\0"
    "StatusInfo\0statusInfo\0opr\0slotCheckLoginInfo\0"
    "mainPowerStop\0slot_saveLoginData\0"
    "slot_flashControl\0slot_linkageComStatus\0"
    "DeviceResetStatus\0DeviceDefaultResetStatus\0"
    "objectStatusChanged\0CObject*\0object\0"
    "status\0oldValue\0newValue\0time\0timeOut\0"
    "recvData\0nMsgType\0QByteArray&\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CClientBusiness[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   84,    2, 0x06 /* Public */,
       5,    2,   89,    2, 0x06 /* Public */,
       9,    2,   94,    2, 0x06 /* Public */,
      10,    3,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  106,    2, 0x0a /* Public */,
      15,    0,  107,    2, 0x0a /* Public */,
      16,    0,  108,    2, 0x0a /* Public */,
      17,    0,  109,    2, 0x0a /* Public */,
      18,    0,  110,    2, 0x0a /* Public */,
      19,    0,  111,    2, 0x0a /* Public */,
      20,    0,  112,    2, 0x0a /* Public */,
      21,    5,  113,    2, 0x0a /* Public */,
      28,    0,  124,    2, 0x08 /* Private */,
      29,    2,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QVariantList,    3,    4,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    6,    8,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int, QMetaType::Int,   12,    3,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22, QMetaType::Short, QMetaType::UChar, QMetaType::UChar, QMetaType::UInt,   23,   24,   25,   26,   27,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 31,   30,   32,

       0        // eod
};

void CClientBusiness::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CClientBusiness *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->notifyInformation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QList<QVariant>(*)>(_a[2]))); break;
        case 1: _t->commandResult((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< CMsgStructBase*(*)>(_a[2]))); break;
        case 2: _t->requestVerifyAdmin((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< CMsgStructBase*(*)>(_a[2]))); break;
        case 3: _t->statusRecord((*reinterpret_cast< const StatusInfo(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->slotCheckLoginInfo(); break;
        case 5: _t->mainPowerStop(); break;
        case 6: _t->slot_saveLoginData(); break;
        case 7: _t->slot_flashControl(); break;
        case 8: _t->slot_linkageComStatus(); break;
        case 9: _t->DeviceResetStatus(); break;
        case 10: _t->DeviceDefaultResetStatus(); break;
        case 11: _t->objectStatusChanged((*reinterpret_cast< CObject*(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< unsigned char(*)>(_a[4])),(*reinterpret_cast< uint(*)>(_a[5]))); break;
        case 12: _t->timeOut(); break;
        case 13: _t->recvData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CMsgStructBase* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CMsgStructBase* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CClientBusiness::*)(int , const QList<QVariant> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CClientBusiness::notifyInformation)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CClientBusiness::*)(int , CMsgStructBase * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CClientBusiness::commandResult)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CClientBusiness::*)(int , CMsgStructBase * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CClientBusiness::requestVerifyAdmin)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CClientBusiness::*)(const StatusInfo & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CClientBusiness::statusRecord)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CClientBusiness::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CClientBusiness.data,
    qt_meta_data_CClientBusiness,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CClientBusiness::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CClientBusiness::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CClientBusiness.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CClientBusiness::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CClientBusiness::notifyInformation(int _t1, const QList<QVariant> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CClientBusiness::commandResult(int _t1, CMsgStructBase * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CClientBusiness::requestVerifyAdmin(int _t1, CMsgStructBase * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CClientBusiness::statusRecord(const StatusInfo & _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
