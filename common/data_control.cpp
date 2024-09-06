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
    std::string topic = "test/ccoRouter/JSON/get/request/acqQueryFiles";
    cJSON *root = cJSON_CreateObject();
    addResObject(root, 1);
    cJSON_AddItemToObject(root, "prio", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(root, "startIndex", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(root, "curMeterNum", cJSON_CreateNumber(255));
    char *payload = NULL;
    payload = cJSON_Print(root);

    m_ccoCtl->dataInit(topic.c_str(), payload);

    cJSON_Delete(root);
    free(payload);
}

void DataControl::set_timeout(int timeout)
{
    m_timeout = timeout;
}

int DataControl::addPubilcObject(cJSON *root)
{
    char sztoken[10];
    sprintf(sztoken, "%d", m_nToken);
    int token = m_nToken;
    m_nToken++;
    cJSON_AddItemToObject(root, "token", cJSON_CreateString(sztoken));
    char time[64];
    sys_gettime_timestamp(time);

    cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(time));
    return token;
}

int DataControl::addResObject(cJSON *root, int token)
{
    char sztoken[10];
    sprintf(sztoken, "%d", token);
    cJSON_AddItemToObject(root, "token", cJSON_CreateString(sztoken));
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
    /*
    for (const auto &topicStr : topicStrs)
    {
        // std::cout << topicStr << std::endl;
        if (topicStr == "devinfo") // 设备信息
        {
            devInfoSend(message, 2);
            return;
        }
        if (topicStr == "frpcstatus") // frpc状态
        {
            sendFrpcStatus(message);
            return;
        }
        if (topicStr == "frpcup") // 启动frpc
        {
            frpcStart(message);
            return;
        }
        if (topicStr == "frpcdown") // 停止frpc
        {
            frpcStop((void *)message);
            return;
        }
    }


    std::string str;
    str = topic;
    std::vector<std::string> vectror;
    split(str, "/", vectror);
//    zlog_debug(m_logc,"topic %s",topic);

    cJSON *root = cJSON_Parse(message);
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
    }
    cJSON *element;
    for (element = (root != NULL) ? root->child : NULL; element != NULL; element = element->next)
    {
        if (element->type == cJSON_String)
        {
            if (strcmp(element->string, "token") == 0)
            {
                int token = atoi(element->valuestring);
                std::string str = message;
                m_mapMessage.insert(make_pair(token, str));
                break;
            }
        }
    }
    m_deal.notify_one();
    cJSON_Delete(root);

    */
}

void DataControl::sendFrpcStatus(const char *message)
{
    int token = 0;
    if (message != NULL)
    {
        cJSON *root_ret = cJSON_Parse(message);
        if (root_ret == NULL)
        {
            zlog_error(m_logc, "ret message parse error");
            return;
        }
        cJSON *element;
        for (element = (root_ret != NULL) ? root_ret->child : NULL; element != NULL; element = element->next)
        {
            if (element->type == cJSON_Number)
            {
                if (strcmp(element->string, "token") == 0)
                {
                    token = element->valueint;
                    break;
                }
            }
        }
        cJSON_Delete(root_ret);
    }

    cJSON *root = cJSON_CreateObject();
    cJSON *ports = cJSON_CreateObject();

    if (message != NULL)
    {
        addResObject(root, token);
    }
    else
    {
        addPubilcObject(root);
    }

    if (m_isStart)
    {
        cJSON_AddItemToObject(root, "status", cJSON_CreateString("up"));
        for (int i = 0; i < m_frpInfo->frpcInfo.size(); i++)
        {
            std::string portName = m_frpInfo->frpcInfo[i].name + "_port";
            cJSON_AddItemToObject(ports, portName.c_str(), cJSON_CreateNumber(m_frpInfo->frpcInfo[i].remote_port));
        }
        cJSON_AddItemToObject(root, "ports", ports);
        double keep_time = ((m_frpInfo->keep_time * 60 - m_timeCount) / 60) + 1;
        cJSON_AddItemToObject(root, "keep_time", cJSON_CreateNumber(keep_time));
    }
    else
    {
        cJSON_AddItemToObject(root, "status", cJSON_CreateString("down"));
        cJSON_AddItemToObject(root, "ports", ports);
        cJSON_AddItemToObject(root, "keep_time", cJSON_CreateNumber(0));
    }

    char *payload = NULL;
    payload = cJSON_Print(root);
    bool bRet;

    /// ToPlatform/{sn}/json/set_ark/frpcup
    std::vector<std::string> topic_part;
    topic_part.push_back("ToPlatform");
    topic_part.push_back(m_devSn);
    topic_part.push_back("json");
    if (message != NULL)
    {
        topic_part.push_back("get_ark");
    }
    else
    {
        topic_part.push_back("update");
    }

    topic_part.push_back("frpcstatus");
    char topic[128];
    memset(topic, 0, 128);
    sysOutputTopic(topic_part, topic);
    bRet = m_mqttControl->mqttPublish(masterserver, topic, strlen(payload), payload, m_mInfo->nqos, false);
    if (!bRet)
    {
        zlog_error(m_logc, "publish topic %s error", topic);
    }
    zlog_info(m_logc, "frpcstatus response : \n%s ", payload);
    cJSON_Delete(root);
    free(payload);
}

