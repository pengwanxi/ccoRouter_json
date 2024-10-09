/**
 *   \file protocol_gw1376_2_data.h
 *   \brief 一些数据相关的函数
 */
#ifndef _PROTOCOL_GW1376_2_DATA_H_
#define _PROTOCOL_GW1376_2_DATA_H_
#include "gw13762_task.h"
#include "Dlist.h"

#include <pthread.h>

#define PROTOCOL_GW1376_2_BUF_LEN 4096
#define PROTOCOL_GW1376_2_APPLY_DATA_LEN 3072
#define PROTOCOL_GW1376_2_SENGDING_TIMEOUT 20 /* 最大发送时间，单位s */
#define PROTOCOL_GW1376_2_SUBNODE_TIMEOUT 10  /* 从节点超时时间，单位s */
#define ONCE_ADD_SUBNODE_NUM (50)

/* 方向 */
typedef enum _PROTOCOL_GW1376_2_DIR
{
    GW1376_2_DIR_DOWN = 0,
    GW1376_2_DIR_UP,
} PROTOCOL_GW1376_2_DIR;

/* 启动 */
typedef enum _PROTOCOL_GW1376_2_PRM
{
    GW1376_2_PRM_SLAVE = 0,
    GW1376_2_PRM_MASTER,
} PROTOCOL_GW1376_2_PRM;

/* 通讯方式 */
typedef enum _PROTOCOL_GW1376_2_COMM_TYPE
{
    GW1376_2_COMM_TYPE_RESERVE = 0,
    GW1376_2_COMM_TYPE_CENTRALZATION_ROUTE, /* 集中式 */
    GW1376_2_COMM_TYPE_DISTRIBUTED_ROUTE,   /* 分散式 */
    GW1376_2_COMM_TYPE_HPLC_CARRIER,        /* 宽带载波 */
    GW1376_2_COMM_TYPE_WIRELESS_COMM = 10,  /* 无线通讯 */
    GW1376_2_COMM_TYPE_TCP_IP_COMM = 20,    /* 以太通讯 */
} PROTOCOL_GW1376_2_COMM_TYPE;

/* 路由模式 */
typedef enum _PROTOCOL_GW1376_2_ROUTE
{
    GW1376_2_ROUTE_MODE = 0, /* 带路由模式 */
    GW1376_2_ROUTE_MODE_NO,  /* 不带路由模式 */
} PROTOCOL_GW1376_2_ROUTE;

/* 附加节点 */
typedef enum _PROTOCOL_GW1376_2_SUBNODE
{
    GW1376_2_SUBNODE_NO = 0, /* 无附加节点 */
    GW1376_2_SUBNODE_HAVE,   /* 有附加节点 */
} PROTOCOL_GW1376_2_SUBNODE;

/* 通讯模块 */
typedef enum _PROTOCOL_GW1376_2_COMM_MODULE
{
    GW1376_2_CCO = 0, /* 主节点进行操作 */
    GW1376_2_STA,     /* 从节点进行操作 */
} PROTOCOL_GW1376_2_COMM_MODULE;

/* 冲突 */
typedef enum _PROTOCOL_GW1376_2_CONFLICT
{
    GW1376_2_CONFLICT_NO = 0, /* 不冲突 */
    GW1376_2_CONFLICT,        /* 冲突 */
} PROTOCOL_GW1376_2_CONFLICT;

/* 中继级别 */
typedef enum _PROTOCOL_GW1376_2_RELAY_LEVEL
{
    GW1376_2_RELAY_LEVEL0 = 0, /* 中继级别0 */
    GW1376_2_RELAY_LEVEL1,
    GW1376_2_RELAY_LEVEL2,
    GW1376_2_RELAY_LEVEL3,
    GW1376_2_RELAY_LEVEL4,
    GW1376_2_RELAY_LEVEL5,
    GW1376_2_RELAY_LEVEL6,
    GW1376_2_RELAY_LEVEL7,
    GW1376_2_RELAY_LEVEL8,
    GW1376_2_RELAY_LEVEL9,
    GW1376_2_RELAY_LEVEL10,
    GW1376_2_RELAY_LEVEL11,
    GW1376_2_RELAY_LEVEL12,
    GW1376_2_RELAY_LEVEL13,
    GW1376_2_RELAY_LEVEL14,
    GW1376_2_RELAY_LEVEL15,
} PROTOCOL_GWRELAY_1376_2_RELAY_LEVEL;

