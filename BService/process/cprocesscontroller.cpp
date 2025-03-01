﻿#include "cprocesscontroller.h"
#include "cprocessclient.h"
#include "cprocessdistribution.h"
//#include "cprocessmqtt.h"
#include "ccommunicationroute.h"
#include "log/clog.h"
#include "struct/cmsgnull.h"
#include "struct/cmsglog.h"
#include "struct/cmsgdeviceinfo.h"
#include "struct/cmsgobjectstatus.h"
#include "struct/cmsgfirepoint.h"
#include "struct/cmsgchecktime.h"
#include "struct/cmsgevacuationgroup.h"
#include "dialog/dlginformationwindow.h"
#include "cglobal.h"
//#include "Windows.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QCoreApplication>

CProcessController::CProcessController() :
    CProcessBase("Controller"), m_isSelfCheck(false), m_isYearCheck(false), m_isMonthCheck(false),
    m_isReset(false), m_isClearFlow(false), m_isWillShutdown(false), m_shutdownTime(0),
    m_isSpeedUp(false), m_monthCheckTick(0), m_yearCheckTick(0), m_stopMonthCheckTime(0)
{    
    m_isAuto = true;
    m_isMute = false;
    m_isInputPort = false;
    m_isEmergency = false;
    m_isForceEmergency = false;
    m_isRemoteEmergency = false;
    m_isFireEmergency = false;
    m_isSimulateFireEmergency = false;
    m_isInputEmergency = false;
    m_isOutPutPort = false;
    m_isCharging = false;
    m_isMainPowerWork = true;
    m_isStandbyPowerWork = false;

    m_isFault = false;
    m_isMainPowerFault = false;
    m_isStandByPowerOff = false;
    m_isStandByPowerShort = false;
    m_isStandbyPowerUndervoltage = false;
    m_isPowerOutputFault = false;
    m_isBatteryLowVoltageFault = false;
    m_isChargerFault = false;
    m_isSystemFault = false;

    m_isForbidFaultSound = false;
    m_isForbidEmergencySound = false;
    m_isForbidMonthCheck = false;
    m_isForbidYearCheck = false;
    m_CanNumber = 4;
    m_ARTUType = 0;
    m_ARTUAddress = 1;
    m_TwinkleTime = 0;

    m_bStop = false;
    m_isStartStatus = false;
    m_nLedStatus1 = 0x01;
    m_nLedStatus2 = 0x00;
    m_nLedStatus3 = 0x00;

    QSettings setting("zsyf", "Monitor");
    m_yearCycle = setting.value("yearCycle", 365).toInt();
    m_yearDuration = setting.value("yearDuration", 30).toInt();
    m_monthCycle = setting.value("monthCycle", 30).toInt();
    m_monthDuration = setting.value("monthDuration", 30).toInt();
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(yearMonthCheck()));
    m_timer->start(1000);

    m_timerLed = new QTimer(this);
    connect(m_timerLed, SIGNAL(timeout()), this, SLOT(updateLedStatus()));

    m_timerLinkage = new QTimer(this);
    connect(m_timerLinkage, SIGNAL(timeout()), this, SLOT(slot_searchFire()));

    m_checkTime = new CMsgCheckTime();
    m_checkTime->systemStartTime = QDateTime::currentDateTime();
    m_checkTime->systemMainPowerTime = m_checkTime->systemStartTime;

    m_KeyFlagTime = new QTimer(this);
    connect(m_KeyFlagTime, SIGNAL(timeout()), this, SLOT(KeyFlagTimeStop()));
    m_KeyFlag = false;

    //ARTU模块DO状态依次复位
    m_timerARTUdoReset = new QTimer(this);
    connect(m_timerARTUdoReset, SIGNAL(timeout()), this, SLOT(slot_resetARTUdoStatus()));
    m_doNumber = 0;

    m_msgLog = new CMsgLog();
}

CProcessController::~CProcessController()
{
    QSettings setting("zsyf", "Monitor");
    setting.setValue("yearCycle", m_yearCycle);
    setting.setValue("yearDuration", m_yearDuration);
    setting.setValue("monthCycle", m_monthCycle);
    setting.setValue("monthDuration", m_monthDuration);
    delete m_timer;
    delete m_checkTime;
    delete m_timerLed;
    delete m_KeyFlagTime;
    delete m_timerLinkage;
    delete m_timerARTUdoReset;
    delete m_msgLog;
}

void CProcessController::startLinkageCom()
{
    m_timerLinkage->start(1000);
    CGlobal::instance()->m_isReceiveLinkageData = true;
}

void CProcessController::procSendEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    Q_UNUSED(infoType);
    Q_UNUSED(control);
    Q_UNUSED(data);
}