void DataControl::devInfoSend(const char *message, int type)
{
    int token = 0;
    if (type == 2)
    {
        cJSON *root_ret = cJSON_Parse(message);
        if (root_ret == NULL)
        {
            zlog_error(m_logc, "ret message parse error");
            return;
        }
        cJSON *element;
        for (element = (root_ret != NULL) ? root_ret->child : NULL; element != NULL; element = element->next)
        {
            if (element->type == cJSON_Number)
            {
                if (strcmp(element->string, "token") == 0)
                {
                    token = element->valueint;

                    break;
                }
            }
        }
        cJSON_Delete(root_ret);
    }

    cJSON *root = cJSON_CreateObject();
    if (type == 1)
    {
        addPubilcObject(root);
    }
    if (type == 2)
    {
        addResObject(root, token);
    }

    cJSON *body = cJSON_CreateObject();

    cJSON_AddItemToObject(body, "sn", cJSON_CreateString(m_devSn.c_str()));

    struct utsname uts;

    // 调用 uname() 获取系统信息
    if (uname(&uts) == -1)
    {
        zlog_error(m_logc, "uname failed !");
        return;
    }
    // 打印内核版本信息
    // zlog_info(m_logc, "Kernel version: %s", uts.release);
    cJSON_AddItemToObject(body, "uname", cJSON_CreateString(uts.release));
    cJSON_AddItemToObject(root, "body", body);

    char *payload = NULL;
    payload = cJSON_Print(root);
    bool bRet;
    std::vector<std::string> topic_part;
    topic_part.push_back("ToPlatform");
    topic_part.push_back(m_devSn);
    topic_part.push_back("json");
    topic_part.push_back("get_ark");
    topic_part.push_back("devinfo");
    char topic[128];
    memset(topic, 0, 128);
    sysOutputTopic(topic_part, topic);
    bRet = m_mqttControl->mqttPublish(masterserver, topic, strlen(payload), payload, m_mInfo->nqos, false);
    if (!bRet)
    {
        zlog_error(m_logc, "publish topic %s error", topic);
    }
    zlog_info(m_logc, "devinfo response : \n%s ", payload);
    cJSON_Delete(root);
    free(payload);
}

bool DataControl::frpcStart()
{

    std::vector<FRPC_INFO> frpcInfo = m_frpInfo->frpcInfo;
    FRPS_INFO frpsInfo = m_frpInfo->frpsInfo;
    frpcStop(NULL); // 先杀了
    std::string frpcIni = "[common]\nserver_addr = " + std::string(frpsInfo.host) +
                          "\nserver_port = " + std::to_string(frpsInfo.nport) +
                          "\ntoken = " + std::string(frpsInfo.token);

    for (int i = 0; i < frpcInfo.size(); i++)
    {
        std::string portConf = "\n\n[" + m_devSn + std::string("_") + frpcInfo[i].name + std::string("]");
        portConf += "\ntype = " + frpcInfo[i].type;
        portConf += "\nlocal_ip = " + frpcInfo[i].local_ip;
        portConf += "\nlocal_port = " + std::to_string(frpcInfo[i].local_port);
        // portConf += "\nremote_port = " + std::to_string(frpcInfo[i].remote_port); // 用对端下发的端口号
        frpcIni += portConf;
    }

    zlog_info(m_logc, "frpc.ini : \n%s", frpcIni.c_str());

    std::ofstream outputFile(m_frpInfo->conf_path + std::string("frpc.ini"), std::ios_base::out);
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Unable to open file." << std::endl;
        return 1;
    }
    outputFile << frpcIni << std::endl;
    outputFile.close();

    std::string startCmd = "";

    startCmd = "chmod +x " + m_frpInfo->conf_path + std::string("frpc");
    system(startCmd.c_str());
    zlog_info(m_logc, "start frpc.  startCmd : %s", startCmd.c_str());
    startCmd = m_frpInfo->conf_path + std::string("frpc") + " -c " + m_frpInfo->conf_path + "frpc.ini > /tmp/frpcUp.log &";
    // startCmd = m_frpInfo->conf_path + "frpc > /tmp/frpcUp.log" + " &";
    system(startCmd.c_str());
    zlog_info(m_logc, "start frpc.  startCmd : %s", startCmd.c_str());

    return true;
}

