#include "cco_control.h"

#include <array>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/utsname.h>
#include <thread>

CcoControl::CcoControl()
{
    m_logc = zlog_get_category("datacontrol");

    paraInit();
}

CcoControl::~CcoControl()
{
}

void CcoControl::paraInit()
{
    m_ccoTopicInfo[0xff] = {
        GET_MASTERNODE,
    };
}

int CcoControl::parseCcoData(const char *topic, const char *message)
{
    int reqType = -1;
    std::string str;
    str = topic;
    std::vector<std::string> topicStrs;
    split(str, "/", topicStrs);
    std::string sendAppName = topicStrs.at(0); // 发送过来的appname
    topicStrs.erase(topicStrs.begin());
    char cTopic[128] = {0};            // 对比topic字符串
    sysOutputTopic(topicStrs, cTopic); // 去除app名字的topic 去匹配
    zlog_info(m_logc, "cTopic : [%s]", cTopic);

    for (int i = 0; i < 0xff; i++)
    {
        if (cTopic == m_ccoTopicInfo[i])
        {
            reqType = i;
            break;
        }
    }

    if (reqType < 0)
    {
        return -1;
    }

    switch (reqType)
    {
    case GET_MASTERNODE_NUM:
        parseGetMasterNode(topic, message);
        break;

    default:
        break;
    }
}

int CcoControl::parseGetMasterNode(std::string topic, std::string message)
{
    int resToken = -1;
    std::string str;
    str = topic;
    std::vector<std::string> topicStrs;
    split(str, "/", topicStrs);
    std::swap(topicStrs[0], topicStrs[1]);
    char resTopic[128] = {0};            // 返回的topic
    sysOutputTopic(topicStrs, resTopic); // 去除app名字的topic 去匹配

    cJSON *root = cJSON_Parse(message.c_str());
    if (root == NULL)
    {
        zlog_error(m_logc, "cJson parse error");
    }
    cJSON *element;
    for (element = (root != NULL) ? root->child : NULL; element != NULL; element = element->next)
    {
        if (element->type == cJSON_Number)
        {
            if (strcmp(element->string, "token") == 0)
            {
                resToken = element->valueint;
                break;
            }
        }
    }
    cJSON_Delete(root);

    if (resToken < 0)
    {
        zlog_error(m_logc, "reqToken error! reqToken : [ %d ]", resToken);
        return -1;
    }
    
}