#include "cco_control.h"
#include "gw13762_task.h"
#include "protocol_gw1376_2_data.h"
#include "protocol_gw1376_2.h"
#include "common_utils.h"
#include "zlog.h"

#include <array>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/utsname.h>
#include <thread>
std::map<int, std::string> *CcoControl::m_noConcurrentMap_tmp = NULL;
std::map<std::string, std::string> *CcoControl::m_concurrentRes_tmp = NULL;

std::map<int, RES_TOKEN_INFO> *CcoControl::m_mqttResMap_tmp = NULL;
CcoControl::CcoControl()
{

    m_pdata = get_pdata();
    m_logc = zlog_get_category("ccoControl");
    m_token = 1;
    paraInit();
    m_mqttResMap_tmp = &m_mqttResMap;
    m_noConcurrentMap_tmp = &m_noConcurrentMap;
    m_concurrentRes_tmp = &m_concurrentRes;
    set_gettype_func(get13762TypeForToken);
    set_checkConcurrentIndex_func(checkConcurrentForIndex);
}

CcoControl::~CcoControl()
{
}

void CcoControl::setMqttControl(MqttControl *mqttControl, MQTT_INFO *mInfo)
{
    m_mqttControl = mqttControl;
    m_mInfo = mInfo;
}

int CcoControl::checkConcurrentForIndex(int token)
{
    std::map<int, std::string>::iterator noConcurrent = m_noConcurrentMap_tmp->find(token);
    // zlog_info(m_logc, "m_mqttResMap.size() : [ %d ]  m_mqttResMap[ %d ] resTopic : [%s]", m_mqttResMap.size(), resInfo->index, mqttRes->second.resTopic.c_str());
    // dzlog_info("resInfo->gw13762DataType : [%d]", resInfo->gw13762DataType);
    if (noConcurrent != m_noConcurrentMap_tmp->end())
    {
        std::string addr = noConcurrent->second;
        std::map<std::string, std::string>::iterator concurrentRes = m_concurrentRes_tmp->find(addr);
        if (concurrentRes != m_concurrentRes_tmp->end())
        {
            m_concurrentRes_tmp->erase(addr);
            // printf("m_concurrentRes 并发采集地址擦除 [%s]", addr);
        }
        m_noConcurrentMap_tmp->erase(token);
    }
    return 0;
}

int CcoControl::get13762TypeForToken(int token)
{
    std::map<int, RES_TOKEN_INFO>::iterator mqttRes = m_mqttResMap_tmp->find(token);
    // zlog_info(m_logc, "m_mqttResMap.size() : [ %d ]  m_mqttResMap[ %d ] resTopic : [%s]", m_mqttResMap.size(), resInfo->index, mqttRes->second.resTopic.c_str());
    // dzlog_info("resInfo->gw13762DataType : [%d]", resInfo->gw13762DataType);
    if (mqttRes != m_mqttResMap_tmp->end())
    {
        RES_TOKEN_INFO resTokenInfo = mqttRes->second;
        return resTokenInfo.gw13762Type;
    }
    else
    {
        return -1;
    }
}