bool DataControl::frpcStart(const char *message)
{
    int token = parseFrpcUpInfo(message);
    if (token < 0)
    {
        return false;
    }
    std::vector<FRPC_INFO> frpcInfo = m_frpInfo->frpcInfo;
    FRPS_INFO frpsInfo = m_frpInfo->frpsInfo;
    frpcStop(NULL); // 先杀了
    std::string frpcIni = "[common]\nserver_addr = " + std::string(frpsInfo.host) +
                          "\nserver_port = " + std::to_string(frpsInfo.nport) +
                          "\ntoken = " + std::string(frpsInfo.token);

    for (int i = 0; i < frpcInfo.size(); i++)
    {
        std::string portConf = "\n\n[" + m_devSn + std::string("_") + frpcInfo[i].name + std::string("]");
        portConf += "\ntype = " + frpcInfo[i].type;
        portConf += "\nlocal_ip = " + frpcInfo[i].local_ip;
        portConf += "\nlocal_port = " + std::to_string(frpcInfo[i].local_port);
        portConf += "\nremote_port = " + std::to_string(frpcInfo[i].remote_port); // 用对端下发的端口号
        frpcIni += portConf;
    }

    zlog_info(m_logc, "frpc.ini : \n%s", frpcIni.c_str());

    std::ofstream outputFile(m_frpInfo->conf_path + std::string("frpc.ini"), std::ios_base::out);
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Unable to open file." << std::endl;
        return 1;
    }
    outputFile << frpcIni << std::endl;
    outputFile.close();

    std::string startCmd = "chmod +x " + m_frpInfo->conf_path + std::string("frpc");
    system(startCmd.c_str());
    zlog_info(m_logc, "start frpc.  startCmd : %s", startCmd.c_str());
    startCmd = m_frpInfo->conf_path + std::string("frpc") + " -c " + m_frpInfo->conf_path + "frpc.ini > /tmp/frpcUp.log &";
    // startCmd = m_frpInfo->conf_path + "frpc > /tmp/frpcUp.log" + " &";
    system(startCmd.c_str());
    zlog_info(m_logc, "start frpc.  startCmd : %s", startCmd.c_str());

    std::string resMsg = "";
    std::string result = "";
    if (getProcessIdByName("frpc") < 0)
    {
        std::ifstream file("/tmp/frpcUp.log");
        std::string line;
        while (std::getline(file, line))
        {
            resMsg += line;
        }
        file.close();
        result = "failure";
        zlog_error(m_logc, "start frpc failed! reason : %s", resMsg.c_str());
    }
    else
    {
        if (!m_isStart)
        {
            std::thread stopFrpcThread(&DataControl::stopFrpcThreadFunc, this);
            stopFrpcThread.detach();
        }
        result = "succeed";
        zlog_info(m_logc, "start frpc successed!");
    }

    // 打包
    cJSON *root = cJSON_CreateObject();
    addResObject(root, token);
    cJSON_AddItemToObject(root, "result", cJSON_CreateString(result.c_str()));
    cJSON_AddItemToObject(root, "msg", cJSON_CreateString(resMsg.c_str()));

    char *payload = NULL;
    payload = cJSON_Print(root);
    bool bRet;

    /// ToPlatform/{sn}/json/set_ark/frpcup
    std::vector<std::string> topic_part;
    topic_part.push_back("ToPlatform");
    topic_part.push_back(m_devSn);
    topic_part.push_back("json");
    topic_part.push_back("set_ark");
    topic_part.push_back("frpcup");
    char topic[128];
    memset(topic, 0, 128);
    sysOutputTopic(topic_part, topic);
    bRet = m_mqttControl->mqttPublish(masterserver, topic, strlen(payload), payload, m_mInfo->nqos, false);
    if (!bRet)
    {
        zlog_error(m_logc, "publish topic %s error", topic);
    }
    zlog_info(m_logc, "frpcUp response : \n %s ", payload);
    cJSON_Delete(root);
    free(payload);
    return true;
}

