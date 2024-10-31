#ifndef CCOMMUNICATIONLINKAGE_H
#define CCOMMUNICATIONLINKAGE_H

#include <QCoreApplication>
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include <QMutex>

class linkageSerialWorker : public QObject {
    Q_OBJECT

public:
    explicit linkageSerialWorker(QSerialPort *port = 0);
    ~linkageSerialWorker();

public slots:
    void sendData(const QByteArray &data);
    void readData();
    void setLinkageMsg(const long &baudrate, const QString &portName);
    void onErrorOccurred(QSerialPort::SerialPortError error);

signals:
    void ledDataReceived(const QByteArray &data);  // 数据接收信号
    void debug(const QByteArray &data);  // 数据接收信号
    void changeLinkageComplete();
private:
    QSerialPort *m_linkageSerialPort;
    QByteArray m_linkageReceiveData;
    QList<QByteArray> m_linkageSendDataList;  // 数据队列
    QMutex m_mutex; // 互斥锁
    bool m_sendingInProgress; //发送状态标志
};



#endif // CCOMMUNICATIONLINKAGE_H
