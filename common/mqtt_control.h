#ifndef _MQTTCONTROL_H_
#define _MQTTCONTROL_H_

#include "mosquitto.h"
#include "define.h"
#include "zlog.h"

typedef void (*mqttcallback)(int type, char *topic, char *message);

class MqttControl
{
public:
    MqttControl();
    ~MqttControl();
    // 初始化 mqtt
    bool mqttInit(MQTT_INFO &mqttinfo);
    // 连接 MQTT 服务器 函数
    bool mqttConnect();

    bool mqttStart();
    // 发布MQTT 消息
    bool mqttPublish(MQTT_SERVERTYPE type, const char *topic, int payloadlen, const void *payload, int qos, bool retain);
    // 订阅MQTT 消息
    bool mqttSubscribe(MQTT_SERVERTYPE type, int *mid, const char *sub, int qos);
    // 取消订阅MQTT 消息
    bool mqttUnsubscribe(int *mid, const char *sub);

    // 接受消息的 callback 函数
    static void mqttMessageCallback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);

    // 断开连接callback 函数
    static void mqttDisconnectCallback(struct mosquitto *mosq, void *userdata, int connect);

    // 连接callback 函数
    static void mqttConnectCallback(struct mosquitto *mosq, void *userdata, int connect);
    void setCallbackThis();

    void setCallback(void (*mqttcallback)(int, char *, char *));

public:
    static MqttControl *m_mqttcontrol;
    void setAppName(std::string appName);

private:
    void mqttMessage(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
    void mqttConnect(struct mosquitto *mosq, std::string topic, int connect);

private:
    mosquitto *m_mqttcco;
    MQTT_INFO m_mInfo;
    MQTT_INFO m_mInfolocal;
    bool m_bConnect;
    std::string m_appname;
    zlog_category_t *m_logc; // zlog 指針
    mqttcallback m_callback;
};

#endif
