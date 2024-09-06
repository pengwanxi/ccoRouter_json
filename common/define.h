#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <string>
#include <list>
#include <vector>
#include <map>

#define MAIN_DEBUG_EN 1

typedef bool BOOLEAN;

typedef unsigned char uint8;
typedef unsigned char uint8_t;

typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef float fp32;
typedef double fp64;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;

// mqtt  define info

// end


typedef struct
{
    int proToken;           // 下发1376.2token
    int mqttToken;          // mqtt返回token
    std::string resTopic;   // 响应的topic
    std::string judgeTopic; // 返回时用来做判断的topic
} RES_TOKEN_INFO;

typedef struct
{
    uint8 MqttInitial;
    uint8 DatabaseInitial;
} TEM_STATUS;

typedef std::map<int, std::string> MSGMAP;

typedef struct
{
    int nport;
    int nkeepalive;
    int nqos;
    char clientid[64];
    char host[64];
    char user[64];
    char password[64];
} MQTT_INFO;

typedef struct
{
    std::string host;
    int nport;
    std::string token;
} FRPS_INFO;

typedef struct
{
    std::string name;
    std::string type;
    std::string local_ip;
    int local_port;
    int remote_port;
} FRPC_INFO;

typedef struct
{
    std::string conf_path;
    int keep_time; // frpc结束时间  单位：min
    FRPS_INFO frpsInfo;
    std::vector<FRPC_INFO> frpcInfo;
} FRP_INFO;

typedef struct
{
    short id;
    char name[20];
    int baud;
    int bit;
    int stopBit;
    char parity[10];
    int type;
} SERIAL_PORT_INFO;

typedef std::list<std::string> LISTSTR;

typedef struct
{
    char name[20];
    int isreport;
    char type[10];
    char des[20];
    char unit[10];
    char deadzone[10];
    char ratio[10];
    char userdefine[10];
    LISTSTR row;

} COLLECT_POINT;

typedef enum
{
    serial_Port = 0,
    spi_Port,
    other_Port,
} SERIAL_TYPE;

typedef enum
{
    localserver,
    masterserver,
} MQTT_SERVERTYPE;

typedef struct
{
    std::string version;
    std::string compatible;
} VERINFO;

typedef struct
{
    std::string name;      // 名称  符合模型命名规则
    std::string val;       // 值
    std::string quality;   // 信号质量
    std::string timestamp; // 时间戳 格式 2019-03-01T09:30:09.230+0800
    std::string secret;    // 是否加密，1 或 0。1：加密，0：不加密
    int report;
    float deadzone;
} REALTIME_BASE;

typedef struct
{
    std::string name;   // 名称  符合模型命名规则
    std::string val;    // 值
    std::string secret; // 是否加密，1 或 0。1：加密，0：不加密
} FROZEN_DATA;

typedef struct
{
    std::string timestamp;         // 数据时标 格式 2019-03-01T09:30:09.230+0800
    std::string time_start_gather; // 采集开始 格式 2019-03-01T09:30:09.230+0800
    std::string time_end_gather;   // 采集结束 格式 2019-03-01T09:30:09.230+0800
    std::string additional_check;  // 附加校验
    std::list<FROZEN_DATA> data;
} FROZEN_BASE;
typedef enum
{
    mon = 0,     // 月冻结
    billday,     // 结算日
    day_frozen,  // 日冻结
    hour_frozen, // 小时冻结
    min_frozen,  // 分钟冻结
    realtime,    // 采集实时分钟冻结
    schfroz      // 实时数据转冻结
} FROZEN_TYPE;

typedef struct
{
    std::string name;       // 名称  符合模型命名规则
    std::string type;       // 数据类型
    std::string unit;       // 单位
    std::string deadzone;   // 死区
    std::string ratio;      // 变比
    std::string isreport;   // 是否上报
    std::string userdefine; // 自定义描述
    // 字段特殊说明
    // 属性描述,模型名称可同时具备多个属性，选择范围： single,frozen,single_p,frozen_p。single表示共享实时数据，frozen表示共享冻结数据，single_p表示私有实时数据，frozen_p表示私有冻结数据。
    std::vector<std::string> datarow; // 属性描述 新版本数据中心接口需要参数
} MODEL_BASE;