/* 信道 */
typedef enum _PROTOCOL_GW1376_2_CHANNEL
{
    GW1376_2_CHANNEL0 = 0, /* 中继级别0 */
    GW1376_2_CHANNEL1,
    GW1376_2_CHANNEL2,
    GW1376_2_CHANNEL3,
    GW1376_2_CHANNEL4,
    GW1376_2_CHANNEL5,
    GW1376_2_CHANNEL6,
    GW1376_2_CHANNEL7,
    GW1376_2_CHANNEL8,
    GW1376_2_CHANNEL9,
    GW1376_2_CHANNEL10,
    GW1376_2_CHANNEL11,
    GW1376_2_CHANNEL12,
    GW1376_2_CHANNEL13,
    GW1376_2_CHANNEL14,
    GW1376_2_CHANNEL15,
} PROTOCOL_GW1376_2_CHANNEL;

/* 编码 */
typedef enum _PROTOCOL_GW1376_2_CODING
{
    GW1376_2_CODING_NO = 0,  /* 未编码 */
    GW1376_2_CODING_RS,      /* RS编码 */
    GW1376_2_CODING_RESEVER, /* 保留 */
} PROTOCOL_GW1376_2_CODING;

/* 延时 */
typedef enum _PROTOCOL_GW1376_2_BYTES
{
    GW1376_2_BYTES_DEFAULT = 0, /* 默认时间 */
} PROTOCOL_GW1376_2_BYTES;

/* 协议 */
typedef enum _PROTOCOL_GW1376_2_PROTOCOL
{
    GW1376_2_PROTOCOL_TRANSPARENT = 0, /* 透明传输 */
    GW1376_2_PROTOCOL_DLT645_97,
    GW1376_2_PROTOCOL_DLT645_07,
} PROTOCOL_GW1376_2_PROTOCOL;

/* 类型 */
typedef enum _PROTOCOL_GW1376_2_DATA_TYPE
{
    GW1376_2_DATA_TYPE_NULL = 0,                 /* 空 */
    GW1376_2_DATA_TYPE_HARD_INIT,                /* 硬件初始化 */
    GW1376_2_DATA_TYPE_PARAM_INIT,               /* 参数初始化 */
    GW1376_2_DATA_TYPE_DATA_INIT,                /* 数据初始化 */
    GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR,      /* 读取主节点地址 */
    GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR,     /* 设置主节点地址 */
    GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP,    /* 设置子节点主动上传 */
    GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT,    /* 设置子节点超时时间 */
    GW1376_2_DATA_TYPE_READ_MAIN_RUN_MODE,       /* 读取运行模式 */
    GW1376_2_DATA_TYPE_READ_MAIN_NODE,           /* 读取主节点 */
    GW1376_2_DATA_TYPE_WRITE_MAIN_NODE,          /* 设置主节点 */
    GW1376_2_DATA_TYPE_READ_NODE,                /* 读取节点 */
    GW1376_2_DATA_TYPE_WRITE_NODE,               /* 设置节点 */
    GW1376_2_DATA_TYPE_TRANS_DATA,               /* 转发报文 */
    GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA,     /* 转发广播报文 */
    GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM,   /* 查询子节点数量 */
    GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO,  /* 查询子节点信息 */
    GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE,    /* 增加所有节点 */
    GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE,        /* 增加子节点 */
    GW1376_2_DATA_TYPE_ROUTE_DEL_SUBNODE,        /* 删除子节点 */
    GW1376_2_DATA_TYPE_ROUTE_SET_SUBNODE_RELAY,  /* 设置子节点中继 */
    GW1376_2_DATA_TYPE_AUTOUP,                   /* 主动上送 */
    GW1376_2_DATA_TYPE_ROUTE_RUN_STATE,          /* 查询路由状态 */
    GW1376_2_DATA_TYPE_ROUTE_NET_TOPO_INFO,      /* 查看单个网络在线信息 */
    GW1376_2_DATA_TYPE_ROUTE_ALL_NET_TOPO_INFO,  /* 查看所有网络在线信息 */
    GW1376_2_DATA_TYPE_CONCURRENT_METER_READING, /* 并发抄表 */
    GW1376_2_DATA_TYPE_SIZE,

} PROTOCOL_GW1376_2_DATA_TYPE;

