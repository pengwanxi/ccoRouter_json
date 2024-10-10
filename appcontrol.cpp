#include "appcontrol.h"
#include "cJSON.h"
#include "globalbase.h"
#include "protocol_gw1376_2_data.h"
#include "protocol_gw1376_2.h"
#include "protocol_gw1376_2_public.h"
#include "Dlist.h"
#include <thread>
#include <algorithm>
#include <iostream>
#include <algorithm>

AppControl *AppControl::m_pApp = NULL;

AppControl::AppControl()
{
    m_logc = zlog_get_category("appcontrol");

    m_queue = new QueueBuffer(100);
    m_uartQueue = new QueueBuffer(100); // 串口接收队列

    m_pApp = this;

    m_hplcPort = new SerialPort();

    m_channel = 1;
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

    m_pDataControl = new DataControl(m_appName, &m_frpInfo);

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
    // mqtt消息处理线程
    std::thread dealMessageThread(&AppControl::dealMessageThreadFunc, this);
    dealMessageThread.detach();

    // 串口发送线程  判断链表是否为空，不为空拿出来发送
    std::thread serialSendThread(&AppControl::serialSendThreadFunc, this);
    serialSendThread.detach();

    // 串口接收线程
    std::thread serialRecvThread(&AppControl::serialRecvThreadFunc, this);
    serialRecvThread.detach();

    // 串口处理线程
    // std::thread dealSerialRecvThread(&AppControl::dealSerialRecvThreadFunc, this);
    // dealSerialRecvThread.detach();

    // mqtt消息返回线程
    std::thread mqttRecvThread(&AppControl::mqttRecvThreadFunc, this);
    mqttRecvThread.detach();
    usleep(500 * 1000);
    zlog_info(m_logc, "main thread start!");
    m_pDataControl->dataInit();

    while (true)
    {
        usleep(100 * 1000);
        // m_pMqttControl->mqttPublish(masterserver, "SCU_202404161435/cloud/JSON/get/response/devinfo", 3, "abc", 1, false);
    }
}

// 串口发送线程
void AppControl::serialSendThreadFunc()
{
    int len = 0;
    char buf[PROTOCOL_BUF_LEN];
    PROTOCOL_GW1376_2_DATA *pdata = get_pdata();
    GW13762_TASK *ptask = &pdata->task;

    while (true)
    {
        len = 0;
        memset(buf, 0, PROTOCOL_BUF_LEN);
        usleep(20 * 1000);

        if (!m_hplcPort->devNodeExist())
        {
            m_hplcPort->isOpen = false;
            continue;
        }

        if (!m_hplcPort->isOpen)
        {
            m_hplcPort->init();
        }

        if (gw13762_task_idle(ptask) == false)
        {

            GW13762_TASK_DATA *ptdata = gw13762_task_head(ptask);
            if (NULL == ptdata)
            {
                continue;
            }

            /**
            if (gw13762_task_read_queue_clear_head_timeout(ptask) != 0)
            {
                printf("11111111continue \n");
                continue;
            }*/

            protocol_gw1376_2_data_set_send_task_data(pdata, ptdata);

            int type = ptdata->type;

            if (type <= 0 || type >= GW1376_2_DATA_TYPE_SIZE)
            {
                dzlog_notice("error type = %d", type);
                continue;
            }

            pdata->type = type;
            // 设置信道
            protocol_gw1376_2_data_set_send_channel(pdata, m_channel);
            m_channel++;
            if (m_channel > GW1376_2_CHANNEL15)
            {
                m_channel = GW1376_2_CHANNEL1;
            }

            // 获取报文
            len = protocol_gw1376_2_get_sendbuf(buf, &len, (void *)pdata);
            if (len > 0)
            {
                // 发送报文  判断串口设备节点是否存在
                int writeNum = m_hplcPort->sendData(buf, len);

                char msg[] = "uart send data : [ %s ]";
                zlog_print(m_logc, msg, buf, len);
                memcpy(&pdata->recv.task_data, ptdata, sizeof(GW13762_TASK_DATA));
                gw13762_task_remove(&pdata->task, ptdata->index);
                if (writeNum > 0 && ptdata->type == GW1376_2_DATA_TYPE_CONCURRENT_METER_READING || ptdata->type == GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA)
                {
                    AFN00_INFO afnInfo = {0};
                    afnInfo.AFN = 0x01;
                    afnInfo.Fn = 0x01;
                    afnInfo.result = 0;
                    RES_INFO *resInfo = CreateResInfo();
                    if (resInfo == NULL)
                    {
                        dzlog_error("malloc failed!");
                        continue;
                    }
                    resInfo->index = pdata->send.ptask_data->index;
                    resInfo->isReport = false;
                    resInfo->info = (void *)&afnInfo;
                    resInfo->infoSize = sizeof(AFN00_INFO);
                    resInfo->gw13762DataType = pdata->type;
                    protocol_gw1376_res_ListAddNode(pdata, (void *)resInfo, sizeof(RES_INFO));
                }
                else if (writeNum <= 0 && ptdata->type == GW1376_2_DATA_TYPE_CONCURRENT_METER_READING || ptdata->type == GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA)
                {
                    AFN00_INFO afnInfo = {0};
                    afnInfo.AFN = 0x00;
                    afnInfo.Fn = 0x02;
                    afnInfo.result = 0;
                    RES_INFO *resInfo = CreateResInfo();
                    if (resInfo == NULL)
                    {
                        dzlog_error("malloc failed!");
                        continue;
                    }
                    resInfo->index = pdata->send.ptask_data->index;
                    resInfo->isReport = false;
                    resInfo->info = (void *)&afnInfo;
                    resInfo->infoSize = sizeof(AFN00_INFO);
                    resInfo->gw13762DataType = pdata->type;
                    protocol_gw1376_res_ListAddNode(pdata, (void *)resInfo, sizeof(RES_INFO));
                }

                // hzlog_info(m_logc, buf, len);
            }
            continue;
        }
    }
}