typedef struct
{
    std::string model;    // 模型名称
    std::string port;     // 设备端口
    std::string addr;     // 终端地址
    std::string desc;     // 自定义描述
    std::string manuID;   // 厂商ID
    std::string isreport; // 上报标志
} MODEL_INFO;

typedef struct
{
    int devid;
    std::string model; // 模型名词
    std::string guid;  // 设备在数据中心中的唯一编码
    std::string dev;   // 由模型和guid组合而成，是数据中心中设备的唯一地址标识
    std::string port;
    std::string desc;
    std::string addr;

} GUID_INFO;

typedef struct
{
    std::string guid;
    std::string dev;
    std::string addr;
    std::string desc;
    std::string manuID;
    std::string isReport;
    std::string nodeID;
    std::string productID;
} DEV_INFO;

typedef struct
{
    std::string model;
    std::string port;
    std::vector<DEV_INFO> dev;
} REGISTER_DEV;

typedef struct
{
    std::string model;
    std::string port;
    std::string addr;
    std::string desc;
    std::string manuID;
    std::string manuname;    // 厂商名称 新增 新版本数据中心使用
    std::string protype;     // 协议类型  新增 新版本数据中心使用
    std::string device_type; // 设备型号 新增 新版本数据中心使用
    std::string isReport;
    std::string nodeID;
    std::string productID;
} DEV_REGISTER_INFO;

typedef struct
{
    std::string name;      // 名称  符合模型命名规则
    std::string id;        // 序号
    std::string val;       // 值
    std::string unit;      // 单位
    std::string quality;   // 信号质量
    std::string timestamp; // 时间戳 格式 2019-03-01T09:30:09.230+0800
} CHANGE_UPDATE;

typedef struct
{
    std::string devno;    // 设备编号
    std::string port;     // 若为真实的物理设备端口，在RS485-1、RS485-2、RS485-3、RS485-4、PLC、UMW中足选择，分别表示485口1-4通道、电力载波、微功率无线；模拟端口自定义，不可为空
    std::string addr;     // 设备地址
    std::string desc;     // 自定义描述，用于 model、port、addr 无法确定唯一设备时使用
    std::string protocol; // 设备通信协议
    std::string model;
} DEV_PARM;

typedef struct
{
    std::string model; // 模型名称
    std::vector<DEV_PARM> dev;
} DEVLIST;

typedef struct
{
    std::string devlist; // 模型名称
    std::vector<DEV_PARM> dev;
} REQUEST_DEVLIST;

typedef struct
{
    std::string devlist; // 模型名称
    std::string devno;
    std::string dev;
} RESPONSE_DEVLIST;

// 数据上报变化接口 所使用的结构，不同的数据上报所使用的参数不同。每次只需要使用
// 和本次上报有关的字段，其他字段无需处理。
typedef struct
{
    int type;
    std::string model;               // 模型名称
    std::string dev;                 // 数据中心内唯一ID
    std::string event;               // 事件名称
    std::string report_time;         // 上报时间
    std::string start_time;          // 开始时间
    std::string endtime;             // 结束时间
    std::vector<CHANGE_UPDATE> data; // 当 datatype=5 上报日月越限， 字段填写的是越上限数据
    std::vector<CHANGE_UPDATE> lowdata;
} EXT_UPDATE;

typedef struct
{
    std::string dev;               // 由模型和 guid 组合而成，是数据中心中设备的唯一地址
    std::string totalcall;         // 表示全部或部分定值参数，0-表示部分，1-表示全部
    std::vector<std::string> name; // totalcall 为 1 时，为空；totalcall 为 0 时，填写数据名词
} REQUEST_REALTIME;

typedef struct
{
    std::string dev;
    std::vector<REALTIME_BASE> data;
} RESPONSE_REALTIME;