// 并发抄表响应信息
typedef struct
{
    char addr[6];                                  // 地址
    int proType;                                   // 协议类型
    int bufLen;                                    // 报文长度
    char buffer[PROTOCOL_GW1376_2_APPLY_DATA_LEN]; // 报文
} CONCURRENT_INFO;

// 从节点信息
typedef struct
{
    char addr[6];     // 地址
    int delayLevel;   // 中继级别
    int listenSignal; // 侦听品质
    int proType;      // 协议类型
    int phase;        // 相位
} FILE_INFO;

// 从节点信息总
typedef struct
{
    int totalNum; // 总从节点数量
    int validNum; // 响应的从节点数量
    FILE_INFO *fileInfos;
} ACQ_FILES_INFO;

// 上报从节点信息
typedef struct
{
    int staDevType;    // 从节点设备类型
    int proType;       // 通讯协议类型
    char buffer[2048]; // 报文内容
    int bufferLen;     // 报文长度
} AUTOUP_STA_EVENT_INFO;

// 链表中数据
typedef struct
{
    int index;           // gw1376.2返回的token
    int gw13762DataType; // 返回的类型
    void *info;          // 对应返回类型的结构体
    int infoSize;        // 长度
    int isReport;        // 是否为上报数据  并发采集上报，从节点上报，主动上报
} RES_INFO;

// 确认否认信息
typedef struct
{
    int AFN;
    int Fn;
    int result;
} AFN00_INFO;

// 主节点信息
typedef struct
{
    char addr[6];
} GET_MASTER_NODE_INFO;

/* 延时 */
typedef enum _PROTOCOL_GW1376_2_BAUD_FLAG
{
    GW1376_2_BAUD_BIT = 0, /* bit/s */
    GW1376_2_BAUD_KBIT,    /* kbit/s */
} PROTOCOL_GW1376_2_BAUD_FLAG;

/* 信息域 */
typedef struct _PROTOCOL_GW1376_2_INFO_REGION
{
    char data[6];
} PROTOCOL_GW1376_2_INFO_REGION;

/* 信息域 */
typedef struct _PROTOCOL_GW1376_2_ADDR_REGION
{
    int relay_num;
    char src[6];
    char relay[6][6];
    char dest[6];
} PROTOCOL_GW1376_2_ADDR_REGION;

/* 用户数据 */
typedef struct _PROTOCOL_GW1376_2_APPLY_DATA
{
    unsigned char AFN;
    unsigned char Fn;
    char unit_buf[PROTOCOL_GW1376_2_APPLY_DATA_LEN];
    int unit_len;
} PROTOCOL_GW1376_2_APPLY_REGION;

/* 发送控制数据 */
typedef struct _PROTOCOL_GW1376_2_SEND_DATA
{
    bool subnode_auto_up;
    bool subnode_auto_up_enable;

    bool dir;                  /* 方向 */
    bool prm;                  /* 启动 */
    unsigned char comm_type;   /* 通讯类型 */
    bool route_mode;           /* 路由模式 */
    bool subnode;              /* 附加节点 */
    bool comm_module_flag;     /* 通信模块标识 */
    bool conflict;             /* 冲突 */
    unsigned char relay_level; /* 中继级别 */
    unsigned char channel;     /* 信道 */
    unsigned char coding;      /* 编码 */
    unsigned char bytes_count; /* 字节延时 */
    unsigned short comm_baud;
    bool comm_baud_flag;
    unsigned char protocol_type; /* 协议 */

    unsigned char ctrl_region;
    PROTOCOL_GW1376_2_INFO_REGION info_region;
    PROTOCOL_GW1376_2_ADDR_REGION addr_region;
    PROTOCOL_GW1376_2_APPLY_REGION apply_region;

    unsigned char apply_data[PROTOCOL_GW1376_2_APPLY_DATA_LEN];
    int apply_data_len;

    GW13762_TASK_DATA *ptask_data;
} PROTOCOL_GW1376_2_SEND_DATA;