void CcoControl::paraInit()
{
    m_ccoTopicInfo.emplace(GET_MASTERNODE, GET_MASTERNODE_ENUM);
    m_ccoTopicInfo.emplace(SET_MASTERNODE, SET_MASTERNODE_ENUM);
    m_ccoTopicInfo.emplace(SET_ACQ_ADD_FILES, SET_ACQ_ADD_FILES_ENUM);
    m_ccoTopicInfo.emplace(GET_ACQ_QUERY_FILES, GET_ACQ_QUERY_FILES_ENUM);
    m_ccoTopicInfo.emplace(ACTION_DEL_ACQFILES, ACTION_DEL_ACQFILES_ENUM);
    m_ccoTopicInfo.emplace(ACTION_CLEAR_ACQFILES, ACTION_CLEAR_ACQFILES_ENUM);
    m_ccoTopicInfo.emplace(GET_ACQFILES_NUM, GET_ACQFILES_NUM_ENUM);
    m_ccoTopicInfo.emplace(ACTION_EN_SEARCHMETER, ACTION_EN_SEARCHMETER_ENUM);
    m_ccoTopicInfo.emplace(ACTION_DIS_SEARCHMETER, ACTION_DIS_SEARCHMETER_ENUM);
    m_ccoTopicInfo.emplace(GET_MONITOR_NODEDELAY, GET_MONITOR_NODEDELAY_ENUM);
    m_ccoTopicInfo.emplace(ACTION_MONITOR_NODE, ACTION_MONITOR_NODE_ENUM);
    m_ccoTopicInfo.emplace(ACTION_CONCURRENT, ACTION_CONCURRENT_ENUM);
    m_ccoTopicInfo.emplace(ACTION_DATATRANS, ACTION_DATATRANS_ENUM);
    m_ccoTopicInfo.emplace(GET_NETTOPO_INFO, GET_NETTOPO_INFO_ENUM);
    m_ccoTopicInfo.emplace(GET_CHIP_INFORMATION, GET_CHIP_INFORMATION_ENUM);
    m_ccoTopicInfo.emplace(GET_MULTINET_INFORMATION, GET_MULTINET_INFORMATION_ENUM);
    m_ccoTopicInfo.emplace(GET_NODELINE_INFORMATION, GET_NODELINE_INFORMATION_ENUM);
    m_ccoTopicInfo.emplace(GET_NETWORK_SIZE, GET_NETWORK_SIZE_ENUM);
    m_ccoTopicInfo.emplace(GET_SLAVE_MODEID, GET_SLAVE_MODEID_ENUM);
    m_ccoTopicInfo.emplace(GET_HOST_MODEID, GET_HOST_MODEID_ENUM);
    m_ccoTopicInfo.emplace(GET_MODE_INFO, GET_MODE_INFO_ENUM);
    m_ccoTopicInfo.emplace(GET_NODE_VERSION, GET_NODE_VERSION_ENUM);
    m_ccoTopicInfo.emplace(SET_HPLC_FREQ, SET_HPLC_FREQ_ENUM);
    m_ccoTopicInfo.emplace(GET_HPLC_FREQ, GET_HPLC_FREQ_ENUM);
    m_ccoTopicInfo.emplace(ACTION_IDENTIAREA_CFG, ACTION_IDENTIAREA_CFG_ENUM);
    m_ccoTopicInfo.emplace(ACTION_NODEOFFLINE_CFG, ACTION_NODEOFFLINE_CFG_ENUM);
    m_ccoTopicInfo.emplace(ACTION_REFUSE_NODEREPORT_CFG, ACTION_REFUSE_NODEREPORT_CFG_ENUM);
    m_ccoTopicInfo.emplace(GET_BROADCAST_DELAY, GET_BROADCAST_DELAY_ENUM);
    m_ccoTopicInfo.emplace(ACTION_BROADCAST_CMD, ACTION_BROADCAST_CMD_ENUM);
    m_ccoTopicInfo.emplace(GET_METERING_STATE, GET_METERING_STATE_ENUM);
    m_ccoTopicInfo.emplace(ACTION_START_FILEUPGRADE, ACTION_START_FILEUPGRADE_ENUM);
    m_ccoTopicInfo.emplace(GET_FILEUPGRADE_STATE, GET_FILEUPGRADE_STATE_ENUM);
    m_ccoTopicInfo.emplace(ACTION_PAUSE_METERING, ACTION_PAUSE_METERING_ENUM);
    m_ccoTopicInfo.emplace(ACTION_RESUME_METERING, ACTION_RESUME_METERING_ENUM);
    m_ccoTopicInfo.emplace(ACTION_RESTART_METERING, ACTION_RESTART_METERING_ENUM);
    m_ccoTopicInfo.emplace(GET_MODULEINFO, GET_MODULEINFO_ENUM);
}

void CcoControl::dataInit(const char *topic, const char *message)
{
    parseCcoData(topic, message);
}