void CProcessController::KeyFlagTimeStop()
{
    m_KeyFlagTime->stop();
    m_KeyFlag = false;
}

void CProcessController::procRecvEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    Q_UNUSED(infoType);
    qDebug() << "CProcessController::procRecvEvent" << data.toHex() << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(control["communicationType"].toInt() == CT_LedCard)
    {
        CMsgObjectStatus objectStatus;
        if(data.count() < 12)
            return;
        int Ledstatus_led = data.at(0);                                    //led状态
        int Ledstatus_sound = data.at(1);                                  //声音状态
        int Ledstatus_power = data.at(2);                                  //主备电状态
        int Ledstatus_key = data.at(3);                                    //按键状态
        int Ledstatus_out = data.at(4);                                    //DO状态

        objectStatus.strdata = data.mid(5, 7).toHex(); // 从索引5 开始提取长度为 7 的字节
        CGlobal::instance()->DealEvent(NCT_ControllerParam, objectStatus.data(NCT_ControllerParam));
        if(Ledstatus_key != 0)
        {
            CMsgNull msgNull;
            //无源联动输入
            if(Ledstatus_key & 0x01)
            {
                QByteArray byte = msgNull.data(NCT_PassiveInputEmergency);
                CGlobal::instance()->DealEvent(NCT_PassiveInputEmergency, byte);
            }
            //有源联动输入
            if(Ledstatus_key & 0x02)
            {
                QByteArray byte = msgNull.data(NCT_ActiveInputEmergency);
                CGlobal::instance()->DealEvent(NCT_ActiveInputEmergency, byte);
            }
            //检测按键
            if(Ledstatus_key & 0x10)
            {

            }
            //消音按键
            if(Ledstatus_key & 0x20)
            {
                QByteArray byte = msgNull.data(NCT_Mute);
                CGlobal::instance()->DealEvent(NCT_Mute, byte);
            }
            //应急按键
            if(Ledstatus_key & 0x40)
            {
                if(m_KeyFlag == true)
                    return;
                QByteArray byte = msgNull.data(NCT_ForceEmergency);
                CGlobal::instance()->DealEvent(NCT_ForceEmergency, byte);
                m_KeyFlagTime->start(500);
                m_KeyFlag = true;
            }
        }
        //主电工作
        if(Ledstatus_power & 0x01)
        {
            m_isMainPowerWork = true;
            m_isStandbyPowerWork = false;
        }
        //备电工作
        if(Ledstatus_power & 0x02)
        {
            m_isMainPowerWork = false;
            m_isStandbyPowerWork = true;
        }
        //主电故障
        if(!m_isMainPowerWork && (Ledstatus_power & 0x20))
        {
            if(m_isMainPowerFault == false)
            {
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MainPowerFault, 1, QDateTime::currentDateTime().toTime_t());
                m_isMainPowerFault = true;
            }
        }
        //主电故障恢复
        else if(m_isMainPowerWork)
        {
            if(m_isMainPowerFault == true)
            {
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MainPowerFault, 0, QDateTime::currentDateTime().toTime_t());
                m_isMainPowerFault = false;
            }
        }
        //备电故障
        if(m_isMainPowerWork && (Ledstatus_power & 0x08))
        {
            //非备电欠压时
            if(!(Ledstatus_power & 0x04))
            {
                //备电断路
                if(Ledstatus_power & 0x40)
                {
                    if(m_isStandByPowerShort)
                    {
                        m_isStandByPowerShort = false;
                        uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerShort, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    if(m_isStandByPowerOff == false)
                    {
                        uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerOff, 1, QDateTime::currentDateTime().toTime_t());
                        m_isStandByPowerOff = true;
                    }
                }
                //备电短路
                else
                {
                    if(m_isStandByPowerOff)
                    {
                        uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerOff, 0, QDateTime::currentDateTime().toTime_t());
                        m_isStandByPowerOff = false;
                    }
                    if(m_isStandByPowerShort == false)
                    {
                        uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerShort, 1, QDateTime::currentDateTime().toTime_t());
                        m_isStandByPowerShort = true;
                    }
                }
            }
            //备电欠压
            else
            {
                if(m_isStandbyPowerUndervoltage == false)
                {
                    uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerUndervoltage, 1, QDateTime::currentDateTime().toTime_t());
                    m_isStandbyPowerUndervoltage = true;
                }
            }
        }
        else
        {
            //备电断路恢复
            if(m_isStandByPowerOff == true)
            {
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerOff, 0, QDateTime::currentDateTime().toTime_t());
                m_isStandByPowerOff = false;
            }
            //备电短路恢复
            if(m_isStandByPowerShort == true)
            {
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerShort, 0, QDateTime::currentDateTime().toTime_t());
                m_isStandByPowerShort = false;
            }
            //备电欠压恢复
            if(m_isStandbyPowerUndervoltage == true)
            {
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerUndervoltage, 0, QDateTime::currentDateTime().toTime_t());
                m_isStandbyPowerUndervoltage = false;
            }
        }
        //备电充电完成
        if((Ledstatus_power & 0x80) || m_isMainPowerFault)
        {
            m_isCharging = false;
        }
        else
        {
            //备电充电
            if(Ledstatus_power & 0x10)
            {
                m_isCharging = true;
            }
        }
        if(Ledstatus_led)      {    }
        if(Ledstatus_sound)    {    }
        if(Ledstatus_out)      {    }
    }
    else if(control["communicationType"].toInt() == CT_LinkageCard)
    {
        if(data.count() < 6)
            return;
        if(CGlobal::instance()->processController()->m_ARTUType)
        {
            int deviceAddress = static_cast<unsigned char>((data.at(0)<<8) | data.at(1));  //设备
            int loopAddress = static_cast<unsigned char>((data.at(2)<<8) | data.at(3));    //回路
            int address = static_cast<unsigned char>((data.at(4)<<8) | data.at(5));        //地址
            uploadStatus(5, CGlobal::instance()->panelAddress(), deviceAddress, loopAddress, address, OBJS_FireEmergency, true, QDateTime::currentDateTime().toTime_t());
            //426和079-KJ  DO控制
            if(CGlobal::instance()->processController()->m_ARTUType == 2 ||
                    CGlobal::instance()->processController()->m_ARTUType == 3)
            {
                QByteArray byteArray;
                byteArray.append(deviceAddress);
                byteArray.append(char(0x05));
                byteArray.append(char(((address-1)>>8)&0xFF));
                byteArray.append(char((address-1)&0xFF));
                byteArray.append(char(0xFF));
                byteArray.append(char(0X00));
                CGlobal::instance()->communicationRoute()->linkageSendData(byteArray);
            }
        }
        else
        {
            if((static_cast<unsigned char>(data.at(1)) == 0xFF)
                    && (static_cast<unsigned char>(data.at(3)) == 0xFE)
                    && (static_cast<unsigned char>(data.at(5)) == 0xFD))
            {
                //复位
            }
            else if((static_cast<unsigned char>(data.at(1)) == 0)
                    && (static_cast<unsigned char>(data.at(3)) == 0)
                    && (static_cast<unsigned char>(data.at(1)) == 0))
            {
                //无火警
            }
            else
            {
                int deviceAddress = static_cast<unsigned char>(data.at(1));  //设备
                int loopAddress = static_cast<unsigned char>(data.at(3));    //回路
                int address = static_cast<unsigned char>(data.at(5));        //地址
    //            emergency(OBJS_FireEmergency, true);
                uploadStatus(5, CGlobal::instance()->panelAddress(), deviceAddress, loopAddress, address, OBJS_FireEmergency, true, QDateTime::currentDateTime().toTime_t());
            }
        }

    }
    else if(control["communicationType"].toInt() == CT_TestCard)
    {
        if(data.isEmpty())
            return;
        CGlobal::instance()->m_linkageTestStatus |= data.at(0);
        if(CGlobal::instance()->m_linkageTestStatus & 0x03)
        {
            CMsgNull msgNull;
            QByteArray byte = msgNull.data(NCT_TestLinkageCom);
            CGlobal::instance()->DealEvent(NCT_TestLinkageCom, byte);
        }
    }
}

