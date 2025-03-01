#ifndef CCOMMUNICATIONLED_H
#define CCOMMUNICATIONLED_H

#include <QCoreApplication>
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include <QMutex>

//class ledSerialWorker : public QObject {
//    Q_OBJECT

//public:
//    explicit ledSerialWorker(QSerialPort *port = 0);
//    ~ledSerialWorker();

//public slots:
//    void sendData(const QByteArray &data);
//    void readData();


//    void onErrorOccurred(QSerialPort::SerialPortError error);

//signals:
//    void ledDataReceived(const QByteArray &data);  // 数据接收信号
//    void debug(const QByteArray &data);  // 数据接收信号
//private:
//    QSerialPort *m_ledSerialPort;
//    QByteArray m_ledReceiveData;
//    QList<QByteArray> m_ledSendDataList;  // 数据队列
//    QMutex m_mutex; // 互斥锁
//};


class ledSerialSender : public QObject
{
    Q_OBJECT

public:
    explicit ledSerialSender(QSerialPort *port = 0);
    ~ledSerialSender();
public slots:
    void sendData(const QByteArray &data);
private:
    QSerialPort *m_ledSerialSender;
    QList<QByteArray> m_ledSendDataList;  // 数据队列
};

class ledSerialReceiver : public QObject
{
    Q_OBJECT

public:
    explicit ledSerialReceiver(QSerialPort *port = 0);
    ~ledSerialReceiver();
public slots:
    void readData();
    void onErrorOccurred(QSerialPort::SerialPortError error);
signals:
    void dataReceived(const QByteArray &data);  // 数据接收信号
private:
    QSerialPort *m_ledSerialReceiver;
    QByteArray m_ledReceiveData;
};


#endif // CCOMMUNICATIONLED_H