/* 发送控制数据 */
typedef struct _PROTOCOL_GW1376_2_RECV_CONTROL_DATA
{
    bool dir;                  /* 方向 */
    bool prm;                  /* 启动 */
    unsigned char comm_type;   /* 通讯类型 */
    bool route_mode;           /* 路由模式 */
    bool subnode;              /* 附加节点 */
    bool comm_module_flag;     /* 通信模块标识 */
    bool conflict;             /* 冲突 */
    unsigned char relay_level; /* 中继级别 */
    unsigned char channel;     /* 信道 */
    unsigned char coding;      /* 编码 */
    unsigned char bytes_count; /* 字节延时 */
    unsigned short comm_baud;
    bool comm_baud_flag;
    unsigned char protocol_type; /* 协议 */

    int frame_len;
    unsigned char ctrl_region;
    PROTOCOL_GW1376_2_INFO_REGION info_region;
    PROTOCOL_GW1376_2_ADDR_REGION addr_region;
    PROTOCOL_GW1376_2_APPLY_REGION apply_region;

    unsigned char apply_data[PROTOCOL_GW1376_2_APPLY_DATA_LEN];
    int value_len;

    int apply_data_len;
    int recvIndex;               // 存储返回的token
    ListHead_t *res_data_head;   // 用于接收后的数据返回
    pthread_mutex_t resHeadLock; // 增加互斥锁和条件变量
    pthread_cond_t resHeadCond;

    GW13762_TASK_DATA task_data;
} PROTOCOL_GW1376_2_RECV_DATA;

/* 1376.2 的数据 */
typedef struct _PROTOCOL_GW1376_2_DATA
{
    void *pprotocol_data;

    int type; /* 数据类型 */
    int state;

    int dlt645_init;
    int unpack_frame_error_count;

    unsigned char buf_count;

    char user_data_buf[PROTOCOL_GW1376_2_BUF_LEN];
    int user_data_len;

    char resend_buf[PROTOCOL_GW1376_2_BUF_LEN];
    int resend_len;
    int resend_type;
    int isresend;

    bool isbroadcast;
    bool broadcast_buf_count;
    bool issending;
    int sending_time;

    PROTOCOL_GW1376_2_SEND_DATA send;
    PROTOCOL_GW1376_2_RECV_DATA recv;
    GW13762_TASK task;

    /* 读上来的数据 */
    unsigned short subnode_start;
    unsigned char subnode_num;

} PROTOCOL_GW1376_2_DATA;

PROTOCOL_GW1376_2_DATA *get_pdata(void);

void protocol_gw1376_2_data_set_send_channel(PROTOCOL_GW1376_2_DATA *pdata,
                                             int channel);

/**
 *  \brief 初始化
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
int protocol_gw1376_2_data_init(void *p);

/**
 *  \brief 初始化
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
void protocol_gw1376_2_data_exit(void *p);

/**
 *  \brief 通过comm 获取数据指针
 *  \param void *pcomm
 *  \return PROTOCOL_GW1376_2_DATA *
 */

/**
 *  \brief 获取发送数据指针
 *  \param PROTOCOL_GW1376_2_DATA
 *  \return 指针
 */