// 接收串口消息线程
void AppControl::serialRecvThreadFunc()
{
    zlog_info(m_logc, "serialRecvThreadFunc start!");

    char recvBuf[2048];
    while (true)
    {
        usleep(50 * 1000);
        memset(recvBuf, 0, sizeof(recvBuf));
        int size = m_hplcPort->receiveData(recvBuf, sizeof(recvBuf));
        // dzlog_info("read size : [%d] ", size);
        if (size <= 0)
        {
            continue;
        }
        // printf("======>1111111");
        char msg[] = "uart recv data : [ %s ]";
        zlog_print(m_logc, msg, recvBuf, size);

        DATAMESSAGE send;
        send.src = UART_MSG;
        send.topic = "uart";
        send.size = size;
        send.message = std::string(recvBuf, send.size);
        dealMessage(&send);
    }
}
// 处理串口消息线程
void AppControl::dealSerialRecvThreadFunc()
{
    zlog_info(m_logc, "dealSerialRecvThreadFunc start!");
    while (true)
    {
        DATAMESSAGE message = m_uartQueue->get();
        dealMessage(&message);
    }
}
void FreeFunc(void *info)
{
    free(info);
}
// 返回mqtt消息线程
void AppControl::mqttRecvThreadFunc()
{
    zlog_info(m_logc, "mqttRecvThreadFunc start!");
    PROTOCOL_GW1376_2_DATA *pdata = get_pdata();
    PROTOCOL_GW1376_2_RECV_DATA *precv = &pdata->recv;
    ListHead_t *resHead = precv->res_data_head;
    ListNode *listNode = NULL;

    while (true)
    {
        usleep(20 * 1000);
        listNode = NULL;
        pthread_mutex_lock(&precv->resHeadLock);
        listNode = ListGetNode(resHead);
        pthread_mutex_unlock(&precv->resHeadLock);
        while (listNode == NULL)
        {
            pthread_mutex_lock(&precv->resHeadLock);
            zlog_info(m_logc, "线程等待");
            pthread_cond_wait(&precv->resHeadCond, &precv->resHeadLock);
            listNode = ListGetNode(resHead);
            pthread_mutex_unlock(&precv->resHeadLock);
        }
        if (listNode == NULL)
        {
            continue;
        }
        m_pDataControl->packSendMqttMessage(listNode->data, listNode->dataSize);

        DestroyListNodeData(listNode, FreeFunc);
    }
}