typedef struct
{
    std::string appname;                // 微应用名称
    std::string sourtype;               // 协议类型
    std::string model;                  // 模型名称
    std::string dev;                    // 数据中心中设备的唯一地址
    std::string event;                  // 事件名称
    std::string timestamp;              // 数据时标
    std::string timestartgather;        // 采集开始时间
    std::string timeendgather;          // 采集结束时间
    std::string starttimestamp;         // 事件发生时间
    std::string endtimestamp;           // 事件结束时间
    std::string happensrc;              // 事件发生源
    std::string isneedrpt;              // 事件是否需要主动上报
    std::string occurnum;               // 事件序号
    std::string eventlevel;             // 事件类型，普通事件-common重要事件-important
    std::vector<std::string> rptstatus; // 事件上报状态
    std::string data;                   // Base64编码后的数据，101/104协议时该字段为空
    std::vector<REALTIME_BASE> extdata; // 使用104协议时事件的关联属性
} SOEBASE;

typedef struct
{
    std::string dev;      // 数据中心中设备的唯一地址
    std::string rptevt;   // 事件名称
    std::string maxrecsn; // 该事件的最大序号

} NOTIFY_SOE;

typedef struct
{
    int type; // 0:极大值 1:极小值
    std::vector<CHANGE_UPDATE> data;
} EXTREME;

typedef struct
{
    int type;               // 0:日合格率，1:月合格率
    std::string reporttime; // 上报时间
    std::vector<CHANGE_UPDATE> data;
} QUALIFICATION;

typedef struct
{
    int type;               // 0:日越限，1:月越限。
    std::string reporttime; // 上报时间
    std::vector<CHANGE_UPDATE> upper;
    std::vector<CHANGE_UPDATE> lower;
} LIMIT;

typedef struct
{
    int type; // 0:日重载 1:日过载
    std::string starttime;
    std::string endtime;
    std::vector<CHANGE_UPDATE> data;
} OVERLOAD;

typedef struct
{
    std::string model;
    std::string dev;
    std::string event;
    std::string starttime;
    std::string endtime;
    std::vector<CHANGE_UPDATE> data;
} FAULT;

typedef struct
{
    int datatype; // 消息类型：0-遥测；1-遥信；2-15 分钟冻结数据；3-极值；4-日月合格率；5-日月越限；6-日重载、日过载；7-故障报告
    std::string model;
    std::string dev;
    std::vector<CHANGE_UPDATE> ycyxvalue; // 遥信遥测
    std::vector<FROZEN_BASE> forzenvalue; // 冻结数据
    std::vector<EXTREME> extreme;         // 极值
    std::vector<QUALIFICATION> qual;      // 合格率
    std::vector<LIMIT> limit;             // 限值
    std::vector<OVERLOAD> overload;       // 过载和重载
    std::vector<FAULT> falut;             // 故障
} NOTIFY_REPORT;

// SOE 事件查询 请求结构
typedef struct
{
    std::string model;              // 模型名称     非必选 按设备查询填写其他查询无需填写
    std::string totaldev;           // 1-所有设备，0-指定设备 非必选 按设备查询填写其他查询无需填写
    std::vector<std::string> dev;   // 设备唯一标识 非必选 按设备查询填写其他查询无需填写
    std::string totalevent;         // 1-所有事件，0-指定事件 非必选 按事件查询填写其他查询无需填写
    std::vector<std::string> event; // 事件名称集合 非必选 按事件查询填写其他查询无需填写

} CONDITION;
typedef struct
{
    std::string timetype;             // 时标类型 非必选 按事件查询，按设备查询 填写
    std::string starttime;            // 事件开始时间 非必选 按事件查询，按设备查询 填写
    std::string endtime;              // 事件结束时间 非必选 按事件查询，按设备查询 填写
    std::string srctype;              // 协议类型  必选 所有查询都需要
    std::vector<CONDITION> cond;      // 查询条件 非必选 按事件查询，按设备查询 填写
    std::string model;                // 模型名称  非必选 按序号查询 上N条记录 批量查询填写
    std::string dev;                  // 设备唯一标识 非必选 按序号查询 上N条记录 批量查询填写
    std::string occurnum;             // 事件序号 非必选 按序号查询 上N条记录 批量查询填写
    std::string startsn;              // 开始序号 非必选  批量查询填写
    std::string num;                  // 记录数 非必选 上N条记录 批量查询填写
    std::vector<std::string> devcond; // 事件名称集合
} REQUEST_SOE;