void CProcessController::initProcess()
{
    m_controllerStr = "控制器";
    CGlobal::instance()->processDistribution()->setMidLayerOption();
    CGlobal::instance()->processController()->reset();
//    CGlobal::instance()->processDistribution()->flashConfig(true, 0, 0, 0, 0);
    QTimer::singleShot(500, this, SLOT(selfCheck()));
}

void CProcessController::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void CProcessController::SystemSet(CMsgObjectStatus *msgObjectStatus)
{
    if(msgObjectStatus->ndata.count() < 8)
        return;
    m_isForbidFaultSound = msgObjectStatus->ndata.at(0);
    m_isForbidEmergencySound = msgObjectStatus->ndata.at(1);
    m_isForbidMonthCheck = msgObjectStatus->ndata.at(2);
    m_isForbidYearCheck = msgObjectStatus->ndata.at(3);
    m_CanNumber = msgObjectStatus->ndata.at(4);
    m_monthDuration = msgObjectStatus->ndata.at(5);
    m_yearDuration = msgObjectStatus->ndata.at(6);
    m_ARTUType = msgObjectStatus->ndata.at(7);
}

//自动模式
void CProcessController::setAutoEvacuation(const bool enable)
{
    if(m_isAuto == enable) return;
    m_isAuto = enable;
    addLog(PrintType::Other, controllerStr() + tr("设置%1疏散模式").arg(enable ? "自动" : "手动"));
}
//自检
void CProcessController::selfCheck()
{
    m_isSelfCheck = true;
    QByteArray data = m_msgNull->data(NCT_SelfCheckStart);      /*25 start self-check, data= type(25)+m_msgnull*/
    CGlobal::instance()->DealEvent(NCT_SelfCheckStart, data);   /*call callback func*/
    CGlobal::instance()->processDistribution()->selfCheck();
    if(CGlobal::isDebug()) qDebug() << "selfcheck start" << QDateTime::currentDateTime();
    m_nLedStatus1 = 0xFF;
    m_nLedStatus2 = 0x00;
    m_nLedStatus3 = 0x00;
    CGlobal::instance()->communicationRoute()->setLedStatus(m_nLedStatus1, m_nLedStatus2, m_nLedStatus3);
    QTimer::singleShot(PROCESSCONTROLLER_SELFCHECK_DURATION, this, SLOT(stopSelfCheck()));
    addLog(PrintType::Other, controllerStr() + "系统自检");
}
//结束自检
void CProcessController::stopSelfCheck()
{
    m_isSelfCheck = false;
    m_nLedStatus1 = 0x01;
    m_nLedStatus2 = 0x00;
    m_nLedStatus3 = 0x00;
    m_timerLed->start(500);
    CGlobal::instance()->communicationRoute()->setLedStatus(m_nLedStatus1, m_nLedStatus2, m_nLedStatus3);
}
//手动月检
void CProcessController::manualMonthCheck()
{
    addLog(PrintType::Other, controllerStr() + "手动月检");
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 1, QDateTime::currentDateTime().toTime_t());
    monthCheck();
}
//手动年检
void CProcessController::manualYearCheck()
{
    addLog(PrintType::Other, controllerStr() + "手动年检");
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 1, QDateTime::currentDateTime().toTime_t());
    yearCheck();
}
//年检
void CProcessController::autoYearCheck()
{
    if(m_isForbidYearCheck)
        return;
    addLog(PrintType::Other, controllerStr() + "自动年检");
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 1, QDateTime::currentDateTime().toTime_t());
    yearCheck();
}
//月检
void CProcessController::autoMonthCheck()
{
    if(m_isForbidMonthCheck)
        return;
    addLog(PrintType::Other, controllerStr() + "自动月检");
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 1, QDateTime::currentDateTime().toTime_t());
    monthCheck();
}
//消音
void CProcessController::mute(bool enable)
{
    if(m_isMute == enable) return;
    if(m_isFault && m_isStartStatus)
        enable = false;
    m_isMute = enable;
}
//年检开始
void CProcessController::yearCheck()
{
    m_isYearCheck = true;
    m_isEmergency = true;
    sendYearCheck(true);
    m_YearCheckTime = QDateTime::currentDateTime().addSecs(m_yearDuration * 60);
    CMsgObjectStatus objectStatus;
    objectStatus.nValue = m_yearDuration * 60;
    QByteArray data = objectStatus.data(NCT_YearCheckBegin);
    CGlobal::instance()->DealEvent(NCT_YearCheckBegin, data);
}
//月检开始
void CProcessController::monthCheck()
{
    m_isMonthCheck = true;
    m_isEmergency = true;
    sendMonthCheck(true);
    m_MonthCheckTime = QDateTime::currentDateTime().addSecs(m_monthDuration);
    CMsgObjectStatus objectStatus;
    objectStatus.nValue = m_monthDuration;
    QByteArray data = objectStatus.data(NCT_MonthCheckBegin);
    CGlobal::instance()->DealEvent(NCT_MonthCheckBegin, data);
}
//结束年检
void CProcessController::stopYearCheck()
{
    if(m_isYearCheck == false)
        return;
    m_isYearCheck = false;
    m_yearCheckTick = 0;
    m_isEmergency = false;
    m_bStop = false;
    sendYearCheck(false);
    QByteArray data = m_msgNull->data(NCT_YearCheckEnd);
    CGlobal::instance()->DealEvent(NCT_YearCheckEnd, data);
    addLog(PrintType::Other, controllerStr() + "年检停止");
    setSpeedUp(false);
}
//结束月检
void CProcessController::stopMonthCheck()
{
    if(m_isMonthCheck == false)
        return;
    m_isMonthCheck = false;
    m_monthCheckTick = 0;
    m_isEmergency = false;
    m_bStop = false;
    sendMonthCheck(false);
    QByteArray data = m_msgNull->data(NCT_MonthCheckEnd);
    CGlobal::instance()->DealEvent(NCT_MonthCheckEnd, data);
    addLog(PrintType::Other, controllerStr() + "月检停止");
}
//模拟加速
void CProcessController::setSpeedUp(bool enable)
{
    if(m_isSpeedUp == enable) return;
    m_isSpeedUp = enable;
    m_monthCheckTick = 0;
    m_yearCheckTick = 0;
    addLog(PrintType::Other, enable ? "模拟加速开始" : "模拟加速停止");
    QByteArray data = m_msgNull->data(enable ? NCT_StartSpeedUp : NCT_StopSpeedUp);
    CGlobal::instance()->DealEvent((enable ? NCT_StartSpeedUp : NCT_StopSpeedUp), data);
}