int CcoControl::parseCcoData(const char *topic, const char *message)
{
    int reqType = -1;
    std::string str;
    str = topic;
    std::vector<std::string> topicStrs;
    split(str, "/", topicStrs);
    topicStrs.erase(topicStrs.begin());
    char cTopic[128] = {0};            // 对比topic字符串
    sysOutputTopic(topicStrs, cTopic); // 去除app名字的topic 去匹配
    zlog_info(m_logc, "cTopic : [%s]", cTopic);

    RES_TOKEN_INFO resTokenInfo;
    int resToken = -1;
    std::string strtop;
    char resTopic[128] = {0};
    strtop = topic;
    std::vector<std::string> topicStrs1;
    split(strtop, "/", topicStrs1);
    std::swap(topicStrs1[0], topicStrs1[1]); // 交换
    topicStrs1[4] = "response";
    sysOutputTopic(topicStrs1, resTopic);

    resTokenInfo.resTopic = resTopic;

    cJSON *root = cJSON_Parse(message);
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
    }

    cJSON *cjson_token = cJSON_GetObjectItemCaseSensitive(root, "token");
    if (!cJSON_IsNumber(cjson_token))
    {
        dzlog_error("cJson parse error");
        return -1;
    }
    resTokenInfo.mqttToken = cjson_token->valueint;

    cJSON_Delete(root);

    auto it = m_ccoTopicInfo.find(cTopic);
    if (it == m_ccoTopicInfo.end())
    {
        zlog_info(m_logc, "cTopic : [%s] not found !", cTopic);
        return -1;
    }

    reqType = it->second;

    int res = -1;
    switch (reqType)
    {
    case GET_MASTERNODE_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR;
        res = parseGetMasterNode(topic, message);
    }
    break;

    case SET_MASTERNODE_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR;
        res = parseSetMasterNode(topic, message);
    }
    break;
    case SET_ACQ_ADD_FILES_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE;
        res = parseSetAcqAddFiles(topic, message);
    }
    break;
    case GET_ACQ_QUERY_FILES_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO;
        res = parseGetAcqQueryFiles(topic, message);
    }
    break;
    case ACTION_DEL_ACQFILES_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_ROUTE_DEL_SUBNODE;
        res = parseDelAcqFiles(topic, message);
    }

    break;
    case ACTION_CLEAR_ACQFILES_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_PARAM_INIT;
        res = parseClearAcqFiles(topic, message);
    }

    break;
    case GET_ACQFILES_NUM_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM;
        res = parseGetAcqFilesNum(topic, message);
    }
    break;
    case ACTION_EN_SEARCHMETER_ENUM:

        break;
    case ACTION_DIS_SEARCHMETER_ENUM:

        break;
    case GET_MONITOR_NODEDELAY_ENUM:

        break;
    case ACTION_MONITOR_NODE_ENUM:

        break;
    case ACTION_CONCURRENT_ENUM:
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_CONCURRENT_METER_READING;
        res = parseActionConCurrent(resTopic, message);
    }
    break;
    case ACTION_DATATRANS_ENUM:
        // res = parseActionDataTrans(topic, message);
        break;
    case GET_NETTOPO_INFO_ENUM:

        break;
    case GET_CHIP_INFORMATION_ENUM:

        break;
    case GET_MULTINET_INFORMATION_ENUM:

        break;
    case GET_NODELINE_INFORMATION_ENUM:

        break;
    case GET_NETWORK_SIZE_ENUM:

        break;
    case GET_HOST_MODEID_ENUM:

        break;
    case GET_SLAVE_MODEID_ENUM:

        break;
    case GET_MODE_INFO_ENUM:

        break;
    case GET_NODE_VERSION_ENUM:

        break;
    case SET_HPLC_FREQ_ENUM:

        break;
    case GET_HPLC_FREQ_ENUM:

        break;
    case ACTION_IDENTIAREA_CFG_ENUM:

        break;
    case ACTION_NODEOFFLINE_CFG_ENUM:

        break;
    case ACTION_REFUSE_NODEREPORT_CFG_ENUM: // 禁止/允许节点上报
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP;
        res = parseActionRefuseNodeReport(topic, message);
    }

    break;
    case GET_BROADCAST_DELAY_ENUM:

        break;
    case ACTION_BROADCAST_CMD_ENUM: // 广播
    {
        resTokenInfo.gw13762Type = GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA;
        res = parseActionBroadcastCmd(topic, message);
    }
    break;
    case GET_METERING_STATE_ENUM:

        break;
    case ACTION_START_FILEUPGRADE_ENUM:

        break;
    case GET_FILEUPGRADE_STATE_ENUM:

        break;
    case ACTION_PAUSE_METERING_ENUM:

        break;

    case ACTION_RESUME_METERING_ENUM:

        break;
    case ACTION_RESTART_METERING_ENUM:

        break;
    case GET_MODULEINFO_ENUM:

        break;

    default:
        res = -1;
        break;
    }
    dzlog_info("=======>mqtt recv token : [%d] , send 13762 token : [%d]  ", resTokenInfo.mqttToken, res);
    if (res >= 0)
    {
        resTokenInfo.proToken = res;
        m_mqttResMap.emplace(resTokenInfo.proToken, resTokenInfo);
    }
    return res;
}