typedef struct
{
    std::string model;            // 模型名称
    std::string dev;              // 设备的唯一标识
    std::string event;            // 事件名称
    std::string happensrc;        // 事件源
    std::string happencnt;        // 事件发生次数
    std::string totaltime;        // 事件累计时间
    std::string last_happen_time; // 最近一次发生时间
    std::string last_resume_time; // 最近一次结束时间
} SOE_STATUS;

typedef struct
{
    std::string startsn;   // 开始序号
    std::string readcnt;   // 查询数量
    std::string model;     // 模型名称
    std::string dev;       // 设备的唯一标识
    std::string event;     // 事件名称
    std::string happensrc; // 事件源
} REQUEST_SOESTATUS;

typedef struct
{
    std::string model;            // 模型名称
    std::string dev;              // 设备的唯一标识
    std::string event;            // 事件名称
    std::string reccnt;           // 当前记录数
    std::string maxreccnt;        // 最大记录序号
    std::string happensrc;        // 事件源
    std::string happencnt;        // 事件发生次数
    std::string totaltime;        // 事件累计时间
    std::string last_happen_time; // 最近一次发生时间
    std::string last_resume_time; // 最近一次结束时间

} RESPONSE_SOESTATUS;

typedef struct
{
    std::string timetype;          // 时标类型
    std::string starttime;         // 开始时间
    std::string endtime;           // 结束时间
    std::string timespan;          // 周期
    std::string frozentype;        // 冻结数据类型 mon：表示月冻结；day：表示日冻结；billday：结算日；hour：表示小时冻结；min：表示分钟冻结；realtime：采集实时的分钟冻结
    std::string orderno;           // 记录号
    std::string uppern;            // 上N条
    std::string dev;               // 数据中心设备唯一地址
    std::vector<std::string> name; // 数据名称
} REQUEST_HISTORY;

typedef struct
{
    std::string dev;               // 数据中心设备唯一地址
    std::vector<FROZEN_BASE> data; // 冻结数据
} RESPONSE_HISTORY;

typedef struct
{
    std::string dev;               // 数据中心设备唯一地址
    std::string totalcall;         // 总召唤
    std::string datarow;           // 新版本数据中心使用
    std::vector<std::string> name; // 数据名称

} CHILD_MODEL;

typedef struct
{
    std::string modelname; // 子集名称
    std::vector<CHILD_MODEL> data;
} REQUEST_CHILDMODEL;

typedef struct
{
    std::string ctrlapp; // 遥控指令下发的APP名字
    std::string dev;     // 设备标识
    std::string name;    // 遥控变量名称
    std::string type;    // 遥控类型，SCO单点遥信，DCO双点遥信
    std::string cmd;     // 0：分闸，1：合闸，其它无效
    std::string action;  // 0：撤销，1：执行，2：预置，其它无效
    std::string mode;    // 0，被控站内部确定，1短脉冲方式，2长脉冲方式，3持续脉冲方式 其他无效  非必选 下发遥控命名有效
    std::string timeout; // 超时时间，单位秒 非必选 下发遥控命名有效
    std::string status;  // 返回状态“OK”/“FAILURE”/“TIMEOUT”  非必选 遥控命名回覆有效
} ACTION_CTRL;

typedef struct
{
    std::string dev;
    std::string model;
    std::string port;
    std::string addr;
    std::string desc;
} VTBODY;

typedef struct
{
    std::string appname; // APP名称
    std::vector<VTBODY> data;
} VT_APP;

typedef struct
{
    std::string name;     // 定值名称
    std::string val;      // 值
    std::string datatype; // 参数类型
    std::string unit;     // 新版本数据中心使用
} FIXED;

typedef struct
{
    std::string dev; // 设备标识
    std::vector<FIXED> param;
} REQUEST_FIXED;

typedef struct
{
    std::string dev;               // 设备标识
    std::string totalcall;         // 0表示部分，1表示全部
    std::vector<std::string> name; // 定值名称
} REQUEST_GETFIXED;

