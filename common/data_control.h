#ifndef _DATACONTROL_H_
#define _DATACONTROL_H_

#include "globalbase.h"
#include <map>
#include "zlog.h"
#include "mqtt_control.h"
#include "common_utils.h"
#include "cJSON.h"
#include "timernode.h"
#include "cco_control.h"

typedef std::map<std::string, std::vector<MODEL_BASE>> modelmap;
typedef std::map<std::string, REGISTER_DEV> registermap;

typedef void (*callback)(int type, int size, void *msg);

class DataControl
{
public:
    DataControl(std::string devSn, FRP_INFO *frpInfo);
    ~DataControl();

    int init(MqttControl *mqttControl);
    void set_timeout(int timeout); // 单位秒
    static void callbackMqttMessage(char *topic, char *message);
    void parsingMqttMessage(const char *topic, const char *message);
    void setCallback(void (*callback)(int, int, void *));
    void addSubscribeTopic();

public:
    static DataControl *m_datacontrol;
    void packSendMqttMessage(void *data, int dataSize);

    void dataInit();

    void frpcStop(void *userdata); // frpc程序关闭
    bool frpcStart();
    /*
     *设备信息响应 type : 1主动上送 2响应
     */
    void devInfoSend(const char *message, int type);

    void sendFrpcStatus(const char *message);

private:
    int addPubilcObject(cJSON *root);
    int addResObject(cJSON *root, int toker);
    bool frpcStart(const char *message); // frpc程序启动
    int parseFrpcUpInfo(const char *message);
    void stopFrpcThreadFunc();

private:
    CcoControl *m_ccoCtl; // cco数据控制

    std::string m_devSn;
    std::string m_appName;
    MQTT_INFO *m_mInfo; // MQTT 配置信息，配置文件中获取
    int m_nToken;       // 与数据中心通过MQTT报文交互时用于定位报文顺序的变量
    int m_timeout;
    MqttControl *m_mqttControl; // MQTT接口类指针
    bool m_MqttMessge;
    std::mutex m_mutex;             // 消息通知锁
    std::condition_variable m_deal; // 条件变量类
    MSGMAP m_mapMessage;
    zlog_category_t *m_logc; // zlog 指針

    callback m_callback;

    FRP_INFO *m_frpInfo; // frp配置信息，配置文件中获取

    bool m_isStart;  // 线程是否启动
    int m_timeCount; // 时间计数
    bool m_iskill;   // 之前的线程killflag
};

#endif