void CProcessController::setLedStatus(const int nLedID, const bool nLedStatus)
{
    if(isSelfCheck()) return;
    if(nLedID == LedEmergency) m_isEmergency = nLedStatus;
    if(nLedID == LedMainPower) m_isMainPowerWork = nLedStatus;
    if(nLedID == LedStandbyPower) m_isStandbyPowerWork = nLedStatus;
    if(nLedID == LedAuto) m_isAuto = nLedStatus;
    if(nLedID == LedManual) m_isAuto = nLedStatus;
    if(nLedID == LedFault) m_isFault = nLedStatus;
    if(nLedID == LedSystemFault) m_isSystemFault = nLedStatus;
    if(nLedID == LedMute) m_isMute = nLedStatus;
    updateLedStatus();
}
void CProcessController::setLedupdateTime2s()
{
    m_timerLed->stop();
    m_timerLed->start(2000);
}
void CProcessController::setLedupdateTime150ms()
{
    m_timerLed->stop();
    m_timerLed->start(150);
}
/**
 * LED: 左 zhudian 备电 系统故障 故障指示 应急启动 自动 手动 消音
 *
 * LEDSTATUS1: LED STATE(BIT7-BIT0: EMERGENCY
 * LEDSTATUS2: SOUND(BIT7-BIT6) do(bit5)
 */