int CcoControl::parseGetMasterNode(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR;
    data.ismanu_index = false;
    int gw13762Index = gw13762_task_push(ptask, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseSetMasterNode(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }

    cJSON *pmasterNode = cJSON_GetObjectItemCaseSensitive(root, "masterNode");
    if (!cJSON_IsString(pmasterNode))
    {
        zlog_error(m_logc, "cJson parse error");
        return 0;
    }
    std::string masterNode = pmasterNode->valuestring;
    int len = 0;
    char addr[6] = {0};
    stringToHexArray(masterNode.c_str(), addr, &len);
    hdzlog_info(addr, 6);
    cJSON_Delete(root);

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR;
    data.ismanu_index = false;
    memcpy(data.buf, addr, sizeof(addr));
    data.len = sizeof(addr);
    int gw13762Index = gw13762_task_push(ptask, &data);
    // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseSetAcqAddFiles(std::string topic, std::string message)
{
    std::map<std::string, int> addrs; // addr proType
    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }

    cJSON *bodys = cJSON_GetObjectItemCaseSensitive(root, "body");
    if (!cJSON_IsArray(bodys))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    cJSON *body = NULL;
    char addAddr[1024] = {0};
    int index = 0;

    addAddr[index] = cJSON_GetArraySize(bodys);
    index++;
    cJSON_ArrayForEach(body, bodys)
    {
        cJSON *acqAddr = NULL;
        cJSON *macAddr = NULL;
        cJSON *proType = NULL;

        acqAddr = cJSON_GetObjectItemCaseSensitive(body, "acqAddr");
        if (!cJSON_IsString(acqAddr))
        {
            dzlog_debug(" acqAddr null!");
            return -1;
        }
        int len;
        char addr[6] = {0};
        stringToHexArray(acqAddr->valuestring, addr, &len);
        reverseHexArraycom(addAddr + index, addr, sizeof(addr));
        index += 6;

        proType = cJSON_GetObjectItemCaseSensitive(body, "proType");
        if (!cJSON_IsNumber(proType))
        {
            dzlog_debug(" proType null!");
            return -1;
        }
        addAddr[index] = proType->valueint;
        index++;

        macAddr = cJSON_GetObjectItemCaseSensitive(body, "macAddr");
        if (!cJSON_IsString(macAddr))
        {
            dzlog_debug(" macAddr null!");
            return -1;
        }

        addrs.emplace(acqAddr->valuestring, proType->valueint);
        dzlog_info("acqAddr : [%s] macAddr : [%s] proType : [%d]", acqAddr->valuestring, macAddr->valuestring, proType->valueint);
    }
    cJSON_Delete(root);

    auto addr = addrs.begin();

    int flag = 0;
    for (int i = 0; i < addrs.size(); i++)
    {
        // addr = addr + i;
        auto it = m_fileInfosMap.find(addr->first);
        if (it != m_fileInfosMap.end())
        {
            flag = 1;
        }
        addr++;
    }
    if (flag == 0)
    {
        m_fileInfosMap.insert(addrs.begin(), addrs.end());
    }

    for (const auto &pair : m_fileInfosMap)
    {
        // std::cout << pair.first << " => " << pair.second << std::endl;
        zlog_info(m_logc, "addr : [%s] proType : [%d]", pair.first.c_str(), pair.second);
    }

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE;
    data.ismanu_index = false;
    memcpy(data.buf, addAddr, index);
    data.len = index;
    int gw13762Index = gw13762_task_push(ptask, &data);
    // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseGetAcqQueryFiles(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
    }

    cJSON *startIndex = cJSON_GetObjectItemCaseSensitive(root, "startIndex");
    if (!cJSON_IsNumber(startIndex))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    int start = startIndex->valueint;

    cJSON *curMeterNum = cJSON_GetObjectItemCaseSensitive(root, "curMeterNum");
    if (!cJSON_IsNumber(curMeterNum))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    int fileNum = curMeterNum->valueint;
    cJSON_Delete(root);

    char buf[3] = {0};
    buf[0] = start & 0xff;        // 取后八位
    buf[1] = (start >> 8) & 0xff; // 取前八位
    buf[2] = fileNum;
    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO;
    data.ismanu_index = false;
    memcpy(data.buf, buf, sizeof(buf));
    data.len = sizeof(buf);
    int gw13762Index = gw13762_task_push(ptask, &data);
    // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseDelAcqFiles(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }

    cJSON *bodys = cJSON_GetObjectItemCaseSensitive(root, "body");
    if (!cJSON_IsArray(bodys))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    cJSON *body = NULL;
    char delAddr[1024] = {0};
    int index = 0;

    delAddr[index] = cJSON_GetArraySize(bodys);
    index++;
    char addr[6] = {0};
    cJSON_ArrayForEach(body, bodys)
    {
        memset(addr, 0, sizeof(addr));
        cJSON *acqAddr = NULL;

        acqAddr = cJSON_GetObjectItemCaseSensitive(body, "acqAddr");
        if (!cJSON_IsString(acqAddr))
        {
            dzlog_debug(" acqAddr null!");
            return -1;
        }
        int len;

        stringToHexArray(acqAddr->valuestring, addr, &len);
        reverseHexArraycom(delAddr + index, addr, sizeof(addr));
        index += 6;
        dzlog_info("acqAddr : [%s] ", acqAddr->valuestring);

        auto it = m_fileInfosMap.find(acqAddr->valuestring);
        if (it != m_fileInfosMap.end())
        {
            m_fileInfosMap.erase(acqAddr->valuestring);
        }
    }
    cJSON_Delete(root);

    for (const auto &pair : m_fileInfosMap)
    {
        // std::cout << pair.first << " => " << pair.second << std::endl;
        zlog_info(m_logc, "addr : [%s] proType : [%d]", pair.first.c_str(), pair.second);
    }

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_ROUTE_DEL_SUBNODE;
    data.ismanu_index = false;
    memcpy(data.buf, delAddr, index);
    data.len = index;
    int gw13762Index = gw13762_task_push(ptask, &data);
    // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseActionConCurrent(std::string topic, std::string message)
{
    std::string str;
    str = topic;
    std::vector<std::string> topicStrs;
    split(str, "/", topicStrs);
    topicStrs[3] = "report";
    topicStrs[4] = "notification";
    topicStrs[5] = "reportConcurrent";
    char cTopic[128] = {0};            // 对比topic字符串
    sysOutputTopic(topicStrs, cTopic); // 去除app名字的topic 去匹配

    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }

    cJSON *acqAddr = cJSON_GetObjectItemCaseSensitive(root, "acqAddr");
    if (!cJSON_IsString(acqAddr))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    char addr[6] = {0};
    int len;
    stringToHexArray(acqAddr->valuestring, addr, &len);
    cJSON *dataCJSON = cJSON_GetObjectItemCaseSensitive(root, "data");
    if (!cJSON_IsString(dataCJSON))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    std::string base64Data = decode_base64((unsigned char *)(dataCJSON->valuestring), strlen(dataCJSON->valuestring));
    char base64DataBuf[2048];
    char tmpBuf[2048];
    sprintf(tmpBuf, "%s", base64Data.c_str());
    // printf("String: %s\n", tmpBuf); // 输出 "Hello"
    int bufLen;
    stringToHexArray(tmpBuf, base64DataBuf, &bufLen);
    hdzlog_info(base64DataBuf, bufLen);

    int gw13762Index = 0;

    auto it = m_fileInfosMap.find(std::string(acqAddr->valuestring));

    if (it != m_fileInfosMap.end())
    {
        dzlog_info("acqAddr : [%s]", acqAddr->valuestring);
        auto conIt = m_concurrentRes.find(acqAddr->valuestring);
        if (conIt != m_concurrentRes.end())
        {
            /**
            if()
            struct timeval nowTime;
            gettimeofday(&nowTime, NULL); // 获取当前时间
            int timeouts = nowTime.tv_sec - pdev_data->startTime.tv_sec;
            m_delConResMap.emplace();
    */
            cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");
            if (!cJSON_IsNumber(token))
            {
                zlog_error(m_logc, "cJson parse error");
                return -1;
            }
            cJSON *sendRoot = cJSON_CreateObject();

            addResObject(sendRoot, token->valueint);
            cJSON_AddItemToObject(sendRoot, "status", cJSON_CreateNumber(-1));
            cJSON_AddItemToObject(sendRoot, "reason", cJSON_CreateString("In READ_METER State"));
            char *payload = NULL;
            payload = cJSON_Print(sendRoot);

            int bRet = m_mqttControl->mqttPublish(masterserver, topic.c_str(), strlen(payload), payload, m_mInfo->nqos, false);
            if (!bRet)
            {
                zlog_error(m_logc, "publish topic %s error", topic.c_str());
            }
            zlog_info(m_logc, "ccoRouter response : topic : [%s]\n%s", topic.c_str(), payload);
            cJSON_Delete(sendRoot);
            free(payload);
            cJSON_Delete(root);
            return -1;
        }
        int index = 0;
        char sendData[1024] = {0};
        GW13762_TASK_DATA data;
        memset(&data, 0, sizeof(GW13762_TASK_DATA));
        data.type = GW1376_2_DATA_TYPE_CONCURRENT_METER_READING;
        data.ismanu_index = false;
        reverseHexArraycom(data.sta_addr, addr, sizeof(addr));
        sendData[index] = it->second;
        index++;
        sendData[index] = 0;
        index++;

        sendData[index] = bufLen & 0xff;
        index++;
        sendData[index] = bufLen >> 8 & 0xff;
        index++;

        memcpy(sendData + index, base64DataBuf, bufLen);
        index += bufLen;

        memcpy(data.buf, sendData, index);
        data.len = index;
        gw13762Index = gw13762_task_push(ptask, &data);
        // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
        if (gw13762Index < 0)
        {
            zlog_error(m_logc, "gw13762_task_push error!");
        }
        else
        {
            // std::string str = std::string(acqAddr->valuestring) + "_" + std::to_string(gw13762Index);
            m_concurrentRes.emplace(acqAddr->valuestring, cTopic);
            m_noConcurrentMap.emplace(gw13762Index, acqAddr->valuestring);
        }
    }
    else
    {
        gw13762Index = -1;
        // 组报文发送档案冲突
        cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");
        if (!cJSON_IsNumber(token))
        {
            zlog_error(m_logc, "cJson parse error");
            return -1;
        }
        cJSON *sendRoot = cJSON_CreateObject();

        addResObject(sendRoot, token->valueint);
        cJSON_AddItemToObject(sendRoot, "status", cJSON_CreateNumber(-1));
        cJSON_AddItemToObject(sendRoot, "reason", cJSON_CreateString("Meter Conflict"));
        char *payload = NULL;
        payload = cJSON_Print(sendRoot);

        int bRet = m_mqttControl->mqttPublish(masterserver, topic.c_str(), strlen(payload), payload, m_mInfo->nqos, false);
        if (!bRet)
        {
            zlog_error(m_logc, "publish topic %s error", topic.c_str());
        }
        zlog_info(m_logc, "ccoRouter response : topic : [%s]\n%s", topic.c_str(), payload);
        cJSON_Delete(sendRoot);
        free(payload);
    }
    cJSON_Delete(root);
    return gw13762Index;
}