PROTOCOL_GW1376_2_SEND_DATA *
protocol_gw1376_2_send_data_get(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 获取接收数据指针
 *  \param PROTOCOL_GW1376_2_DATA
 *  \return 指针
 */
PROTOCOL_GW1376_2_RECV_DATA *
protocol_gw1376_2_recv_data_get(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 获取发送用户数据指针
 *  \param PROTOCOL_GW1376_2_DATA
 *  \return 指针
 */
PROTOCOL_GW1376_2_APPLY_REGION *
protocol_gw1376_2_send_apply_region_get(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 获取接收数据指针
 *  \param PROTOCOL_GW1376_2_DATA
 *  \return 指针
 */
PROTOCOL_GW1376_2_APPLY_REGION *
protocol_gw1376_2_recv_apply_region_get(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 设置unit_buf 发送数据
 *  \param PROTOCOL_GW1376_2_DATA
 *  \return 指针
 */
int protocol_gw1376_2_set_send_apply_region_unit_buf(
    PROTOCOL_GW1376_2_DATA *pdata, unsigned char *buf, int len);

int protocol_gw1376_2_send_apply_region_unit_buf(PROTOCOL_GW1376_2_DATA *pdata,
                                                 unsigned char *buf);

/**
 *  \brief 获取接收unit_buf 数据
 *  \param PROTOCOL_GW1376_2_DATA
 *  \return 指针
 */
int protocol_gw1376_2_set_recv_apply_region_unit_buf(
    PROTOCOL_GW1376_2_DATA *pdata, unsigned char *buf, int len);

int protocol_gw1376_2_recv_apply_region_unit_buf(PROTOCOL_GW1376_2_DATA *pdata,
                                                 unsigned char *buf);

/**
 *  \brief 设置发送方向
 *  \param PROTOCOL_GW1376_2_DIR
 *  \return
 */
void protocol_gw1376_2_data_set_send_dir(PROTOCOL_GW1376_2_DATA *pdata,
                                         PROTOCOL_GW1376_2_DIR dir);

/**
 *  \brief 设置启动
 *  \param PROTOCOL_GW1376_2_PRM
 *  \return
 */
void protocol_gw1376_2_data_set_send_prm(PROTOCOL_GW1376_2_DATA *pdata,
                                         PROTOCOL_GW1376_2_PRM prm);

/**
 *  \brief 获取接收启动
 *  \param PROTOCOL_GW1376_2_PRM
 *  \return
 */
bool protocol_gw1376_2_data_set_recv_prm(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 设置通讯方式
 *  \param MPROTOCOL_GW1376_2_COMM_TYPE
 *  \return
 */
void protocol_gw1376_2_data_set_send_comm_type(
    PROTOCOL_GW1376_2_DATA *pdata, PROTOCOL_GW1376_2_COMM_TYPE comm_type);

/**
 *  \brief 路由模式
 *  \param param
 *  \return return type
 */
void protocol_gw1376_2_data_set_send_route_mode(PROTOCOL_GW1376_2_DATA *pdata,
                                                int route_mode);

/**
 *  \brief 设置通讯标识
 *  \param MPROTOCOL_GW1376_2_COMM_TYPE
 *  \return
 */
void protocol_gw1376_2_data_set_send_comm_module_flag(
    PROTOCOL_GW1376_2_DATA *pdata, int comm_module_flag);

/**
 *  \brief 设置中继
 *  \param MPROTOCOL_GW1376_2_COMM_TYPE
 *  \return
 */
void protocol_gw1376_2_data_set_send_relay_level(PROTOCOL_GW1376_2_DATA *pdata,
                                                 int relay_level);

/**
 *  \brief 设置发送序号
 *  \param MPROTOCOL_GW1376_2_COMM_TYPE
 *  \return
 */
void protocol_gw1376_2_data_set_send_buf_count(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 设置发送数据
 *  \return
 */
void protocol_gw1376_2_data_set_send_task_data(PROTOCOL_GW1376_2_DATA *pdata,
                                               GW13762_TASK_DATA *ptask_data);

/**
 *  \brief 设置协议类型
 *  \param type
 *  \return void
 */
void protocol_gw1376_2_data_set_data_type(PROTOCOL_GW1376_2_DATA *pdata,
                                          int type);

/**
 *  \brief 获取协议类型
 *  \param type
 *  \return void
 */
int protocol_gw1376_2_data_get_data_type(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 设置协议状态
 *  \param type
 *  \return void
 */
void protocol_gw1376_2_data_set_state(PROTOCOL_GW1376_2_DATA *pdata, int type);

/**
 *  \brief 获取协议状态
 *  \param type
 *  \return void
 */
int protocol_gw1376_2_data_get_state(PROTOCOL_GW1376_2_DATA *pdata);

RES_INFO *CreateResInfo();
CONCURRENT_INFO *CreateConcurrentInfo();

#endif /* _PROTOCOL_GW1376_2_DATA_H_ */
