#include <QDebug>
#include <QVector>
#include <QTextCodec>
#include <QThread>
#include <QByteArray>
#include "struct/cmsgnull.h"
#include "cprocessbase.h"
#include "cprocesscontroller.h"
#include "ccommunicationroute.h"
#include "ccommunicationmanager.h"
#include "cglobal.h"
#include "cserialport.h"
#include "ccommunicationled.h"
#include "ccommunicationlinkage.h"

CCommunicationRoute::CCommunicationRoute(QObject *parent) :
    QObject(parent), m_communicationManager(NULL)
{
    initCommunicationManager();
//    createCommunication(CT_LedCard);

    // 创建led串口
    m_ledSerialPort = makeSerialPort("/dev/ttyUSB0", QSerialPort::Baud19200);

    // 创建led发送线程
    m_ledSendThread = new QThread;
    m_ledSerialSender = new ledSerialSender(m_ledSerialPort);
    m_ledSerialSender->moveToThread(m_ledSendThread);
    // 启动led发送线程
    m_ledSendThread->start();
    connect(this, &CCommunicationRoute::ledSendDataRequested, m_ledSerialSender, &ledSerialSender::sendData);

    // 创建led接收线程
    m_ledReceiveThread = new QThread;
    m_ledSerialReceiver = new ledSerialReceiver(m_ledSerialPort);
    m_ledSerialReceiver->moveToThread(m_ledReceiveThread);
    // 启动led接收线程
    m_ledReceiveThread->start();
    connect(m_ledSerialReceiver, &ledSerialReceiver::dataReceived, this, &CCommunicationRoute::slot_dataProcessed);

//    // 创建led线程
//    m_ledThread = new QThread;
//    ledSerialWorker *ledSerial = new ledSerialWorker(ledSerialPort);
//    ledSerial->moveToThread(m_ledThread);
//    // 启动led线程
//    m_ledThread->start();
//    // 连接led数据发送信号
//    connect(this, &CCommunicationRoute::ledSendDataRequested, ledSerial, &ledSerialWorker::sendData);
//    // 连接led数据接收信号
//    connect(ledSerial, &ledSerialWorker::ledDataReceived, this, &CCommunicationRoute::onDataProcessed);

    // 创建linkage串口
    m_linkageSerialPort = makeSerialPort("/dev/ttyUSB1", QSerialPort::Baud19200);

    // 创建linkage线程
    m_linkageThread = new QThread;
    m_linkageSerial = new linkageSerialWorker(m_linkageSerialPort);
    m_linkageSerial->moveToThread(m_linkageThread);
    // 启动linkage线程
    m_linkageThread->start();
    // 连接linkage数据发送信号
    connect(this, &CCommunicationRoute::linkageSendDataRequested, m_linkageSerial, &linkageSerialWorker::sendData);
    // 连接linkage数据接收信号
    connect(m_linkageSerial, &linkageSerialWorker::linkageDataReceived, this, &CCommunicationRoute::slot_dataProcessed);
    // 连接交换灯键和火报串口信号
    connect(m_linkageSerial, &linkageSerialWorker::switchLedAndLinkageSerial, this, &CCommunicationRoute::slot_switchLedAndLinkageSerial);

    //    createCommunication(CT_LinkageCard);
    createCommunication(CT_TestCard);
}

CCommunicationRoute::~CCommunicationRoute()
{
    delete m_communicationManager;
    delete m_ledSerialPort;
    delete m_ledSerialSender;
    delete m_ledSerialReceiver;
    delete m_linkageSerialPort;
    delete m_linkageSerial;
    // 停止 LED 发送线程
    if (m_ledSendThread) {
        m_ledSendThread->quit();          // 请求线程退出
        m_ledSendThread->wait();          // 等待线程结束
        delete m_ledSendThread;           // 删除线程对象
    }

    // 停止 LED 接收线程
    if (m_ledReceiveThread) {
        m_ledReceiveThread->quit();       // 请求线程退出
        m_ledReceiveThread->wait();       // 等待线程结束
        delete m_ledReceiveThread;        // 删除线程对象
    }

    // 停止 Linkage 线程
    if (m_linkageThread) {
        m_linkageThread->quit();          // 请求线程退出
        m_linkageThread->wait();          // 等待线程结束
        delete m_linkageThread;           // 删除线程对象
    }
}