int CcoControl::parseActionDataTrans(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }

    cJSON *bodys = cJSON_GetObjectItemCaseSensitive(root, "body");
    if (!cJSON_IsArray(bodys))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    cJSON *body = NULL;
    char delAddr[1024] = {0};
    int index = 0;

    delAddr[index] = cJSON_GetArraySize(bodys);
    index++;
    char addr[6] = {0};
    cJSON_ArrayForEach(body, bodys)
    {
        memset(addr, 0, sizeof(addr));
        cJSON *acqAddr = NULL;

        acqAddr = cJSON_GetObjectItemCaseSensitive(body, "acqAddr");
        if (!cJSON_IsString(acqAddr))
        {
            dzlog_debug(" acqAddr null!");
            return -1;
        }
        int len;

        stringToHexArray(acqAddr->valuestring, addr, &len);
        reverseHexArraycom(delAddr + index, addr, sizeof(addr));
        index += 6;
        dzlog_info("acqAddr : [%s] ", acqAddr->valuestring);
    }
    cJSON_Delete(root);

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_ROUTE_DEL_SUBNODE;
    data.ismanu_index = false;
    memcpy(data.buf, delAddr, index);
    data.len = index;
    int gw13762Index = gw13762_task_push(ptask, &data);
    // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseClearAcqFiles(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_PARAM_INIT;
    data.ismanu_index = false;
    int gw13762Index = gw13762_task_push(ptask, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    m_fileInfosMap.clear();
    for (const auto &pair : m_fileInfosMap)
    {
        // std::cout << pair.first << " => " << pair.second << std::endl;
        zlog_info(m_logc, "addr : [%s] proType : [%d]", pair.first.c_str(), pair.second);
    }
    return gw13762Index;
}

