#include "data_control.h"
#include "define.h"
#include "globalbase.h"

#include "protocol_gw1376_2.h"
#include "protocol_gw1376_2_data.h"
#include "cJSON.h"

#include <array>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/utsname.h>
#include <thread>

DataControl::DataControl(std::string appname, FRP_INFO *frpInfo)
{

    m_appName = appname;
    m_mInfo = new MQTT_INFO;
    m_mInfo->nport = 0;
    m_mInfo->nkeepalive = 60;
    m_nToken = 1;
    m_timeout = 2;

    m_callback = NULL;
    m_datacontrol = this;

    m_frpInfo = frpInfo;

    m_isStart = false;
    m_timeCount = 0;

    // 1376.2数据初始化
    protocol_gw1376_2_init((void *)get_pdata());
    m_ccoCtl = new CcoControl();
}

DataControl::~DataControl()
{
    delete m_mInfo;
    m_mInfo = NULL;
}

int DataControl::init(MqttControl *mqttControl)
{
    m_logc = zlog_get_category("datacontrol");
    m_mqttControl = mqttControl;
    m_ccoCtl->setMqttControl(mqttControl, m_mInfo);
    return 0;
}

void DataControl::dataInit()
{
    char *payload = NULL;
    std::string topic1 = "test/ccoRouter/JSON/get/request/masterNode";
    cJSON *root1 = cJSON_CreateObject();
    addResObject(root1, 1);
    cJSON_AddItemToObject(root1, "prio", cJSON_CreateNumber(1));
    payload = cJSON_Print(root1);
    m_ccoCtl->dataInit(topic1.c_str(), payload);
    cJSON_Delete(root1);
    free(payload);
    usleep(300 * 1000);

    std::string topic = "test/ccoRouter/JSON/get/request/acqQueryFiles";
    cJSON *root = cJSON_CreateObject();
    addResObject(root, 2);
    cJSON_AddItemToObject(root, "prio", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(root, "startIndex", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(root, "curMeterNum", cJSON_CreateNumber(255));
    payload = NULL;
    payload = cJSON_Print(root);
    m_ccoCtl->dataInit(topic.c_str(), payload);
    cJSON_Delete(root);
    free(payload);
    usleep(200 * 1000);
}

void DataControl::gw13762ProcessBuf(std::string message, int len)
{
    protocol_gw1376_2_process_buf((char *)message.c_str(), len, get_pdata());
}

void DataControl::set_timeout(int timeout)
{
    m_timeout = timeout;
}

int DataControl::addPubilcObject(cJSON *root)
{
    int token = m_nToken;
    m_nToken++;
    cJSON_AddItemToObject(root, "token", cJSON_CreateNumber(token));
    char time[64];
    sys_gettime_timestamp(time);

    cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(time));
    return token;
}

int DataControl::addResObject(cJSON *root, int token)
{
    cJSON_AddItemToObject(root, "token", cJSON_CreateNumber(token));
    char time[64];
    sys_gettime_timestamp(time);
    cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(time));
    return token;
}

void DataControl::addSubscribeTopic()
{
    std::string topic;
    bool bRet;
    topic = "+/" + m_appName + "/JSON/#";
    bRet = m_mqttControl->mqttSubscribe(masterserver, NULL, topic.c_str(), m_mInfo->nqos);
    if (!bRet)
    {
        zlog_error(m_logc, "topic %s subscribe faild ", topic.c_str());
    }
    zlog_info(m_logc, "topic %s subscribe successed! ", topic.c_str());
}

DataControl *DataControl::m_datacontrol = NULL;
void DataControl::callbackMqttMessage(char *topic, char *message)
{
    m_datacontrol->parsingMqttMessage(topic, message);
}

void DataControl::packSendMqttMessage(void *data, int dataSize)
{

    m_ccoCtl->packSendMqttMsg(data, dataSize);
}

void DataControl::parsingMqttMessage(const char *topic, const char *message)
{
    zlog_info(m_logc, "topic : %s ; message :\n%s ", topic, message);
    m_ccoCtl->parseCcoData(topic, message);
    return;
}

void DataControl::setCallback(void (*callback)(int, int, void *))
{
    m_callback = callback;
}
