#ifndef CCOMMUNICATIONROUTE_H
#define CCOMMUNICATIONROUTE_H

//#include <QCoreApplication>
//#include <QSerialPort>
//#include <QThread>
//#include <QDebug>
//#include <QMutex>

//class ledSerialWorker : public QObject {
//    Q_OBJECT

//public:
//    ledSerialWorker(QSerialPort *port) : m_ledSerialPort(port)
//    {
//        if(!m_ledSerialPort)
//            return;
//        if(m_ledSerialPort->open(QIODevice::ReadWrite))
//        {
//            connect(m_ledSerialPort, &QSerialPort::readyRead, this, &ledSerialWorker::readData);
//            connect(m_ledSerialPort, &QSerialPort::errorOccurred, this, &ledSerialWorker::onErrorOccurred);
//        }
//    }

//public slots:
//    void sendData(const QByteArray &data)
//    {
//        QMutexLocker locker(&m_mutex); // 加锁
//        m_ledSendDataList.append(data);  // 将新数据加入队列
//        if (m_ledSendDataList.isEmpty())
//        {
////            qDebug() << "No data to send.";
//            return;
//        }
//        if (m_ledSerialPort->isOpen())
//        {
//            QByteArray sendData = m_ledSendDataList.takeFirst();  // 取出并移除第一条数据
//            qint64 bytesWritten = m_ledSerialPort->write(sendData);
//            if (bytesWritten == -1)
//            {
////                qDebug() << "Failed to write data:" << m_serialPort->errorString();
//            } else
//            {
////                qDebug() << "Data sent, bytes written:" << bytesWritten;
//            }
//        } else {
////            qDebug() << "Serial port is not open.";
//        }
//    }

//    void readData()
//    {
//        QMutexLocker locker(&m_mutex); // 加锁
//        QByteArray data = m_ledSerialPort->readAll();
////        qDebug() << "Data received:" << data;
//        if(data.isEmpty())
//            return;
//        m_ledReceiveData.append(data);

//        while (m_ledReceiveData.size() >= 47)
//        {
//            // 检查开头是否为 0x55 0x13
//            if (m_ledReceiveData.at(0) == 0x55 && m_ledReceiveData.at(1) == 0x13)
//            {

//                QByteArray reciveData = m_ledReceiveData.mid(0, 47);
//                m_ledReceiveData.remove(0, 47); // 从索引 0 开始删除 47 个字节
//                char sum = 0;
//                for (int ix = 0; ix < 46; ix++)
//                    sum += reciveData.at(ix);
//                if (reciveData.at(46) != sum) {
//                    reciveData.clear();
//                    return;
//                }
//                QByteArray tmpByteArray;
//                tmpByteArray.append(reciveData.at(2));
//                tmpByteArray.append(reciveData.at(6));
//                tmpByteArray.append(reciveData.at(7));
//                tmpByteArray.append(reciveData.at(9));
//                tmpByteArray.append(reciveData.at(10));
//                tmpByteArray.append(reciveData.at(14));
//                tmpByteArray.append(reciveData.at(15));
//                tmpByteArray.append(reciveData.at(16));
//                tmpByteArray.append(reciveData.at(17));
//                tmpByteArray.append(reciveData.at(18));
//                tmpByteArray.append(reciveData.at(19));
//                tmpByteArray.append(reciveData.at(20));
//                reciveData.clear();
//                emit ledDataReceived(tmpByteArray);  // 发送接收到的数据
//            } else {
//                // 删除开头无效数据
//                m_ledReceiveData.remove(0, 1); // 删除第一个字节
//            }
//        }
//    }

//    void onErrorOccurred(QSerialPort::SerialPortError error) {
//        if (error != QSerialPort::NoError) {
////            qDebug() << "Serial port error occurred:" << m_serialPort->errorString();
//        }
//    }