int CcoControl::parseGetAcqFilesNum(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM;
    data.ismanu_index = false;
    int gw13762Index = gw13762_task_push(ptask, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseActionRefuseNodeReport(std::string topic, std::string message)
{
    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
    }

    cJSON *autoUpSwitch = cJSON_GetObjectItemCaseSensitive(root, "switch");
    if (!cJSON_IsNumber(autoUpSwitch))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    int autoUpFlag = autoUpSwitch->valueint;
    cJSON_Delete(root);

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP;
    data.ismanu_index = false;
    data.buf[0] = autoUpFlag;
    data.len = 1;
    int gw13762Index = gw13762_task_push(ptask, &data);
    // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

int CcoControl::parseActionBroadcastCmd(std::string topic, std::string message)
{

    GW13762_TASK *ptask = &m_pdata->task;
    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
    }
    cJSON *proType = cJSON_GetObjectItemCaseSensitive(root, "proType");
    if (!cJSON_IsNumber(proType))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }

    cJSON *dataCJSON = cJSON_GetObjectItemCaseSensitive(root, "data");
    if (!cJSON_IsString(dataCJSON))
    {
        zlog_error(m_logc, "cJson parse error");
        return -1;
    }
    std::string base64Data = decode_base64((unsigned char *)(dataCJSON->valuestring), strlen(dataCJSON->valuestring));
    char base64DataBuf[2048];
    char tmpBuf[2048];
    sprintf(tmpBuf, "%s", base64Data.c_str());
    // printf("String: %s\n", tmpBuf); // 输出 "Hello"
    int bufLen;
    stringToHexArray(tmpBuf, base64DataBuf, &bufLen);
    hdzlog_info(base64DataBuf, bufLen);

    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(GW13762_TASK_DATA));
    data.type = GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA;
    data.ismanu_index = false;

    data.buf[0] = proType->valueint;
    data.buf[1] = bufLen;
    memcpy(&data.buf[2], base64DataBuf, bufLen);
    data.len = bufLen + 2;
    int gw13762Index = gw13762_task_push(ptask, &data);
    // protocol_gw1376_2_data_set_send_task_data(m_pdata, &data);
    if (gw13762Index < 0)
    {
        zlog_error(m_logc, "gw13762_task_push error!");
        return -1;
    }
    return gw13762Index;
}

