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
#define CONFIGPATH "/data/app/ccoRouter/conf/"

#define APP_VER_LOCAL 0 // 专网卡
#define APP_VER_MQTT 1  // 公网卡

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

    void dealMessageThreadFunc();

private:
    std::string m_devSn;   // 设备sn
    std::string m_appName; // APP名称
    MqttControl *m_pMqttControl;
    QueueBuffer *m_queue;
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

    int m_appVer; // 程序版本 0 专网卡版本   1 公网卡版本
};

#endif