QSerialPort* CCommunicationRoute::makeSerialPort(QString serialName, long baudrate)
{
    QSerialPort *SerialPort = new QSerialPort(serialName);
    SerialPort->setBaudRate(baudrate);
    SerialPort->setDataBits(QSerialPort::Data8);
    SerialPort->setParity(QSerialPort::NoParity);
    SerialPort->setStopBits(QSerialPort::OneStop);
    SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    return SerialPort;
}

void CCommunicationRoute::slot_dataProcessed(const int &type, const QByteArray &data)
{
    QHash<QString, QVariant> controlDomain;
    controlDomain.insert("communicationType",type);
    CGlobal::instance()->processController()->procRecvEvent(0, controlDomain, data);
}

void CCommunicationRoute::initCommunicationManager()
{
    if(!m_communicationManager)
        m_communicationManager = new CCommunicationManager();
    connect(m_communicationManager, SIGNAL(sig_ReciveData(QString,int,QHash<QString,QVariant>,QByteArray)), this, SLOT(procCommunicationRecvEvent(QString,int,QHash<QString,QVariant>,QByteArray)));}

bool CCommunicationRoute::createRoute(CProcessBase *process, const int type, const QHash<QString, QVariant> &control)
{
    QString processName = process->name();
    QString communication = createCommunication(type);
    if(communication.isEmpty())
        return false;
    if(isCommunicationLine(type))
    {
        int port = control.value(INTERFACEBOARD_PORT_NUMBER_STR, QVariant(0)).toInt();
        if(!process->addCardPort(port))
            return false;
        m_communicationManager->setTimeOut(communication, INTERFACEBOARD_TIMEOUT);
        int speed = control.value(INTERFACEBOARD_PORT_SPEED_STR, QVariant(0)).toInt();
        m_interfaceBoardPortSpeed.insert(port, speed);
        if(type == CT_FASCard){
            m_interfaceInfo.insert(INTERFACEBOARD_COMMPNYCODE, control.value(INTERFACEBOARD_COMMPNYCODE, QVariant(0)).toInt());
            m_interfaceInfo.insert(INTERFACEBOARD_PANEL, control.value(INTERFACEBOARD_PANEL, QVariant(0)).toInt());
        }
    }
    if(!m_process.contains(processName)){
        m_process.insert(processName, process);
        m_processCommunication.insert(processName, communication);
        connect(this, SIGNAL(processRecvEvent(QString,int,QHash<QString,QVariant>,QByteArray)), process, SLOT(recvEvent(QString,int,QHash<QString,QVariant>,QByteArray)));
    } else{
        if(!m_processCommunication.values(processName).contains(communication))
            m_processCommunication.insertMulti(processName, communication);
    }
    return true;
}

////create communication line
QString CCommunicationRoute::createCommunication(const int type)
{
    QString communicationName;
    communicationName = getCommunicationName(type);
    if(!communicationName.isEmpty() && m_communicationManager)
    {
        if(!m_communication.contains(communicationName))
        {
            if(!m_communicationManager->createCommunicationLine(communicationName, type))
                return QString();
            m_communication.append(communicationName);
        }
    }
    return communicationName;
}

void CCommunicationRoute::openCommunication()
{
    if(CGlobal::isDebug()) qDebug() << "CCommunicationRoute::openCommunication()" << m_communication.size();
    for(int i = 0; i < m_communication.size(); ++i) /*灯 电源 can1 can2*/
    {
        m_communicationManager->open(m_communication.at(i));
    }
}

void CCommunicationRoute::closeCommunication()
{
    for(int i = 0; i < m_communication.size(); ++i)
    {
        m_communicationManager->close(m_communication.at(i));
    }
}