void CProcessController::updateLedStatus()
{
    if(isSelfCheck())
        return;
    if(m_isMainPowerWork) m_nLedStatus1 |= 0x01; else m_nLedStatus1 &= 0xFE;
    if(m_isCharging) m_nLedStatus1 |= 0x02; else m_nLedStatus1 &= 0xFD;
    if(m_isFault || m_isMainPowerFault || m_isStandByPowerOff || m_isStandByPowerShort || m_isStandbyPowerUndervoltage)
        m_nLedStatus1 |= 0x04;
    else
        m_nLedStatus1 &= 0xFB;
    if((m_isFault || m_isMainPowerFault || m_isStandByPowerOff || m_isStandByPowerShort || m_isStandbyPowerUndervoltage) && !m_isForbidFaultSound)
        m_nLedStatus2 = 0x02;
    else
        m_nLedStatus2 &= 0xFD;
    if(m_isEmergency) {m_nLedStatus1 |= 0x08; m_nLedStatus3 |= 0x01;}else {m_nLedStatus1 &= 0xF7; m_nLedStatus3 &= 0xFE;}
    if(m_isEmergency && !m_isForbidEmergencySound) m_nLedStatus2 = 0x01;else m_nLedStatus2 &= 0xFE;
    if(m_isMute) {m_nLedStatus2 = 0;}

    CGlobal::instance()->communicationRoute()->setLedStatus(m_nLedStatus1, m_nLedStatus2, m_nLedStatus3);
}

void CProcessController::slot_searchFire()
{
    if(!CGlobal::instance()->m_isReceiveLinkageData)
        return;
    QByteArray data;
    int n = 0x00;
    int deviceAddress = ARTUAddress();
    if(m_ARTUType)
    {
        //079-KJ
        if(m_ARTUType == 2)
        {
            data.append(char(deviceAddress));
            data.append(0x02);
            data.append(n);
            data.append(n);
            data.append(n);
            data.append(0x08);
        }
        //426和079-K
        else if(m_ARTUType == 3 || m_ARTUType == 1)
        {
            data.append(char(deviceAddress));
            data.append(0x02);
            data.append(n);
            data.append(n);
            data.append(n);
            data.append(0x20);
        }
    }
    else
    {
        data.append(0x01);
        data.append(0x03);
        data.append(n);
        data.append(0xB0);
        data.append(n);
        data.append(0x03);
    }
    CGlobal::instance()->communicationRoute()->linkageSendData(data);
    CGlobal::instance()->m_isReceiveLinkageData = false;
}

