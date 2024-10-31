#include "cglobal.h"
#include "cprocessmqtt.h"
#include <QtMqtt/QMqttClient>

CprocessMqtt::CprocessMqtt()
{
    m_mqttClient = new QMqttClient(this);
    connect(m_mqttClient, SIGNAL(stateChanged(QMqttClient::ClientState)), this, SLOT(updateLogStateChange(QMqttClient::ClientState)));
    connect(m_mqttClient, SIGNAL(disconnected()), this, SLOT(brokerDisconnected()));
    connect(m_mqttClient, SIGNAL(messageReceived(QByteArray, QMqttTopicName)), this, SLOT(handleMessageReceived(QByteArray, QMqttTopicName)));
}

CprocessMqtt::~CprocessMqtt()
{
    m_mqttClient->disconnectFromHost();
    delete m_mqttClient;
}

void CprocessMqtt::connectToBroker(const QString &host, quint16 port)
{
    m_mqttClient->setHostname(host);
    m_mqttClient->setPort(port);
    m_mqttClient->connectToHost();
}

void CprocessMqtt::brokerDisconnected()
{
    qDebug() << "MQTT broker disconnected";
}

void CprocessMqtt::updateLogStateChange(QMqttClient::ClientState state)
{
    switch (state)
    {
        case QMqttClient::Disconnected:
            qDebug() << "MQTT client disconnected";
            break;
        case QMqttClient::Connecting:
            qDebug() << "MQTT client connecting";
            break;
        case QMqttClient::Connected:
            qDebug() << "MQTT client connected";
            break;

//        case QMqttClient::Disconnecting:
//            qDebug() << "MQTT client disconnecting";
//            break;
    }
}

void CprocessMqtt::subscribeToTopic(const QString &topic)
{
    if (m_mqttClient->state() == QMqttClient::Connected)
    {
        QMqttSubscription *subscription = m_mqttClient->subscribe(topic);
        if (subscription)
        {
            connect(subscription, SIGNAL(messageReceived(QByteArray, QMqttTopicName)),
                                this, SLOT(handleMessageReceived(QByteArray, QMqttTopicName)));
        }
        else
        {
            qDebug() << "Failed to subscribe to topic:" << topic;
        }
    } else {
        qDebug() << "MQTT client is not connected.";
    }
}

void CprocessMqtt::publishMessage(const QString &topic, const QByteArray &message)
{
    if (m_mqttClient->state() == QMqttClient::Connected) {
        m_mqttClient->publish(topic, message);
    } else {
        qDebug() << "MQTT client is not connected.";
    }
}

void CprocessMqtt::handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" Received Topic: ")
                + topic.name()
                + QLatin1String(" Message: ")
                + message
                + QLatin1Char('\n');
    qDebug() << content;
}
