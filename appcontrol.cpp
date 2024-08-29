#include "appcontrol.h"
#include "cJSON.h"
#include "globalbase.h"

#include <thread>
#include <algorithm>
#include <iostream>
#include <algorithm>

AppControl *AppControl::m_pApp = NULL;

AppControl::AppControl()
{
    m_logc = zlog_get_category("appcontrol");

    m_queue = new QueueBuffer(50);
    m_pApp = this;

    m_hplcPort = new SerialPort();
}

AppControl::~AppControl()
{
    delete m_pMqttControl;
    delete m_hplcPort;
}

int AppControl::init()
{

    int nRet = readConfig();
    if (nRet < 0)
    {
        zlog_error(m_logc, "config file read failed");
        return -2;
        //    initMemberData();
    }

    if (m_hplcPort->init() != true)
    {
        zlog_error(m_logc, "m_hplcPort->init() failed");
        return -1;
    }

    m_pMqttControl = new MqttControl();

    m_pMqttControl->mqttInit(m_mInfo);
    m_pMqttControl->setAppName(m_appName);
    m_pMqttControl->mqttConnect();
    m_pMqttControl->mqttStart();
    m_pMqttControl->setCallback(callbackMqttMessage);

    m_pDataControl = new DataControl(m_devSn, &m_frpInfo);
    m_pDataControl->init(m_pMqttControl);

    return 0;
}

