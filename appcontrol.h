#ifndef __APPCONTROL_H__
#define __APPCONTROL_H__

#include "mqtt_control.h"
#include "queuebuffer.h"
#include "define.h"
#include "timerlist.h"
#include "data_control.h"
#include "SerialPort.h"

#include <string>
#include <vector>
#include <map>
#include "zlog.h"

#define MAXTIMEOUT 60
#define CONFIGPATH "/home/sysadm/pengwx/ccoRouter/conf/"

#define APP_VER_LOCAL 0
#define APP_VER_MQTT 1

/* 信道 */
typedef enum dealMsgType
{
    UART_MSG = 0, /* 中继级别0 */
    MQTT_MSG,
    MQTT_CONNECT_MSG,
} DEAL_MSG_TYPE;

class AppControl
{
public:
    AppControl();
    ~AppControl();
    int init();
    void start();
    //    static void timerCallback(void *message);
    static AppControl *m_pApp;

    static void callbackMqttMessage(int type, char *topic, char *message);
    void parsingMqttMessage(int type, char *topic, char *message);
    std::string mySystem(const char *cmd);

    int GetAppVer();

private:
    int readConfig();
    void dealMessage(DATAMESSAGE *message);

    void serialSendThreadFunc();     // 串口发送线程
    void dealSerialRecvThreadFunc(); // 处理串口接收消息 线程
    void dealMessageThreadFunc();    // 处理mqtt接收到的消息  线程
    void serialRecvThreadFunc();     // 接收串口消息线程

    void mqttRecvThreadFunc(); // 返回mqtt消息线程

    //void FreeFunc(void *info);

private:
    std::string m_devSn;   // 设备sn
    std::string m_appName; // APP名称
    MqttControl *m_pMqttControl;
    QueueBuffer *m_queue;
    QueueBuffer *m_uartQueue;
    zlog_category_t *m_logc;
    std::string m_mfr;
    MQTT_INFO m_mInfo;
    SerialPort *m_hplcPort;

    FRP_INFO m_frpInfo;
    DataControl *m_pDataControl;
    int m_cycle;
    TimerList *m_timerlist;
    std::vector<UPDATA_DEVINFO> m_devinfo;
    int m_iotconnect;
    int m_localconnect;

    bool m_bGetDev;
    int m_interval;

    int m_channel; // 信道设置
};

#endif