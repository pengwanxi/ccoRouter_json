#ifndef _CCOCONTROL_H_
#define _CCOCONTROL_H_

#include "globalbase.h"
#include "protocol_gw1376_2_data.h"
#include "zlog.h"
#include "common_utils.h"
#include "cJSON.h"
#include "timernode.h"
#include "mqtt_control.h"
#include <map>
#include <unordered_map>

#define GET_MASTERNODE "ccoRouter/JSON/get/request/masterNode"
#define SET_MASTERNODE "ccoRouter/JSON/set/request/masterNode"
#define SET_ACQ_ADD_FILES "ccoRouter/JSON/set/request/acqAddFiles"
#define GET_ACQ_QUERY_FILES "ccoRouter/JSON/get/request/acqQueryFiles"
#define ACTION_DEL_ACQFILES "ccoRouter/JSON/action/request/delAcqFiles"
#define ACTION_CLEAR_ACQFILES "ccoRouter/JSON/action/request/clearAcqFiles"
#define GET_ACQFILES_NUM "ccoRouter/JSON/get/request/acqFilesNum"
#define ACTION_EN_SEARCHMETER "ccoRouter/JSON/action/request/enSearchMeter"
#define ACTION_DIS_SEARCHMETER "ccoRouter/JSON/action/request/disSearchMeter"
#define GET_MONITOR_NODEDELAY "ccoRouter/JSON/get/request/monitorNodeDelay"
#define ACTION_MONITOR_NODE "ccoRouter/JSON/action/request/monitorNode"
#define ACTION_CONCURRENT "ccoRouter/JSON/action/request/concurrent"
#define ACTION_DATATRANS "ccoRouter/JSON/action/request/dataTrans"
#define GET_NETTOPO_INFO "ccoRouter/JSON/get/request/netTopoInfo"
#define GET_CHIP_INFORMATION "ccoRouter/JSON/get/request/chipInformation"
#define GET_MULTINET_INFORMATION "ccoRouter/JSON/get/request/multiNetInformation"
#define GET_NODELINE_INFORMATION "ccoRouter/JSON/get/request/nodeLineInformation"
#define GET_NETWORK_SIZE "ccoRouter/JSON/get/request/networkSize"
#define GET_SLAVE_MODEID "ccoRouter/JSON/get/request/slaveModeID"
#define GET_HOST_MODEID "ccoRouter/JSON/get/request/hostModeID"
#define GET_MODE_INFO "ccoRouter/JSON/get/request/modeInfo"
#define GET_NODE_VERSION "ccoRouter/JSON/get/request/nodeversion"
#define SET_HPLC_FREQ "ccoRouter/JSON/set/request/hplcFreq"
#define GET_HPLC_FREQ "ccoRouter/JSON/get/request/hplcFreq"
#define ACTION_IDENTIAREA_CFG "ccoRouter/JSON/action/request/identiAreaCfg"
#define ACTION_NODEOFFLINE_CFG "ccoRouter/JSON/action/request/nodeOfflineCfg"
#define ACTION_REFUSE_NODEREPORT_CFG "ccoRouter/JSON/action/request/refuseNodeReportCfg"
#define GET_BROADCAST_DELAY "ccoRouter/JSON/get/request/BroadcastDelay"
#define ACTION_BROADCAST_CMD "ccoRouter/JSON/action/request/BroadcastCmd"
#define GET_METERING_STATE "ccoRouter/JSON/get/request/meteringState"
#define ACTION_START_FILEUPGRADE "ccoRouter/JSON/action/request/startFileUpgrade"
#define GET_FILEUPGRADE_STATE "ccoRouter/JSON/get/request/fileUpgradeState"
#define ACTION_PAUSE_METERING "ccoRouter/JSON/action/request/pauseMetering"
#define ACTION_RESUME_METERING "ccoRouter/JSON/action/request/resumeMetering"
#define ACTION_RESTART_METERING "ccoRouter/JSON/action/request/restartMetering"
#define GET_MODULEINFO "ccoRouter/JSON/get/request/moduleInfo"