void CCommunicationRoute::setDebug(const bool enable)
{
    m_communicationManager->setDebugOut(enable);
}

void CCommunicationRoute::canSendDataClear()
{
    m_communicationManager->canSendDataClear();
}

void CCommunicationRoute::canReceiveDataClear()
{
    m_communicationManager->canReceiveDataClear();
}

void CCommunicationRoute::setLedStatus(const int ledStatus1, const int ledStatus2, const int ledStatus3)
{
    emit ledSendDataRequested(ledStatus1, ledStatus2, ledStatus3);
    if(!CGlobal::instance()->m_isSerialportNameSeted)
    {
        char sum = 0;
        QByteArray byteArray;
        byteArray.append(char(0x55));
        byteArray.append(char(0x13));
        byteArray.append(char(ledStatus1));
        byteArray.append(char(0));
        byteArray.append(char(0));
        byteArray.append(char(0));
        byteArray.append(char(ledStatus2));
        byteArray.append(char(ledStatus3));
        byteArray.append(char(0));
        byteArray.append(char(0));
        byteArray.append(char(0));
        for (int ix = 0; ix < byteArray.size(); ix++)
            sum += byteArray.at(ix);
        byteArray.append(sum);
        emit linkageSendDataRequested(byteArray);
    }
}

void CCommunicationRoute::testLinkageCom()
{
    //测试USB火报通讯口发送（中间层火报接口接收）
    m_communicationManager->setTestLinkageCom(true);
    QTimer::singleShot(5000, this, SLOT(slot_stopTestLinkageCom()));
    QString communicationLinkage = getCommunicationName(CT_LinkageCard);
    if(communicationLinkage.isEmpty())
        return;
    if(communicationIsActive(communicationLinkage))
    {
        QString linkagelinename = "linkagecard";
        if(!m_communicationManager->m_hashCommunicationLine.contains(linkagelinename)) return;
        CSerialPort* pSerialPort_linkage = m_communicationManager->m_hashCommunicationLine[linkagelinename]->pSerialPort;
        QString testlinename = "testcard";
        if(!m_communicationManager->m_hashCommunicationLine.contains(testlinename)) return;
        CSerialPort* pSerialPort_test = m_communicationManager->m_hashCommunicationLine[testlinename]->pSerialPort;
        if(pSerialPort_test && pSerialPort_linkage)
        {
            if(!pSerialPort_test->serialPort()->open(QIODevice::ReadWrite))
            {
                for(int i=2;i<5;i++)
                {
                    pSerialPort_test->serialPort()->close();
                    pSerialPort_test->serialPort()->setPortName("/dev/ttyUSB"+ QString::number(i));
                    //linkagecard的波特率
                    BaudRateType baudRate = pSerialPort_linkage->serialPort()->baudRate();
                    pSerialPort_test->serialPort()->setBaudRate(baudRate);
                    QObject::connect(pSerialPort_test->serialPort(), SIGNAL(readyRead()), pSerialPort_test->serialPort(), SLOT(doReciveData()));
                    if(pSerialPort_test->serialPort()->open(QIODevice::ReadWrite))
                        break;
                }
            }
            char sum = 0;
            QByteArray byteArray;
            byteArray.append(char(0x55));
            byteArray.append(char(0x13));
            byteArray.append(char(0xFF));
            byteArray.append(char(0xFF));
            byteArray.append(char(0xFF));
            for (int ix = 0; ix < byteArray.size(); ix++)
                sum += byteArray.at(ix);
            byteArray.append(sum);

            pSerialPort_linkage->sendData(byteArray);

            QString  data = "FasTestUSBSendData:  " + byteArray.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
            QFile file("/home/xfss/root/logfile/FasTestData.txt");

            if (file.open(QIODevice::Append | QIODevice::Text))
            {
                QTextStream stream(&file);
                stream << data << '\n' << '\n';
                file.close();
            }
        }
    }
}

void CCommunicationRoute::slot_stopTestLinkageCom()
{
    m_communicationManager->setTestLinkageCom(false);
}

