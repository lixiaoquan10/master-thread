#include "ccommunicationlinkage.h"

linkageSerialWorker::linkageSerialWorker(QSerialPort *port) : m_linkageSerialPort(port)
{
    if(!m_linkageSerialPort)
        return;
    if(m_linkageSerialPort->open(QIODevice::ReadWrite))
    {
        connect(m_linkageSerialPort, &QSerialPort::readyRead, this, &linkageSerialWorker::readData);
        connect(m_linkageSerialPort, &QSerialPort::errorOccurred, this, &linkageSerialWorker::onErrorOccurred);
    }
}

linkageSerialWorker::~linkageSerialWorker()
{
    delete m_linkageSerialPort;
    m_linkageSendDataList.clear();
}

void linkageSerialWorker::sendData(const QByteArray &data)
{
    QMutexLocker locker(&m_mutex); // 加锁
    m_linkageSendDataList.append(data);  // 将新数据加入队列
    if (m_linkageSendDataList.isEmpty())
    {
//            qDebug() << "No data to send.";
        return;
    }
    if (m_linkageSerialPort->isOpen())
    {
        m_sendingInProgress = true;
        QByteArray sendData = m_linkageSendDataList.takeFirst();  // 取出并移除第一条数据
        qint64 bytesWritten = m_linkageSerialPort->write(sendData);
        m_sendingInProgress = false;
        if (bytesWritten == -1)
        {
//                qDebug() << "Failed to write data:" << m_serialPort->errorString();
        } else {
//                qDebug() << "Data sent, bytes written:" << bytesWritten;
        }
    } else {
//            qDebug() << "Serial port is not open.";
    }
}

void linkageSerialWorker::readData()
{
    QMutexLocker locker(&m_mutex); // 加锁
    QByteArray data = m_linkageSerialPort->readAll();
//        qDebug() << "Data received:" << data;
    if(data.isEmpty())
        return;
    m_linkageReceiveData.append(data);

    while (m_linkageReceiveData.size() >= 47)
    {
        // 检查开头是否为 0x55 0x13
        if (m_linkageReceiveData.at(0) == 0x55 && m_linkageReceiveData.at(1) == 0x13)
        {

            QByteArray reciveData = m_linkageReceiveData.mid(0, 47);
            m_linkageReceiveData.remove(0, 47); // 从索引 0 开始删除 47 个字节
            char sum = 0;
            for (int ix = 0; ix < 46; ix++)
                sum += reciveData.at(ix);
            if (reciveData.at(46) != sum) {
                reciveData.clear();
                return;
            }
            QByteArray tmpByteArray;
            tmpByteArray.append(reciveData.at(2));
            tmpByteArray.append(reciveData.at(6));
            tmpByteArray.append(reciveData.at(7));
            tmpByteArray.append(reciveData.at(9));
            tmpByteArray.append(reciveData.at(10));
            tmpByteArray.append(reciveData.at(14));
            tmpByteArray.append(reciveData.at(15));
            tmpByteArray.append(reciveData.at(16));
            tmpByteArray.append(reciveData.at(17));
            tmpByteArray.append(reciveData.at(18));
            tmpByteArray.append(reciveData.at(19));
            tmpByteArray.append(reciveData.at(20));
            reciveData.clear();
            emit ledDataReceived(tmpByteArray);  // 发送接收到的数据
        } else {
            // 删除开头无效数据
            m_linkageReceiveData.remove(0, 1); // 删除第一个字节
        }
    }
}

void linkageSerialWorker::setLinkageMsg(const long &baudrate, const QString &portName)
{
    // 停止接收数据
    disconnect(m_linkageSerialPort, &QSerialPort::readyRead, this, &linkageSerialWorker::readData);

    // 如果串口已打开，则先关闭
    if (m_linkageSerialPort->isOpen()) {
        m_linkageSerialPort->close();
    }
    m_linkageSerialPort->setBaudRate(baudrate);
    m_linkageSerialPort->setPortName(portName);
    if(m_linkageSerialPort->open(QIODevice::ReadWrite))
    {
        connect(m_linkageSerialPort, &QSerialPort::readyRead, this, &linkageSerialWorker::readData);
        connect(m_linkageSerialPort, &QSerialPort::errorOccurred, this, &linkageSerialWorker::onErrorOccurred);
    }
    emit changeLinkageComplete();
}

void linkageSerialWorker::onErrorOccurred(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::NoError) {
//            qDebug() << "Serial port error occurred:" << m_serialPort->errorString();
    }
}