typedef REQUEST_FIXED RESPONSE_FIXED;

typedef REQUEST_GETFIXED REQUEST_DELFIXED;

typedef struct
{
    std::string paratype; // 参数名称
    std::string data;     // 参数内容
} REQUEST_PARAMETER;

typedef struct
{
    std::string sourtype; // 协议类型，
    std::string paratype; // 参数名称,单独查询有效
    std::string startsn;  // 参数开始序号
    std::string readcnt;  // 查询数量

} REQUEST_GETPARAMETER;

typedef struct
{
    std::string name;     // 定值名称
    std::string val;      // 定值参数 新版本数据中心有效
    std::string unit;     // 定值单位 新版本数据中心有效
    std::string datatype; // 参数类型 新版本数据中心有效

} NOTIFY_PARAMITEM;

typedef struct
{
    std::string dev; // guid
    std::vector<NOTIFY_PARAMITEM> items;

} NOTIFY_PARAMETEER;

typedef REQUEST_PARAMETER RESPONSE_PARAMETER;

typedef struct
{
    std::string cfgsn;         // 配置讯号
    std::string acqaddr;       // 采集多功能表地址，base64编码后数据
    std::string port;          // 物理设备端口
    std::string commbaud;      // 通讯波特率
    std::string commpsw;       // 通讯密码
    std::string protype;       // 协议类型
    std::string devtype;       // 用户类型
    std::string wiretype;      // 接线方式
    std::string ratedvolt;     // 额定电压
    std::string ratedcurr;     // 额定电流
    std::string collectoraddr; // 采集器地址
    std::string assetnum;      // 资产号
    std::string ratecnt;       // 费率 作为隐性字段可有可无
    std::string pt;            // 电压互感器
    std::string ct;            // 电流互感器
    std::string annexinfo;     // 附加信息
} ACQFILE;

typedef struct
{
    std::string start_cfg_sn; // 起始序号
    std::string end_cfg_sn;   // 结束序号
    std::string port;         // 设备端口  不可为空
    std::string acqaddr;      // 采集多功能表地址
} REQUEST_ACQFILE;

typedef struct
{
    std::string regiontype; // 运行时段类型
    std::string starttime;  // 起始时段
    std::string endtime;    // 结束时段

} TASKTIME;

typedef struct
{
    std::string taskid;              // 采集任务号
    std::string invunit;             // 执行时间单位
    std::string invvalue;            // 执行时间
    std::string delayunit;           // 延时时间单位
    std::string delayvalue;          // 延时时间
    std::string prio;                // 执行优先级
    std::string acqschtype;          // 方案类型
    std::string acqschno;            // 方案编号
    std::string starttime;           // 开始时间
    std::string endtime;             // 结束时间
    std::string state;               // 状态
    std::string pre_runscript_id;    // 任务开始前脚本id
    std::string post_runscript_id;   // 任务完成后脚本id
    std::vector<TASKTIME> enablerun; // 时段数组
} ACQTASK;

typedef struct
{
    std::string starttaskid; // 起始任务号
    std::string endtaskid;   // 结束任务号
} REQUEST_ACQTASK;

typedef struct
{
    std::string schno;                // 方案编号
    std::string storedepth;           // 存储深度
    std::string acqmode;              // 采集类型
    std::string acqcontent;           // 采集内容
    std::vector<std::string> devtype; // 设备类型
    std::string acqidtype;            // 采集ID类型,Real表示实时数据
    std::vector<std::string> acqid;   // 采集ID
    std::string storetimeflag;        // 存储时标
} COMACQSCHS;

typedef struct
{
    std::string schno;                   // 方案编号
    std::string acqmode;                 // 采集类型
    std::vector<std::string> acqcontent; // 采集内容
    std::vector<std::string> devtype;    // 设备类型
    std::string isneedrpt;               // 是否上报标志
    std::string storedepth;              // 存储深度
} EVTACQSCHS;

typedef struct
{
    std::string packetsn; // 报文序号
    std::string content;  // 报文内容
} PACKET;

typedef struct
{
    std::string sn;                 // 序号
    std::string comaddr;            // 采集地址
    std::vector<PACKET> packetdata; // 包数据
} ACQCONTENT;