//signals:
//    void ledDataReceived(const QByteArray &data);  // 数据接收信号
//    void debug(const QByteArray &data);  // 数据接收信号
//private:
//    QSerialPort *m_ledSerialPort;
//    QByteArray m_ledReceiveData;
//    QList<QByteArray> m_ledSendDataList;  // 数据队列
//    QMutex m_mutex; // 互斥锁
//};



#include <QHash>
#include <QObject>
#include <QTimer>
#include <QSerialPort>
#include "struct.h"

#define INTERFACEBOARD_TIMEOUT 4000
#define INTERFACEBOARD_PORT_NUMBER_STR "_portNumber_"   // 0 ~ 4, 0 is the interface board
#define INTERFACEBOARD_PORT_SPEED_STR "_portSPeed_"
#define INTERFACEBOARD_COMPORT_NUMBER_STR "Port"        // 1 2 3 ...
#define INTERFACEBOARD_COMMPNYCODE "commpny"
#define INTERFACEBOARD_PANEL   "panel"

class CProcessBase;
class CCommunicationManager;

class CCommunicationRoute : public QObject
{
    Q_OBJECT
public:
    explicit CCommunicationRoute(QObject *parent = 0);
    ~CCommunicationRoute();
    bool createRoute(CProcessBase* process, const int type, const QHash<QString, QVariant>& control);
    void setLedStatus(const int ledStatus1, const int ledStatus2, const int ledStatus3);
    void openCommunication();
    void closeCommunication();
    void setDebug(const bool enable);
    QString canNameByAdd(int canportAdd);
    void testLinkageCom();
    void setLinkageBaudRate(QString baudrateString);

    void canSendDataClear();
    void canReceiveDataClear();
    void linkageSendData(QByteArray data);

    void onDataProcessed(const QByteArray &data);
    QSerialPort* makeSerialPort(QString serialName, long baudrate);
    long ledSerialBaudRate() { return m_ledSerialPort->baudRate(); }
    QString ledSerialName() { return m_ledSerialPort->portName(); }
    long linkageSerialBaudRate() { return m_linkageSerialPort->baudRate(); }
    QString linkageSerialName() { return m_linkageSerialPort->portName(); }

private:
    void initCommunicationManager();

    int  getIntFromByte(QByteArray data, int start, int len);
    QString createCommunication(const int type);
    QString getCommunicationName(const int type);
    int  getCommunicationType(const QString lineName);
    bool isCommunicationLine(const int type) const;
    bool communicationIsActive(const QString& name) const;

signals:
    void communicationSendEvent(const QString& name, const QString &commandName, const QHash<QString, QVariant>& control, const QByteArray &data);
    void processRecvEvent(const QString& processname, const int &infotype, const QHash<QString, QVariant>& control, const QByteArray &data);
    void ledSendDataRequested(const QByteArray &data); // 自定义信号
    void linkageSendDataRequested(const QByteArray &data); // 自定义信号
    void setLinkageMsg(const long &baudrate, const QString &portName);
public slots:
    void procCommunicationRecvEvent(const QString &lineName,const int &infoType,
                                    const QHash<QString, QVariant> &controlDomain,
                                    const QByteArray &data);
    void slot_stopTestLinkageCom();
    void slot_startChangeLinkageSerialPort();
    void slot_endChangeLinkageSerialPort();
public:
    CCommunicationManager*  m_communicationManager;
//    QThread *m_ledThread;
    QSerialPort *m_ledSerialPort;
    QSerialPort *m_linkageSerialPort;
    QThread *m_ledSendThread;
    QThread *m_ledReceiveThread;
    QThread *m_linkageThread;
    QByteArray m_receiveData;
    long m_linkageBaudrate;
private:
    CCommunicationManager*  m_printManager;
    QHash<int, int>         m_interfaceBoardPortSpeed;
    QHash<int, int>         m_currentLedStatus;
    QHash<QString, int>     m_interfaceInfo;
    QHash<QString, CProcessBase*>   m_process;
    QHash<QString, QString>         m_processCommunication;//<processname, linename>
    QList<QString>                  m_communication;/*line names list*/
    
};

#endif // CCOMMUNICATIONROUTE_H