int DataControl::parseFrpcUpInfo(const char *message)
{
    int token = 0;
    cJSON *root_ret = cJSON_Parse(message);
    if (root_ret == NULL)
    {
        zlog_error(m_logc, "ret message parse error");
        return -1;
    }
    cJSON *element;
    for (element = (root_ret != NULL) ? root_ret->child : NULL; element != NULL; element = element->next)
    {
        if (element->type == cJSON_Number)
        {
            if (strcmp(element->string, "token") == 0)
            {
                token = element->valueint;

                continue;
            }
            if (strcmp(element->string, "server_port") == 0)
            {
                m_frpInfo->frpsInfo.nport = element->valueint;

                continue;
            }
            if (strcmp(element->string, "keep_time") == 0)
            {
                m_frpInfo->keep_time = element->valueint;

                continue;
            }
        }
        if (element->type == cJSON_String)
        {
            if (strcmp(element->string, "server_addr") == 0)
            {
                m_frpInfo->frpsInfo.host = element->valuestring;

                continue;
            }
            if (strcmp(element->string, "server_token") == 0)
            {
                m_frpInfo->frpsInfo.token = element->valuestring;

                continue;
            }
        }

        if (element->type == cJSON_Array)
        {
            int nSize = cJSON_GetArraySize(element);
            for (int i = 0; i < nSize; i++)
            {
                cJSON *item = cJSON_GetArrayItem(element, i);

                cJSON *temp = cJSON_GetObjectItem(item, "name");
                m_frpInfo->frpcInfo[i].name = temp->valuestring;

                temp = cJSON_GetObjectItem(item, "remote_port");
                m_frpInfo->frpcInfo[i].remote_port = temp->valueint;
            }
        }
    }
    cJSON_Delete(root_ret);

    return token;
}

void DataControl::stopFrpcThreadFunc()
{
    zlog_info(m_logc, "stopFrpcThread run!");
    int second = 60;
    m_isStart = true;
    while (m_isStart)
    {
        sleep(1);
        if (second == 0 || m_timeCount == 0)
        {
            second = 60;
        }

        second--;
        m_timeCount++;
        zlog_info(m_logc, "remaining time  : [%d min %d s]", (int)((m_frpInfo->keep_time * 60 - m_timeCount) / 60), second);
        if (m_timeCount >= m_frpInfo->keep_time * 60)
        {
            zlog_info(m_logc, "stopFrpcThread stop!");
            break;
        }
    }
    if (m_isStart)
    {
        frpcStop(NULL);
        m_isStart = false;
    }
}

void DataControl::frpcStop(void *userdata)
{
    std::string processName = "frpc";
    int frpcProcessId = getProcessIdByName(processName);
    zlog_info(m_logc, "frpcProcessId = %d", frpcProcessId);

    if (frpcProcessId > 0)
    {
        std::string cmd = "kill -9 " + std::to_string(frpcProcessId);
        zlog_info(m_logc, "cmd : %s", cmd.c_str());
        system(cmd.c_str());
    }

    int token = 0;
    m_timeCount = 0;
    if (userdata != NULL)
    {
        m_isStart = false;
        cJSON *root_ret = cJSON_Parse((char *)userdata);
        if (root_ret == NULL)
        {
            zlog_error(m_logc, "ret message parse error");
            return;
        }
        cJSON *element;
        for (element = (root_ret != NULL) ? root_ret->child : NULL; element != NULL; element = element->next)
        {
            if (element->type == cJSON_Number)
            {
                if (strcmp(element->string, "token") == 0)
                {
                    token = element->valueint;
                    break;
                }
            }
        }
        cJSON_Delete(root_ret);

        cJSON *root = cJSON_CreateObject();
        addResObject(root, token);
        cJSON_AddItemToObject(root, "result", cJSON_CreateString("succeed"));
        cJSON_AddItemToObject(root, "msg", cJSON_CreateString(""));
        char *payload = NULL;
        payload = cJSON_Print(root);
        bool bRet;

        // ToPlatform/{sn}/json/set_ark/frpcup
        std::vector<std::string> topic_part;
        topic_part.push_back("ToPlatform");
        topic_part.push_back(m_devSn);
        topic_part.push_back("json");
        topic_part.push_back("set_ark");
        topic_part.push_back("frpcdown");
        char topic[128];
        memset(topic, 0, 128);
        sysOutputTopic(topic_part, topic);
        bRet = m_mqttControl->mqttPublish(masterserver, topic, strlen(payload), payload, m_mInfo->nqos, false);
        if (!bRet)
        {
            zlog_error(m_logc, "publish topic %s error", topic);
        }
        zlog_info(m_logc, "frpcDown response : \n %s ", payload);
        cJSON_Delete(root);
        free(payload);
    }
}

void DataControl::setCallback(void (*callback)(int, int, void *))
{
    m_callback = callback;
}