typedef struct
{
    std::string schno;               // 方案编号
    std::vector<ACQCONTENT> acqcont; // 方案内容集
    std::string storedepth;          // 存储深度
} TRANSACQSCHS;

typedef struct
{
    std::string addr; // 地址
    std::string key1; // 关键字
    std::string key2; // 关键字
    std::string key3; // 关键字
    std::string body; //
} LIST_DATA;

typedef struct
{
    std::string listname; // 链表名称
    std::string key1;     // 关键字
} REQUEST_LISTDATA;

// localccocontrol   struct

typedef struct
{
    std::string acqaddr; // 设备地址。长度6个字节，若超过则取前六个字节，若不足则用0补齐，该字段不能为空
    std::string macaddr; // 采集器地址。长度6个字节，若超过则取前六个字节，若不足或没有则用0补齐
    std::string protype; // 协议类型：00-透明传输；01-DL/T645-1997；02-DL/T645-2007；03-DL/T698.45

} ADDFILE;

typedef struct
{
    std::string starttime; // 搜表开始时间（YYYY-MM-DD hh:mm:ss）
    std::string schunit;   // 搜表时间单位：second-秒；min-分钟；hour-小时
    std::string schvalue;  // 搜表时间 ?????
    std::string retry;     // 从节点重发次数
    std::string slice;     // 随机等待时间片个数
} SEARCH_METER;

typedef struct
{
    std::string acqaddr;      // 从节点地址
    std::string protype;      // 协议类型：00-透明传输；01-DL/T645-1997；02-DL/T645-2007；03-DL/T698.45
    std::string frametimeout; // 帧超时时间，单位为秒s
    std::string chartimeout;  // 字节超时时间，单位为ms
    std::string data;         // 经过Base64编码后获得的字符串
} MONITORNODE;

typedef struct
{
    std::string frametimeout; // 帧超时时间，单位为秒s
    std::string chartimeout;  // 字节超时时间，单位为ms
    std::string data;         // 经过Base64编码后获得的字符串
} TRANSDATA;

typedef struct
{
    std::string nodeaddr; // 节点地址
    std::string tei;      // 节点标识
    std::string proxytei;
    std::string nodelevel;
    std::string noderole;
} TOPOINFO;

typedef struct
{
    int nodesn;              // 节点序号
    std::string nodeaddr;    // 节点地址
    int16_t devtype;         // 节点设备类型
    std::string chipid;      // 芯片ID信息 24个字节，不足补0
    std::string chipsoftver; // 芯片软件版本

} CHIPINFO;

typedef struct
{
    int nodesn;           // 节点序号
    std::string nodeaddr; // 节点地址
    int node_lineinfo;
    // 相位信息 1字节。D0-D2:置"1"依次表示第 1、2、3 相，置"1"电表接入对应相位，置"0"表示未接入该相位或该相位断相；三相均为0，表示不支持或者正在识别中；
    // D3:电表类型：0 表示单相表、1 表示三相表；（主节点此项填 0）。D4:线路异常：置 0 表示从节点线路无异常或不支持识别功能，置 1 表示从节点线路有异常。当电能表为单相供电时，
    // 线路异常表示零火线反接；当电能表为三相供电时，线路异常表示逆相序、断相、零火反接；D7～D5：三相位相序类型:0 0 0-ABC（正常相序）；0 0 1-ACB；0 1 0-BAC；0 1 1-BCA；1 0 0-CAB；1 0 1-CBA；1 1 0-零火反接；1 1 1-保留。

} NODELINEINFO;

typedef struct
{
    std::string nodeaddr;   // 节点地址
    int devtype;            // 节点设备类型：0-代表电表模块；1-代表采集器模块；15-代表未知；其他保留
    int upgflag;            // 更新标识：0-代表当前已更新；1-代表当前未更新；在未更新的情况下回复的ID信息为存储的信息
    std::string vendorcode; // 厂商代码
    int modeidlen;          // 模块ID长度
    int modeidformat;       // 模块ID格式：00H-为组合格式；01H-为 BCD；02H-为 BIN ；03H-为 ASCII
    std::string modeidinfo; // 模块ID号
} SLAVEMODE;

