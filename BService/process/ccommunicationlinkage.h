#ifndef CCOMMUNICATIONLINKAGE_H
#define CCOMMUNICATIONLINKAGE_H

#include <QCoreApplication>
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QTimer>

class linkageSerialWorker : public QObject {
    Q_OBJECT

public:
    explicit linkageSerialWorker(QSerialPort *port = 0);
    ~linkageSerialWorker();

public slots:
    void sendData(const QByteArray &data);
    void readData();
    void onErrorOccurred(QSerialPort::SerialPortError error);
    void slot_LinkageFlagReset();
    void slot_ARTUDIsend();

signals:
    void linkageDataReceived(const int &type, const QByteArray &data);        // 数据接收信号
    void switchLedAndLinkageSerial(const QByteArray &data);  // 交换灯键和火报串口
    void debug(const QByteArray &data);
private:
    QSerialPort *m_linkageSerialPort;
    QByteArray m_linkageReceiveData;
    QList<QByteArray> m_linkageSendDataList;  // 数据队列
    QMutex m_mutex; // 互斥锁
    bool m_sendingInProgress; //发送状态标志

    //火报通讯标志定时复位
    QTimer* m_timerResetLinkageFlag;
    //ARTU数据间隔发送
    QTimer* m_timerARTUDIsend;
    //ARTU发送数据链表
    QByteArrayList ARTUDIlists;
};



#endif // CCOMMUNICATIONLINKAGE_H