void CProcessController::emergency(const short emergencyType, const bool enable)
{
    qDebug() << emergencyType << enable << "CProcessController::emergency=============================";

    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, emergencyType, enable, QDateTime::currentDateTime().toTime_t());
    if(emergencyType == OBJS_Emergency){
        m_isEmergency = enable;
    } else if(emergencyType == OBJS_RemoteEmergency){
        m_isRemoteEmergency = enable;
    } else if(emergencyType == OBJS_FireEmergency){
        m_isFireEmergency = enable;
    } else if(emergencyType == OBJS_SimulateFireEmergency){
        m_isSimulateFireEmergency = enable;
    } else if(emergencyType == OBJS_InputEmergency){
        m_isInputEmergency = enable;
    } else if(emergencyType == OBJS_ForceEmergency){
        m_isForceEmergency = enable;
    }

    bool bEmergencyStatus = isEmergencyStatus();
    setLedStatus(LedEmergency, bEmergencyStatus);
    //火警点应急不发全部启动
    if((emergencyType != OBJS_SimulateFireEmergency) && (emergencyType != OBJS_FireEmergency))
    {
        sendEmergency(bEmergencyStatus);
    }
}

void CProcessController::sendEmergency(bool enable)
{
    CGlobal::instance()->processDistribution()->emergency(enable);
    QTimer::singleShot(200, CGlobal::instance()->processDistribution(), SLOT(emergency(enable)));
    QTimer::singleShot(400, CGlobal::instance()->processDistribution(), SLOT(emergency(enable)));
}

void CProcessController::sendMonthCheck(bool enable)
{
    setLedStatus(LedEmergency, enable);
    CGlobal::instance()->processDistribution()->monthCheck(enable);
}

void CProcessController::sendYearCheck(bool enable)
{
    setLedStatus(LedEmergency, enable);
    CGlobal::instance()->processDistribution()->yearCheck(enable);
}