typedef struct
{
    std::string factory;           // 厂商代码
    std::string module_vendorcode; // 芯片代码
    std::string softdate;          // 版本日期：BCD编码，YYMMDD日期格式
    std::string softver;           // 软件版本
} MODEINFO;

typedef struct
{
    std::string nodeaddr; // 节点地址
    std::string softver;  // 软件版本
    std::string softdate; // 版本日期：BCD编码，YYMMDD日期格式
    std::string factory;  // 厂商代码
    std::string chipcode; // 芯片代码
} NODE_VERSION;

typedef struct
{
    std::string last_received; // 接收时标，表示主模块最近一次收到抄表数据的时间（YYYY-MM-DD hh:mm:ss）
    int ctrl_stat;             // 主模块抄表状态：0-未抄表；1-抄表中
    int search_stat;           // 主模块搜表状态：0-未搜表;1-搜表中
    int auto_reading;          // 是否允许自动抄表：0-不允许;1-允许
    int init_data_count;       // 数据初始化次数
    int init_param_count;      // 参数初始化次数
    int reset_module_count;    // 复位模块次数
    int reset_router_count;    // 清除路由次数
} METERING_STATE;

typedef struct
{
    int status;            // 文件升级状态，0–未升级 ；1–升级中；2-升级结束
    std::string starttime; // 时标，升级开始时间(YYYY-MM-DD hh:mm:ss)
    std::string endtime;   // 时标，升级结束时间(YYYY-MM-DD hh:mm:ss)
    int result;            // 升级结果：0—升级成功；1—升级文件异常；2—升级传输失败；255—其他异常。升级未结束可不带该字段
} FILE_UPGRADE;

typedef struct
{
    std::string communication_mode; // 本地通信模式字
    int slave_monitor_overtime;     // 从节点监控最大超时(单位：S)
    int broadcast_maxover_time;     // 广播最大超时时间
    int package_maxlen;             // 最大支持报文长度
    int upgrade_maxpack_len;        // 升级最大单个包长度
    int upgrade_action_waittime;    // 升级操作等待时间(单位：S)
    int broadcast_delay_success;    // 广播成功延时时间(单位：S)
    std::string moduleaddr;         // 模块地址
    int support_maxslave_num;       // 支持的最大从节点数量
    int support_slave_num;          // 当前从节点数量
    std::string module_ver_info;    // 模块版本:厂商代码、芯片代码-年月日-版本
} MODULE_INFO;

// mqttiot  相关结构
typedef struct
{
    std::string guid;                       // 数据中心生成
    std::string dev;                        // 数据中心组合
    std::string model;                      // 本设备的 模型名称
    std::string nodeId;                     // nodeid生成规则未知
    std::string deviceId;                   // 云端生成的设备ID
    std::string name;                       // 设备名称
    std::string desc;                       // 描述
    std::string manuId;                     // 厂商ID
    std::vector<std::string> modelnamelist; // 需要上传的模型属性列表

} UPDATA_DEVINFO;

typedef struct
{
    std::string deviceId; // 唯一标识
    std::string status;   // 状态
} UPDATA_DEVSTATUS;

typedef struct
{
    std::string name;
    std::string value;
} UPDATA_DATA_NODE;

typedef struct
{
    std::string serviceId; // 服务ID  analog
    std::vector<UPDATA_DATA_NODE> data;
    std::string eventTime;
} UPDATA_DATA_BASE;

typedef struct
{
    std::string deviceId;                   // 设备ID
    std::vector<UPDATA_DATA_BASE> services; // 服务数据
} UPDATA_DEVDATA;

typedef struct
{
    int mid;
    std::string deviceId;
    std::string msgType;
    std::string serviceId;
    std::string cmd;
    std::vector<UPDATA_DATA_NODE> paras;
} UPDATA_CMD;

typedef struct
{
    std::string deviceId;               // 设备ID
    std::string serviceId;              // 服务ID  analog
    std::vector<UPDATA_DATA_NODE> data; // 服务数据
    std::string eventTime;
} UPDATA_DATA;

#endif