std::string AppControl::mySystem(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

void AppControl::start()
{

    std::thread dealMessageThread(&AppControl::dealMessageThreadFunc, this);
    dealMessageThread.detach();
    zlog_info(m_logc, "main thread start!");
    while (true)
    {
        usleep(100 * 1000);
        // m_pMqttControl->mqttPublish(masterserver, "SCU_202404161435/cloud/JSON/get/response/devinfo", 3, "abc", 1, false);
    }
}

void AppControl::dealMessageThreadFunc()
{
    zlog_info(m_logc, "dealMessageThreadFunc start!");
    while (true)
    {
        DATAMESSAGE message = m_queue->get();
        dealMessage(&message);
    }
}

int AppControl::readConfig()
{
    FILE *file;
    zlog_debug(m_logc, "begin read configfile");
    char szPath[128] = {0};
    getcwd(szPath, sizeof(szPath));
    sprintf(szPath, CONFIGPATH);

    sprintf(szPath, "%sconfig.json", szPath);
    zlog_debug(m_logc, "config is %s ", szPath);
    if (access(szPath, R_OK) == -1)
    {
        zlog_error(m_logc, "the config file is not exist ");
        return -1;
    }
    file = fopen(szPath, "r");
    if (file == NULL)
    {
        zlog_error(m_logc, "the config file open failed ");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    int nLen = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *szFile = new char[nLen];

    memset(szFile, 0, nLen);
    fread(szFile, 1, nLen, file);
    fclose(file);

    cJSON *root = cJSON_Parse(szFile);
    if (!root)
    {
        zlog_error(m_logc, "json config Invalid Json Error before: [%s]", cJSON_GetErrorPtr());
        delete[] szFile;
        szFile = NULL;
        return -1;
    }

    cJSON *element;
    cJSON *element_child;
    for (element = root->child; element != NULL; element = element->next)
    {
        if (element->type == cJSON_String)
        {
            if (strcmp(element->string, "appname") == 0)
            {
                m_appName = element->valuestring;
                zlog_info(m_logc, "m_appName is %s ", m_appName.c_str());
            }
        }
        else if (element->type == cJSON_Object)
        {
            if (strcmp(element->string, "mqttinfo") == 0)
            {
                for (element_child = element->child; element_child != NULL; element_child = element_child->next)
                {
                    if (strcmp(element_child->string, "ip") == 0)
                    {
                        strcpy(m_mInfo.host, element_child->valuestring);
                        zlog_info(m_logc, "m_mInfo.host is %s ", m_mInfo.host);
                    }
                    else if (strcmp(element_child->string, "port") == 0)
                    {
                        m_mInfo.nport = element_child->valueint;
                        zlog_info(m_logc, "m_mInfo.nport is %d ", m_mInfo.nport);
                    }
                    else if (strcmp(element_child->string, "keepalive") == 0)
                    {
                        m_mInfo.nkeepalive = element_child->valueint;
                        zlog_info(m_logc, "m_mInfo.nkeepalive is %d ", m_mInfo.nkeepalive);
                    }
                    else if (strcmp(element_child->string, "clientid") == 0)
                    {
                        strcpy(m_mInfo.clientid, element_child->valuestring);
                        zlog_info(m_logc, "m_mInfo.clientid is %s ", m_mInfo.clientid);
                    }
                    else if (strcmp(element_child->string, "user") == 0)
                    {
                        strcpy(m_mInfo.user, element_child->valuestring);
                        zlog_info(m_logc, "m_mInfo.user is %s ", m_mInfo.user);
                    }
                    else if (strcmp(element_child->string, "password") == 0)
                    {
                        strcpy(m_mInfo.password, element_child->valuestring);
                        zlog_info(m_logc, "m_mInfo.password is %s ", m_mInfo.password);
                    }
                    else if (strcmp(element_child->string, "qos") == 0)
                    {
                        m_mInfo.nqos = element_child->valueint;
                        zlog_info(m_logc, "m_mInfo.nqos is %d ", m_mInfo.nqos);
                    }
                }
            }

            if (strcmp(element->string, "uart") == 0)
            {
                for (element_child = element->child; element_child != NULL; element_child = element_child->next)
                {
                    if (strcmp(element_child->string, "name") == 0)
                    {
                        m_hplcPort->portName = std::string(element_child->valuestring);
                        zlog_info(m_logc, "m_plcPort->portName is %s ", m_hplcPort->portName.c_str());
                    }
                    else if (strcmp(element_child->string, "baudrate") == 0)
                    {
                        m_hplcPort->baudRate = element_child->valueint;
                        zlog_info(m_logc, "m_plcPort->baudRate is %d ", m_hplcPort->baudRate);
                    }
                    else if (strcmp(element_child->string, "databit") == 0)
                    {
                        m_hplcPort->dataBit = element_child->valueint;
                        zlog_info(m_logc, "m_plcPort->dataBit is %d ", m_hplcPort->dataBit);
                    }
                    else if (strcmp(element_child->string, "stopbit") == 0)
                    {
                        m_hplcPort->stopBit = element_child->valueint;
                        zlog_info(m_logc, "m_plcPort->stopBit is %d ", m_hplcPort->stopBit);
                    }
                    else if (strcmp(element_child->string, "parity") == 0)
                    {
                        m_hplcPort->parity = std::string(element_child->valuestring);
                        zlog_info(m_logc, "m_plcPort->parity is %s ", m_hplcPort->parity.c_str());
                    }
                    else if (strcmp(element_child->string, "interval") == 0)
                    {
                        m_hplcPort->interval = element_child->valueint;
                        zlog_info(m_logc, "m_plcPort->interval is %d ", m_hplcPort->interval);
                    }
                }
            }
        }
        cJSON_Delete(root);
        return 0;
    }
}

void AppControl::callbackMqttMessage(int type, char *topic, char *message)
{
    m_pApp->parsingMqttMessage(type, topic, message);
}

void AppControl::parsingMqttMessage(int type, char *topic, char *message)
{
    DATAMESSAGE send;
    send.src = type;
    send.topic = topic;
    send.message = message;
    m_queue->push(send);
}

void AppControl::dealMessage(DATAMESSAGE *message)
{
    // zlog_debug(m_logc, "zlog src %d", message->src);
    // zlog_debug(m_logc, "zlog topic %s", message->topic.c_str());
    // zlog_debug(m_logc,"zlog message %s",message->message.c_str());
    if (message->src == 1) // 消息处理
    {
        /*
        size_t pos = message->topic.find("command");
        if (pos != std::string::npos)
        {
            std::thread thread(&AppControl::dealCenterCmd, this, message->message);
            thread.detach();
        }
        else
        {
            m_pDataControl->parsingMqttMessage(message->topic.c_str(), message->message.c_str());
        }*/

        m_pDataControl->parsingMqttMessage(message->topic.c_str(), message->message.c_str());
    }
    else if (message->src == 2) // 连接状态改变处理
    {
        if (message->topic == "connect")
        {
            int temp = stoi(message->message);
            if (temp != 0)
            {
                return;
            }
            m_localconnect = 0;
            m_pDataControl->addSubscribeTopic();
            //m_pDataControl->devInfoSend(NULL, 1);
            //m_pDataControl->sendFrpcStatus(NULL);
            // 心跳线程
            // m_pDataControl->heartThreadStart();
        }
        else if (message->topic == "disconnect")
        {
            m_localconnect = stoi(message->message);
        }
    }
}