void CProcessController::yearMonthCheck()
{
    if(m_isMonthCheck)
    {
        if(QDateTime::currentDateTime() >= m_MonthCheckTime)
        {
            stopMonthCheck();
            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 0, QDateTime::currentDateTime().toTime_t());
        }
        else
        {
            CMsgObjectStatus objectStatus;
            objectStatus.nValue = QDateTime::currentDateTime().secsTo(m_MonthCheckTime);
            QByteArray data = objectStatus.data(NCT_MonthCheckBegin);
            CGlobal::instance()->DealEvent(NCT_MonthCheckBegin, data);
        }
        return;
    }
    if(m_isYearCheck)
    {
        if(QDateTime::currentDateTime() >= m_YearCheckTime)
        {
            stopYearCheck();
            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 0, QDateTime::currentDateTime().toTime_t());
        }
        else
        {
            CMsgObjectStatus objectStatus;
            objectStatus.nValue = QDateTime::currentDateTime().secsTo(m_YearCheckTime);
            QByteArray data = objectStatus.data(NCT_YearCheckBegin);
            CGlobal::instance()->DealEvent(NCT_YearCheckBegin, data);
        }
        return;
    }
    m_checkTime->systemCurrentTime = QDateTime::currentDateTime();


    if(!m_isSpeedUp)//正常月检年检
    {
        // 进入计时状态后的计时天数
        int days = m_checkTime->systemMainPowerTime.daysTo(m_checkTime->systemCurrentTime);
        if((days < m_monthCycle) && !m_bStop)// 小于月检周期
        {// 更新界面显示下次月检、年检提示信息
            m_checkTime->yearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle);
            m_checkTime->monthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle);
        }
        else
        {
            if(days % m_yearCycle == 0)// 满足年检条件
            {
                QDateTime tmpYearTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle)));
                if(m_checkTime->yearCheckTime <= m_checkTime->systemCurrentTime)
                {
                    if(m_checkTime->lastYearCheckTime.date() < m_checkTime->systemCurrentTime.date())
                    {
                        if(!m_isMonthCheck)// 当前不存在月检，则进行年检
                        {
                            m_checkTime->lastYearCheckTime = m_checkTime->systemCurrentTime;
                            m_checkTime->yearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
                            autoYearCheck();
                        }
                    }
                }
                else
                {
                    if(!m_isYearCheck && (tmpYearTime >= m_checkTime->systemCurrentTime)) m_checkTime->yearCheckTime = tmpYearTime;
                }
            }
            else
            {
                if(m_checkTime->yearCheckTime <= m_checkTime->systemCurrentTime){//更新下次年检时间
                    m_checkTime->yearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
                }
            }

            if(days % m_monthCycle == 0)//满足月检条件
            {
                QDateTime tmpMonthTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * (int)(days / m_monthCycle));
                if(m_checkTime->monthCheckTime <= m_checkTime->systemCurrentTime)
                {
                    if(m_checkTime->lastMonthCheckTime < m_checkTime->systemCurrentTime)
                    {
                        if(!m_isYearCheck)//当前不存在年检，则进行月检
                        {
                            m_checkTime->lastMonthCheckTime = m_checkTime->systemCurrentTime;
                            m_checkTime->monthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * ((int)(days / m_monthCycle) + 1));
                            autoMonthCheck();
                        }
                    }
                }
                else
                {
                    if(!m_isMonthCheck && (tmpMonthTime >= m_checkTime->systemCurrentTime)) {
                        m_checkTime->monthCheckTime = tmpMonthTime;
                    }
                }
            }
            else
            {
                if(m_checkTime->monthCheckTime <= m_checkTime->systemCurrentTime){//更新下次月检时间
                    m_checkTime->monthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * ((int)(days / m_monthCycle) + 1));
                }
            }
        }
        m_checkTime->speedMonthCheckTime = m_checkTime->monthCheckTime;
        m_checkTime->speedYearCheckTime = m_checkTime->yearCheckTime;
        m_CheckTick = 0;
    }
    else//模拟加速
    {
        if(!m_isMonthCheck && !m_isYearCheck)
        {
            m_CheckTick++;
            m_checkTime->systemCurrentTime = QDateTime::currentDateTime().addDays(m_CheckTick);
        }
        int days = m_checkTime->systemMainPowerTime.daysTo(m_checkTime->systemCurrentTime);
        if(days % m_yearCycle == 0)// 满足年检条件
        {
            QDateTime tmpYearTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle)));
            if(m_checkTime->speedYearCheckTime <= m_checkTime->systemCurrentTime)
            {
                if(m_checkTime->speedLastYearCheckTime.date() < m_checkTime->systemCurrentTime.date())
                {
                    if(!m_isMonthCheck)// 当前不存在月检，则进行年检
                    {
                        m_checkTime->speedLastYearCheckTime = m_checkTime->systemCurrentTime;
                        m_checkTime->speedYearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
                        autoYearCheck();
                    }
                }
            }
            else
            {
                if(!m_isYearCheck && (tmpYearTime >= m_checkTime->systemCurrentTime)) m_checkTime->speedYearCheckTime = tmpYearTime;
            }
        }
        else
        {
            if(m_checkTime->speedYearCheckTime <= m_checkTime->systemCurrentTime){//更新下次年检时间
                m_checkTime->speedYearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
            }
        }

        if(days % m_monthCycle == 0)//满足月检条件
        {
            QDateTime tmpMonthTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * (int)(days / m_monthCycle));
            if(m_checkTime->speedMonthCheckTime <= m_checkTime->systemCurrentTime)
            {
                if(m_checkTime->speedLastMonthCheckTime < m_checkTime->systemCurrentTime)
                {
                    if(!m_isYearCheck)//当前不存在年检，则进行月检
                    {
                        m_checkTime->speedLastMonthCheckTime = m_checkTime->systemCurrentTime;
                        m_checkTime->speedMonthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * ((int)(days / m_monthCycle) + 1));
                        autoMonthCheck();
                    }
                }
            }
            else
            {
                if(!m_isMonthCheck && (tmpMonthTime >= m_checkTime->systemCurrentTime)) {
                    m_checkTime->speedMonthCheckTime = tmpMonthTime;
                }
            }
        }

    }

    if(m_isMonthCheck || m_isYearCheck)
        return;
    QByteArray data = m_checkTime->data(NCT_CheckTimeSync);
    CGlobal::instance()->DealEvent(NCT_CheckTimeSync, data);
}


//信息记录页码
void CProcessController::RecordPage(CMsgObjectStatus *msgObjectStatus)
{
    if(msgObjectStatus->ndata.count() < 3)
        return;
    bool customMode = msgObjectStatus->ndata.at(0);
    bool isStatusTab = msgObjectStatus->ndata.at(1);
    int page = msgObjectStatus->ndata.at(2);
    //查询模式：1为自定义查询，有故障或应急时显示11条记录，否则显示17条;0为普通查询，有故障或应急时显示12条记录，否则显示18条
    if(customMode)
    {
        if(isStatusTab)
            CGlobal::instance()->setRecordNumber(11);
        else
            CGlobal::instance()->setRecordNumber(17);
    }
    else
    {
        if(isStatusTab)
            CGlobal::instance()->setRecordNumber(12);
        else
            CGlobal::instance()->setRecordNumber(18);
    }

    CGlobal::instance()->setRecordPage(page);
}