void CCommunicationRoute::linkageSendData(QByteArray data)
{
    QByteArray byteArray;
    byteArray.append(data);
    // 计算 CRC 值
    // 获取指向数据的指针和长度
    const unsigned char *dataPtr = reinterpret_cast<const unsigned char *>(data.constData());
    unsigned short length = static_cast<unsigned short>(data.size());
    // 调用 CRC 函数
    unsigned short crc = CGlobal::instance()->usMBCRC16(dataPtr, length);
    byteArray.append(static_cast<char>(crc & 0xFF));        // 低字节
    byteArray.append(static_cast<char>((crc >> 8) & 0xFF)); // 高字节
    emit linkageSendDataRequested(byteArray);

    QString  str = "FasSendData:  " + byteArray.toHex() + "   " + QString::number(m_linkageSerialPort->baudRate()) + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QFile file("/home/xfss/root/logfile/FasSendData.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << str << '\n' << '\n';
        file.close();
    }
    QString filePath = "/home/xfss/root/logfile/FasSendData.txt";
    CGlobal::instance()->manageLogFile(filePath, 1024 * 1024);
}

//设置火报通讯波特率
void CCommunicationRoute::setLinkageBaudRate(QString baudrateString)
{
    //停止发送数据
    CGlobal::instance()->processController()->stopLinkageCom();
    m_linkageBaudrate = baudrateString.toLong(); // 将QString转换为整数
    //延时修改火报通讯波特率
    QTimer::singleShot(1000, this, SLOT(slot_ChangeLinkageSerialPort()));
}
//开始修改火报通讯波特率
void CCommunicationRoute::slot_ChangeLinkageSerialPort()
{
    disconnect(m_linkageSerialPort, &QSerialPort::readyRead, m_linkageSerial, &linkageSerialWorker::readData);

    // 如果串口已打开，则先关闭
    if (m_linkageSerialPort->isOpen()) {
        m_linkageSerialPort->close();
    }
    m_linkageSerialPort->setBaudRate(m_linkageBaudrate);
    m_linkageSerialPort->setPortName(m_linkageSerialPort->portName());
    if(m_linkageSerialPort->open(QIODevice::ReadWrite))
    {
        connect(m_linkageSerialPort, &QSerialPort::readyRead, m_linkageSerial, &linkageSerialWorker::readData);
    }
    CGlobal::instance()->processController()->startLinkageCom();
}

//交换灯键和火报串口
void CCommunicationRoute::slot_switchLedAndLinkageSerial(const QByteArray &data)
{
    disconnect(m_linkageSerialPort, &QSerialPort::readyRead, m_linkageSerial, &linkageSerialWorker::readData);
    disconnect(m_ledSerialPort, &QSerialPort::readyRead, m_ledSerialReceiver, &ledSerialReceiver::readData);
    long ledBaudRate = m_ledSerialPort->baudRate();
    long linkageBaudRate = m_linkageSerialPort->baudRate();
    QString ledPortName = m_ledSerialPort->portName();
    QString linkagePortName = m_linkageSerialPort->portName();
    QString  log = "oldledPortName:  " + ledPortName + " "
                    + QString::number(ledBaudRate) + "\n" +
                    "oldlinkagePortName:  " + linkagePortName + " "
                    + QString::number(linkageBaudRate) + "\n" +
                    QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
    // 如果串口已打开，则先关闭
    if (m_linkageSerialPort->isOpen()) {
        m_linkageSerialPort->close();
    }
    // 如果串口已打开，则先关闭
    if (m_ledSerialPort->isOpen()) {
        m_ledSerialPort->close();
    }
    m_linkageSerialPort->setBaudRate(ledBaudRate);
    m_linkageSerialPort->setPortName(ledPortName);
    m_ledSerialPort->setBaudRate(linkageBaudRate);
    m_ledSerialPort->setPortName(linkagePortName);
    if(m_linkageSerialPort->open(QIODevice::ReadWrite))
    {
        log = log + "linkage串口重新打开成功" + "\n";
        connect(m_linkageSerialPort, &QSerialPort::readyRead,
                m_linkageSerial, &linkageSerialWorker::readData);
    }
    else
    {
        log = log + "linkage串口重新打开失败：" + m_linkageSerialPort->errorString() + "\n";
    }
    if(m_ledSerialPort->open(QIODevice::ReadWrite))
    {
        log = log + "led串口重新打开成功" + "\n";
        connect(m_ledSerialPort, &QSerialPort::readyRead,
                m_ledSerialReceiver, &ledSerialReceiver::readData);
    }
    else
    {
        log = log + "led串口重新打开失败：" + m_ledSerialPort->errorString() + "\n";
    }
    log = log + "newledPortName:  " + m_ledSerialPort->portName() + " "
            + QString::number(m_ledSerialPort->baudRate()) + "\n" +
            "newlinkagePortName:  " + m_linkageSerialPort->portName() + " "
            + QString::number(m_linkageSerialPort->baudRate()) + "\n"
            + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
    QFile file("/home/xfss/root/logfile/portName.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << log << '\n' << '\n';
        file.close();
    }
    CGlobal::instance()->m_isSerialportNameSeted = true;
    //灯键接收数据处理
    slot_dataProcessed(CT_LedCard, data);
}