typedef enum
{
    GET_MASTERNODE_NUM = 0,
    SET_MASTERNODE_NUM,
    SET_ACQ_ADD_FILES_NUM,
    GET_ACQ_QUERY_FILES_NUM,
    ACTION_DEL_ACQFILES_NUM,
    ACTION_CLEAR_ACQFILES_NUM,
    GET_ACQFILES_NUM_NUM,
    REPORT_METERERROR_NUM,
    REPORT_EVENT_NUM,
    ACTION_EN_SEARCHMETER_NUM,
    ACTION_DIS_SEARCHMETER_NUM,
    REPORT_SEARCHMETER_NUM,
    GET_MONITOR_NODEDELAY_NUM,
    ACTION_MONITOR_NODE_NUM,
    ACTION_CONCURRENT_NUM,
    REPORT_REPORT_CONCURRENT_NUM,
    ACTION_DATATRANS_NUM,
    GET_NETTOPO_INFO_NUM,
    REPORT_NETTOPOINFO_NUM,
    GET_CHIP_INFORMATION_NUM,
    REPORT_CHIP_INFORMATION_NUM,
    GET_MULTINET_INFORMATION_NUM,
    REPORT_MULTINET_INFORMATION_NUM,
    GET_NODELINE_INFORMATION_NUM,
    REPORT_NODELINE_INFORMATION_NUM,
    GET_NETWORK_SIZE_NUM,
    REPORT_NETWORK_SIZE_NUM,
    GET_HOST_MODEID_NUM,
    REPORT_HOST_MODEID_NUM,
    GET_SLAVE_MODEID_NUM,
    REPORT_SLAVE_MODEID_NUM,
    GET_MODE_INFO_NUM,
    REPORT_MODE_INFO_NUM,
    GET_NODE_VERSION_NUM,
    REPORT_NODE_VERSION_NUM,
    SET_HPLC_FREQ_NUM,
    GET_HPLC_FREQ_NUM,
    ACTION_IDENTIAREA_CFG_NUM,
    ACTION_NODEOFFLINE_CFG_NUM,
    REPORT_NODEOFFLINE_NUM,
    ACTION_REFUSE_NODEREPORT_CFG_NUM,
    GET_BROADCAST_DELAY_NUM,
    ACTION_BROADCAST_CMD_NUM,
    GET_METERING_STATE_NUM,
    ACTION_START_FILEUPGRADE_NUM,
    GET_FILEUPGRADE_STATE_NUM,
    ACTION_PAUSE_METERING_NUM,
    ACTION_RESUME_METERING_NUM,
    ACTION_RESTART_METERING_NUM,
    GET_MODULEINFO_NUM
} REQ_TYPE;

class CcoControl
{
public:
    CcoControl();
    ~CcoControl();

    int parseCcoData(const char *topic, const char *message);
    int packSendMqttMsg(void *info, int dataSize);
    void setMqttControl(MqttControl *mqttControl, MQTT_INFO *mInfo);
    void dataInit(const char *topic, const char *message);

private:
    int m_token;
    PROTOCOL_GW1376_2_DATA *m_pdata;
    std::map<int, RES_TOKEN_INFO> m_mqttResMap; // 用于mqtt返回 第一个是1376.2下发的token  第二个是返回的mqtt的token和restopic结构体
    zlog_category_t *m_logc;                    // zlog 指針
    std::map<std::string, int> m_ccoTopicInfo;

    MqttControl *m_mqttControl; // MQTT接口类指针
    MQTT_INFO *m_mInfo;         // MQTT 配置信息，配置文件中获取

    std::map<std::string, int> m_fileInfosMap;          // 用于并发采集和透传 string为从节点地址，int为协议类型
    std::map<std::string, std::string> m_concurrentRes; // 并发采集上报，key addr，value topic发完就删

    void paraInit();
    int parseGetMasterNode(std::string topic, std::string message); // 解析 查询主节点地址
    int sendGetMasterNode(cJSON *root, void *addr);                 // 响应 查询主节点地址

    int parseSetMasterNode(std::string topic, std::string message); // 解析 设置主节点地址
    int sendSetMasterNode(cJSON *root, void *info);                 // 响应 设置主节点地址

    int parseSetAcqAddFiles(std::string topic, std::string message); // 解析 添加档案

    int parseGetAcqQueryFiles(std::string topic, std::string message); // 解析 查询档案信息
    int sendGetAcqFilesInfo(cJSON *root, void *info);                  // 响应 查询档案信息

    int parseGetAcqFilesNum(std::string topic, std::string message); // 解析  获取档案数量
    int sendGetAcqFilesNum(cJSON *root, void *addr);                 // 响应  获取档案数量

    int parseDelAcqFiles(std::string topic, std::string message); // 解析 删除档案

    int parseClearAcqFiles(std::string topic, std::string message); // 解析 清空档案

    int parseActionDataTrans(std::string topic, std::string message); // 解析  透传

    int parseActionConCurrent(std::string topic, std::string message); // 解析 并发采集帧发送

    int sendCommonResponse(cJSON *root, void *info); // 通用响应
    std::string getReason(int result);
    int addResObject(cJSON *root, int token);
    int addPubilcObject(cJSON *root);
};

#endif