void CProcessController::reset()
{
    m_isReset = true;
    m_isMonthCheck = false;
    m_isYearCheck = false;
    m_isMute = false;
    m_isInputPort = false;
    m_isCharging = false;
    m_isEmergency = false;
    m_isForceEmergency = false;
    m_isRemoteEmergency = false;
    m_isSimulateFireEmergency = false;
    m_isFireEmergency = false;
    m_isInputEmergency = false;
    m_isOutPutPort = false;
    m_isMainPowerWork = true;
    m_isStandbyPowerWork = false;

    m_isFault = false;
    m_isMainPowerFault = false;
    m_isStandByPowerOff = false;
    m_isStandByPowerShort = false;
    m_isStandbyPowerUndervoltage = false;
    m_isPowerOutputFault = false;
    m_isBatteryLowVoltageFault = false;
    m_isChargerFault = false;
    m_isStartStatus = false;
    m_isSystemFault = false;
//    m_isSpeedUp = false;
    m_monthCheckTick = 0;
    m_yearCheckTick = 0;
    m_bStop = false;

    m_isSystemFault = true;
    CGlobal::instance()->processClient()->clientReset();
    CGlobal::instance()->processDistribution()->reset();
    if(ARTUType())
    {
       m_timerARTUdoReset->start(100);
    }
    setAutoEvacuation(m_isAuto);
    setPowerWork(true, false);
    addLog(PrintType::Other, controllerStr() + "系统复位");
    m_isReset = false;
}

void CProcessController::slot_resetARTUdoStatus()
{
    if(!CGlobal::instance()->m_isReceiveLinkageData)
        return;
    int number = 0;
    if(ARTUType() == 2)
        number = 8;
    else if(ARTUType() == 3)
        number = 16;
    if(m_doNumber < number)
    {
        int deviceAddress = ARTUAddress();
        QByteArray byteArray;
        byteArray.append(deviceAddress);
        byteArray.append(char(0x05));
        byteArray.append(char((m_doNumber>>8)&0xFF));
        byteArray.append(char(m_doNumber&0xFF));
        byteArray.append(char(0x00));
        byteArray.append(char(0X00));
        CGlobal::instance()->communicationRoute()->linkageSendData(byteArray);
        m_doNumber++;
    }
    else
    {
        m_doNumber = 0;
        m_timerARTUdoReset->stop();
    }
}


void CProcessController::addLog(int type, const QString &title)
{
    m_msgLog->User = "系统";
    m_msgLog->Time = QDateTime::currentDateTime();
    m_msgLog->Remark = title;
    if(type == PrintType::Start)
        m_msgLog->Type = "应急";
    else if(type == PrintType::Fault)
        m_msgLog->Type = "故障";
    else if(type == PrintType::Error)
        m_msgLog->Type = "错误";
    else
        m_msgLog->Type = "其它";
    CGlobal::instance()->log()->addLog(m_msgLog);
}

void CProcessController::addLog(CMsgLog* msgLog)
{
    int type = 0;
    CGlobal::instance()->log()->addLog(msgLog);
    if(msgLog->Type == "应急")
        type = PrintType::Start;
    else if(msgLog->Type == "故障")
        type = PrintType::Fault;
    else if(msgLog->Type == "错误")
        type = PrintType::Error;
    else
        type = PrintType::Other;
}




void CProcessController::setPowerWork(bool isMainPower, bool isStandbyPower)
{
    m_isMainPowerWork = isMainPower;
    m_isStandbyPowerWork = isStandbyPower;
}



bool CProcessController::hasStandbyPowerFault() const
{
    if(m_isStandByPowerOff || m_isStandByPowerShort || m_isStandbyPowerUndervoltage || m_isChargerFault || m_isBatteryLineFault || m_isBatteryLowVoltageFault)
        return true;
    else
        return false;
}

void CProcessController::setCheck(int yearCycle, int yearDuration, int monthCycle, int monthDuration)
{
    m_yearCycle = yearCycle;
    m_yearDuration = yearDuration;
    m_monthCycle = monthCycle;
    m_monthDuration = monthDuration;
//    addLog(CPrint::Other, controllerStr() + tr("Set check"));
}

void CProcessController::manualFire(const QString &address)
{
    addLog(PrintType::Other, controllerStr() + tr("手动火警%1").arg(address));
    CGlobal::instance()->processController()->emergency(OBJS_FireEmergency, true);
}