////get line name from line type
QString CCommunicationRoute::getCommunicationName(const int type)
{
    QString name;
    switch(type)
    {
    case CT_LinkageCard:
        name = "linkagecard";
        break;
    case CT_LedCard:
        name = "ledcard";
        break;
    case CT_CanCard1:
        name = "cancard1";
        break;
    case CT_CanCard2:
        name = "cancard2";
        break;
    case CT_FASCard:
        name = "fascard";
        break;
    case CT_TestCard:
        name = "testcard";
        break;
    default:
        name = "Client";
        break;
    }
    return name;
}

int CCommunicationRoute::getCommunicationType(const QString lineName)
{
    if(lineName.toLower() == "linkagecard") return CT_LinkageCard;
    else if(lineName.toLower() == "ledcard") return CT_LedCard;
    else if(lineName.toLower() == "cancard1") return CT_CanCard1;
    else if(lineName.toLower() == "cancard2") return CT_CanCard2;
    else if(lineName.toLower() == "testcard") return CT_TestCard;
    else if(lineName.toLower() == "fascard") return CT_FASCard;
    else return CT_NoCard;
}

////line type is true line
bool CCommunicationRoute::isCommunicationLine(const int type) const
{
    if(type == CT_CanCard1 || type == CT_CanCard2 || type == CT_LedCard || type == CT_LinkageCard
           || type == CT_TestCard || type == CT_FASCard)
        return true;
    else
        return false;
}

bool CCommunicationRoute::communicationIsActive(const QString &name) const
{
    return m_communicationManager->isActive(name);
}


QString CCommunicationRoute::canNameByAdd(int canportAdd)
{
    if(canportAdd == 3 || canportAdd == 4 || canportAdd == 7 || canportAdd == 8)
        return "cancard1";
    else if(canportAdd == 5 || canportAdd == 6 || canportAdd == 9 || canportAdd == 10)
        return "cancard2";
    return "";
}

int CCommunicationRoute::getIntFromByte(QByteArray data, int start, int len)
{
    if(data.size() < start+len) return 0;
    return (*(unsigned int*)data.mid(start,len).data())&0x0000ffff;;
}

////send data to process
void CCommunicationRoute::procCommunicationRecvEvent(const QString &linename, const int &infoType, const QHash<QString, QVariant> &controlDomain, const QByteArray &data)
{
//    if(CGlobal::isDebug()) qDebug() << "CCommunicationRoute::procCommunicationRecvEvent=====" << linename << infoType << data.toHex();
    if(m_communication.contains(linename))
    {
        QList<QString> processList = m_processCommunication.keys(linename);
        for(int i = 0; i < processList.size(); ++i)
            emit processRecvEvent(processList.at(i), infoType, controlDomain, data);
    }
}