/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
/***********打包发送mqtt报文*************/
int CcoControl::packSendMqttMsg(void *data, int dataSize)
{
    std::string topic;
    int res = 0;
    RES_INFO *resInfo = (RES_INFO *)data;
    cJSON *root = cJSON_CreateObject();
    if (resInfo->gw13762DataType == GW1376_2_DATA_TYPE_CONCURRENT_METER_READING && resInfo->isReport == true)
    {
        CONCURRENT_INFO *concurrentInfo = (CONCURRENT_INFO *)resInfo->info;
        char addrTmp[6] = {0};
        reverseHexArraycom(addrTmp, concurrentInfo->addr, sizeof(addrTmp));
        char strAddr[13] = {0};
        hexArrayToString(addrTmp, sizeof(addrTmp), strAddr);
        auto it = m_concurrentRes.find(std::string(strAddr));
        if (it == m_concurrentRes.end())
        {
            zlog_error(m_logc, "m_concurrentRes not found!");
            cJSON_Delete(root);
            return -1;
        }
        topic = it->second;
        addPubilcObject(root);
        cJSON_AddItemToObject(root, "acqAddr", cJSON_CreateString(strAddr));
        cJSON_AddItemToObject(root, "proType", cJSON_CreateNumber(concurrentInfo->proType));

        char *str = (char *)malloc((concurrentInfo->bufLen * 2 + 1) * sizeof(char));

        hexArrayToString(concurrentInfo->buffer, concurrentInfo->bufLen, str);
        std::string encodeBuf = encode_base64((unsigned char *)str, concurrentInfo->bufLen * 2);
        cJSON_AddItemToObject(root, "data", cJSON_CreateString(encodeBuf.c_str()));
        free(str);

        m_concurrentRes.erase(std::string(strAddr));
        dzlog_info("m_concurrentRes 并发采集地址擦除 [%s]", strAddr);
        // m_noConcurrentMap.erase(resInfo->index);
    }
    else if (resInfo->gw13762DataType == GW1376_2_DATA_TYPE_AUTOUP)
    {
        topic = "ccoRouter/Broadcast/JSON/report/notification/event";
        AUTOUP_STA_EVENT_INFO *staEventInfo = (AUTOUP_STA_EVENT_INFO *)resInfo->info;
        addPubilcObject(root);
        cJSON_AddItemToObject(root, "devType", cJSON_CreateNumber(staEventInfo->staDevType));
        cJSON_AddItemToObject(root, "proType", cJSON_CreateNumber(staEventInfo->proType));

        char *str = (char *)malloc((staEventInfo->bufferLen * 2 + 1) * sizeof(char));
        hexArrayToString(staEventInfo->buffer, staEventInfo->bufferLen, str);
        std::string encodeBuf = encode_base64((unsigned char *)str, staEventInfo->bufferLen * 2);
        cJSON_AddItemToObject(root, "data", cJSON_CreateString(encodeBuf.c_str()));
        free(str);
    }
    else
    {
        std::map<int, RES_TOKEN_INFO>::iterator mqttRes = m_mqttResMap.find(resInfo->index);
        // zlog_info(m_logc, "m_mqttResMap.size() : [ %d ]  m_mqttResMap[ %d ] resTopic : [%s]", m_mqttResMap.size(), resInfo->index, mqttRes->second.resTopic.c_str());
        dzlog_info("resInfo->gw13762DataType : [%d]", resInfo->gw13762DataType);
        if (mqttRes != m_mqttResMap.end())
        {
            RES_TOKEN_INFO resTokenInfo = mqttRes->second;

            topic = resTokenInfo.resTopic;
            addResObject(root, resTokenInfo.mqttToken);
            //   addPubilcObject(root);

            switch (resInfo->gw13762DataType)
            {
            case GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR:
                res = sendGetMasterNode(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR:
                res = sendCommonResponse(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE:
                res = sendCommonResponse(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM:
                res = sendGetAcqFilesNum(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO:
                res = sendGetAcqFilesInfo(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_ROUTE_DEL_SUBNODE:
                res = sendCommonResponse(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_PARAM_INIT:
                res = sendCommonResponse(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_CONCURRENT_METER_READING:
            {
                res = sendCommonResponse(root, resInfo->info);
            }
            break;
            case GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP:
                res = sendCommonResponse(root, resInfo->info);
                break;
            case GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA:
                res = sendCommonResponse(root, resInfo->info);
                break;
            default:
                res = -1;
                break;
            }
            if (resInfo->gw13762DataType != GW1376_2_DATA_TYPE_CONCURRENT_METER_READING)
            {
                m_mqttResMap.erase(resInfo->index);
            }
        }
        else
        {
            zlog_error(m_logc, "m_mqttResMap not found!");
            cJSON_Delete(root);
            return -1;
        }
    }
    char *payload = NULL;
    payload = cJSON_Print(root);
    if (res >= 0)
    {
        int bRet = m_mqttControl->mqttPublish(masterserver, topic.c_str(), strlen(payload), payload, m_mInfo->nqos, false);
        if (!bRet)
        {
            zlog_error(m_logc, "publish topic %s error", topic.c_str());
        }
        zlog_info(m_logc, "ccoRouter response : topic : [%s]\n%s", topic.c_str(), payload);
    }
    else
    {
        zlog_error(m_logc, "not found");
    }
    cJSON_Delete(root);
    free(payload);
    return 0;
}

int CcoControl::sendGetMasterNode(cJSON *root, void *addr)
{
    // hdzlog_info((char *)addr, 6);
    char msg[13] = {0};
    hexArrayToString((char *)addr, 6, msg);
    cJSON_AddItemToObject(root, "masterNode", cJSON_CreateString(msg));
    return 0;
}

int CcoControl::sendSetMasterNode(cJSON *root, void *info)
{
    // hdzlog_info((char *)addr, 6);
    sendCommonResponse(root, info);
}

int CcoControl::sendGetAcqFilesNum(cJSON *root, void *number)
{

    cJSON_AddItemToObject(root, "acqNum", cJSON_CreateNumber(*(int *)number));
    return 0;
}

int CcoControl::sendGetAcqFilesInfo(cJSON *root, void *info)
{
    if (info == NULL)
    {
        zlog_error(m_logc, "info is null");
        return -1;
    }
    ACQ_FILES_INFO *acqFilesInfo = (ACQ_FILES_INFO *)info;
    cJSON *body = cJSON_CreateArray();

    if (acqFilesInfo->validNum != 0)
    {
        char addr[6] = {0};
        char msg[13] = {0};

        for (int i = 0; i < acqFilesInfo->validNum; i++)
        {

            memset(addr, 0, sizeof(addr));
            memset(msg, 0, sizeof(msg));

            cJSON *item = cJSON_CreateObject();
            hexArrayToString(acqFilesInfo->fileInfos[i].addr, 6, msg);

            cJSON_AddItemToObject(item, "acqAddr", cJSON_CreateString(msg));
            cJSON_AddItemToObject(item, "macAddr", cJSON_CreateString("000000000000"));
            cJSON_AddItemToObject(item, "proType", cJSON_CreateNumber(acqFilesInfo->fileInfos[i].proType));
            cJSON_AddItemToArray(body, item);

            auto it = m_fileInfosMap.find(msg);
            if (it == m_fileInfosMap.end())
            {
                m_fileInfosMap.emplace(msg, acqFilesInfo->fileInfos[i].proType);
            }
            else
            {
                if (it->second != acqFilesInfo->fileInfos[i].proType)
                {
                    it->second = acqFilesInfo->fileInfos[i].proType;
                }
            }
        }
    }

    cJSON_AddItemToObject(root, "body", body);
    if (acqFilesInfo->fileInfos != NULL)
    {
        free(acqFilesInfo->fileInfos);
    }

    for (const auto &pair : m_fileInfosMap)
    {
        // std::cout << pair.first << " => " << pair.second << std::endl;
        zlog_info(m_logc, "addr : [%s] proType : [%d]", pair.first.c_str(), pair.second);
    }
    return 0;
}

int CcoControl::sendCommonResponse(cJSON *root, void *info)
{
    // printf("============================>111");
    AFN00_INFO *afnInfo = (AFN00_INFO *)info;
    int status;
    std::string reason;

    if (afnInfo->Fn == 1)
    {
        status = 0;
        reason = "OK";
    }
    else if (afnInfo->Fn == 2)
    {
        status = -1;
        reason = getReason(afnInfo->result);
    }

    dzlog_info("AFN00_INFO->AFn : [%d]  AFN00_INFO->Fn : [%d]  afnInfo->result : [%d]", afnInfo->AFN, afnInfo->Fn, afnInfo->result);

    cJSON_AddItemToObject(root, "status", cJSON_CreateNumber(status));
    cJSON_AddItemToObject(root, "reason", cJSON_CreateString(reason.c_str()));
    return 0;
}

std::string CcoControl::getReason(int result)
{
    std::string str;
    switch (result)
    {
    case 0:
        zlog_info(m_logc, "通讯超时!");
        str = "CCO No response";
        break;
    case 1:
        zlog_info(m_logc, "无效数据单元!");
        str = "13 Permission";
        break;
    case 2:
        zlog_info(m_logc, "长度错!");
        str = "13 Permission";
        break;
    case 3:
        zlog_info(m_logc, "校验错误!");
        str = "13 Permission";
        break;
    case 4:
        zlog_info(m_logc, "信息类不存在!");
        str = "13 Permission";
        break;
    case 5:
        zlog_info(m_logc, "格式错误!");
        str = "13 Permission";
        break;
    case 6:
        zlog_info(m_logc, "表号重复!");
        str = "Meter Conflict";
        break;
    case 7:
        zlog_info(m_logc, "表号不存在!");
        str = "Meter Conflict";
        break;
    case 8:
        zlog_info(m_logc, "电表应用层无应答!");
        str = "CCO No response";
        break;
    case 9:
        zlog_info(m_logc, "主节点忙!");
        str = "CCO BUSY";
        break;
    case 10:
        zlog_info(m_logc, "主节点不支持此命令!");
        str = "13 Permission";
        break;

    case 11:
        zlog_info(m_logc, "从节点不应答!");
        str = "CCO No response";
        break;
    case 12:
        zlog_info(m_logc, "从节点不在网内!");
        str = "13 Permission";
        break;
    case 109:
        zlog_info(m_logc, "超过最大并发数!");
        str = "13 Permission";
        break;
    case 110:
        zlog_info(m_logc, "超过单个1376.2帧最大允许的电表协议报文条数!");
        str = "13 Permission";
        break;
    case 111:
        zlog_info(m_logc, "正在抄读该表!");
        str = "In READ_METER State";
        break;
    default:
        zlog_info(m_logc, "主节点忙!");
        str = "CCO BUSY";
        break;
    }
    return str;
}

int CcoControl::addResObject(cJSON *root, int token)
{
    cJSON_AddItemToObject(root, "token", cJSON_CreateNumber(token));
    char time[64];
    sys_gettime_timestamp(time);
    cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(time));
    return token;
}

int CcoControl::addPubilcObject(cJSON *root)
{
    int token = m_token;
    m_token++;
    cJSON_AddItemToObject(root, "token", cJSON_CreateNumber(token));
    char time[64];
    sys_gettime_timestamp(time);

    cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(time));
    return token;
}