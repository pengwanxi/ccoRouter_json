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
    GET_MASTERNODE_ENUM = 0,
    SET_MASTERNODE_ENUM,
    SET_ACQ_ADD_FILES_ENUM,
    GET_ACQ_QUERY_FILES_ENUM,
    ACTION_DEL_ACQFILES_ENUM,
    ACTION_CLEAR_ACQFILES_ENUM,
    GET_ACQFILES_NUM_ENUM,
    REPORT_METERERROR_ENUM,
    REPORT_EVENT_ENUM,
    ACTION_EN_SEARCHMETER_ENUM,
    ACTION_DIS_SEARCHMETER_ENUM,
    REPORT_SEARCHMETER_ENUM,
    GET_MONITOR_NODEDELAY_ENUM,
    ACTION_MONITOR_NODE_ENUM,
    ACTION_CONCURRENT_ENUM,
    REPORT_REPORT_CONCURRENT_ENUM,
    ACTION_DATATRANS_ENUM,
    GET_NETTOPO_INFO_ENUM,
    REPORT_NETTOPOINFO_ENUM,
    GET_CHIP_INFORMATION_ENUM,
    REPORT_CHIP_INFORMATION_ENUM,
    GET_MULTINET_INFORMATION_ENUM,
    REPORT_MULTINET_INFORMATION_ENUM,
    GET_NODELINE_INFORMATION_ENUM,
    REPORT_NODELINE_INFORMATION_ENUM,
    GET_NETWORK_SIZE_ENUM,
    REPORT_NETWORK_SIZE_ENUM,
    GET_HOST_MODEID_ENUM,
    REPORT_HOST_MODEID_ENUM,
    GET_SLAVE_MODEID_ENUM,
    REPORT_SLAVE_MODEID_ENUM,
    GET_MODE_INFO_ENUM,
    REPORT_MODE_INFO_ENUM,
    GET_NODE_VERSION_ENUM,
    REPORT_NODE_VERSION_ENUM,
    SET_HPLC_FREQ_ENUM,
    GET_HPLC_FREQ_ENUM,
    ACTION_IDENTIAREA_CFG_ENUM,
    ACTION_NODEOFFLINE_CFG_ENUM,
    REPORT_NODEOFFLINE_ENUM,
    ACTION_REFUSE_NODEREPORT_CFG_ENUM,
    GET_BROADCAST_DELAY_ENUM,
    ACTION_BROADCAST_CMD_ENUM,
    GET_METERING_STATE_ENUM,
    ACTION_START_FILEUPGRADE_ENUM,
    GET_FILEUPGRADE_STATE_ENUM,
    ACTION_PAUSE_METERING_ENUM,
    ACTION_RESUME_METERING_ENUM,
    ACTION_RESTART_METERING_ENUM,
    GET_MODULEINFO_ENUM
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
    static int get13762TypeForToken(int);
    static std::map<int, RES_TOKEN_INFO> *m_mqttResMap_tmp;

private:
    int m_token;
    PROTOCOL_GW1376_2_DATA *m_pdata;

    std::map<int, RES_TOKEN_INFO> m_mqttResMap; // 用于mqtt返回 第一个是1376.2下发的token  第二个是返回的mqtt的token和restopic结构体
    zlog_category_t *m_logc;                    // zlog 指針
    std::map<std::string, int> m_ccoTopicInfo;
    std::map<int, std::string> m_noConcurrentMap;          // 用于并发采集回并非并发采集消息时  int 对应发送1376.2token string 对应地址

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

    int parseActionRefuseNodeReport(std::string topic, std::string message); // 解析 禁止子节点上报

    int parseActionBroadcastCmd(std::string topic, std::string message); // 解析 广播命令

    int sendCommonResponse(cJSON *root, void *info); // 通用响应
    std::string getReason(int result);
    int addResObject(cJSON *root, int token);
    int addPubilcObject(cJSON *root);
};

#endif