// 处理mqtt消息线程
void AppControl::dealMessageThreadFunc()
{
    zlog_info(m_logc, "dealMessageThreadFunc start!");
    while (true)
    {
        DATAMESSAGE message = m_queue->get();
        dealMessage(&message);
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
    if (message->src == UART_MSG) // 串口消息处理
    {
        m_pDataControl->gw13762ProcessBuf(message->message, message->size);
        // protocol_gw1376_2_process_buf((char *)message->message.c_str(), message->size, get_pdata());
    }
    else if (message->src == MQTT_MSG) // mqtt消息处理
    {

        m_pDataControl->parsingMqttMessage(message->topic.c_str(), message->message.c_str());
    }
    else if (message->src == MQTT_CONNECT_MSG) // 连接状态改变处理
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
            // m_pDataControl->devInfoSend(NULL, 1);
            // m_pDataControl->sendFrpcStatus(NULL);
            //  心跳线程
            //  m_pDataControl->heartThreadStart();
        }
        else if (message->topic == "disconnect")
        {
            m_localconnect = stoi(message->message);
        }
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
    char *show = cJSON_Print(root);
    zlog_info(m_logc, "root : %s ", show);

    cJSON *element;
    cJSON *element_child;

    /*
    cJSON *appname = cJSON_GetObjectItemCaseSensitive(root, "appname");
    m_appName = appname->valuestring;
    zlog_info(m_logc, "m_appName is %s ", m_appName.c_str());

    cJSON *mqttinfo = cJSON_GetObjectItemCaseSensitive(root, "mqttinfo");
    cJSON *ip = cJSON_GetObjectItemCaseSensitive(mqttinfo, "ip");
    strcpy(m_mInfo.host, ip->valuestring);
    zlog_info(m_logc, "m_mInfo.host is %s ", m_mInfo.host);

    cJSON *port = cJSON_GetObjectItemCaseSensitive(mqttinfo, "port");
    m_mInfo.nport = port->valueint;
    zlog_info(m_logc, "m_mInfo.nport is %d ", m_mInfo.nport);

    cJSON *keepalive = cJSON_GetObjectItemCaseSensitive(mqttinfo, "keepalive");
    m_mInfo.nkeepalive = keepalive->valueint;
    zlog_info(m_logc, "m_mInfo.nkeepalive is %d ", m_mInfo.nkeepalive);

    cJSON *clientid = cJSON_GetObjectItemCaseSensitive(mqttinfo, "clientid");
    strcpy(m_mInfo.clientid, clientid->valuestring);
    zlog_info(m_logc, "m_mInfo.clientid is %s ", m_mInfo.clientid);

    cJSON *user = cJSON_GetObjectItemCaseSensitive(mqttinfo, "user");
    strcpy(m_mInfo.user, user->valuestring);
    zlog_info(m_logc, "m_mInfo.user is %s ", m_mInfo.user);

    cJSON *password = cJSON_GetObjectItemCaseSensitive(mqttinfo, "password");
    strcpy(m_mInfo.password, password->valuestring);
    zlog_info(m_logc, "m_mInfo.password is %s ", m_mInfo.password);

    cJSON *nqos = cJSON_GetObjectItemCaseSensitive(mqttinfo, "qos");
    m_mInfo.nqos = nqos->valueint;
    zlog_info(m_logc, "m_mInfo.nqos is %d ", m_mInfo.nqos);

    cJSON *uartinfo = cJSON_GetObjectItemCaseSensitive(root, "uart");
    cJSON *portName = cJSON_GetObjectItemCaseSensitive(uartinfo, "name");
    m_hplcPort->portName = std::string(portName->valuestring);
    zlog_info(m_logc, "m_plcPort->portName is %s ", m_hplcPort->portName.c_str());

    cJSON *baudrate = cJSON_GetObjectItemCaseSensitive(uartinfo, "baudrate");
    m_hplcPort->baudRate = baudrate->valueint;
    zlog_info(m_logc, "m_plcPort->baudRate is %d ", m_hplcPort->baudRate);

    cJSON *databit = cJSON_GetObjectItemCaseSensitive(uartinfo, "databit");
    m_hplcPort->dataBit = databit->valueint;
    zlog_info(m_logc, "m_plcPort->dataBit is %d ", m_hplcPort->dataBit);

    cJSON *stopbit = cJSON_GetObjectItemCaseSensitive(uartinfo, "stopbit");
    m_hplcPort->stopBit = stopbit->valueint;
    zlog_info(m_logc, "m_plcPort->stopBit is %d ", m_hplcPort->stopBit);

    cJSON *parity = cJSON_GetObjectItemCaseSensitive(uartinfo, "parity");
    m_hplcPort->parity = std::string(parity->valuestring);
    zlog_info(m_logc, "m_plcPort->parity is %s ", m_hplcPort->parity.c_str());

    cJSON *interval = cJSON_GetObjectItemCaseSensitive(uartinfo, "interval");
    m_hplcPort->interval = interval->valueint;
    zlog_info(m_logc, "m_plcPort->interval is %d ", m_hplcPort->interval);
    */

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
    }
    cJSON_Delete(root);
    return 0;
}
