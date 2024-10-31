#ifndef CPROCESSMQTT_H
#define CPROCESSMQTT_H
#include <QCoreApplication>
#include <QtMqtt/QMqttClient>

//const std::string SERVER_ADDRESS("tcp://broker.hivemq.com:1883");
//const std::string CLIENT_ID("Qt_MQTT_Client");
//const std::string TOPIC("testtopic");

class CprocessMqtt  : public QObject
{
    Q_OBJECT
public:
    explicit CprocessMqtt();
    ~CprocessMqtt();
    QMqttClient *mqttClient() { return m_mqttClient; }

public slots:
    void connectToBroker(const QString &host, quint16 port);
    void publishMessage(const QString &topic, const QByteArray &message);
    void subscribeToTopic(const QString &topic);

private slots:
    void updateLogStateChange(QMqttClient::ClientState state);
    void brokerDisconnected();
    void handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic);

private:
    QMqttClient *m_mqttClient;
};
#endif // CPROCESSMQTT_H
