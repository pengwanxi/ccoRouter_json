#ifndef _CCOCONTROL_H_
#define _CCOCONTROL_H_

#include "globalbase.h"

#include "zlog.h"
#include "common_utils.h"
#include "cJSON.h"
#include "timernode.h"
#include <map>
#include <unordered_map>

#define GET_MASTERNODE "ccoRouter/JSON/get/request/masterNode"

typedef enum
{
    GET_MASTERNODE_NUM = 0,
} REQ_TYPE;

class CcoControl
{
public:
    CcoControl();
    ~CcoControl();

    int parseCcoData(const char *topic, const char *message);

private:
    std::map<int, RES_TOKEN_INFO> resMap; // 用于mqtt返回 第一个是1376.2下发的token  第二个是返回的mqtt的token和restopic结构体
    zlog_category_t *m_logc;                               // zlog 指針
    std::string m_ccoTopicInfo[0xff];
    void paraInit();

    int parseGetMasterNode(std::string topic, std::string message); // 处理解析 查询主节点地址
};

#endif