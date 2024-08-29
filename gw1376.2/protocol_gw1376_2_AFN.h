/**
 *   \file protocol_gw1376_2_AFN.h
 *   \brief 组织AFN数据
 */
#ifndef _PROTOCOL_GW1376_2_AFN_H_
#define _PROTOCOL_GW1376_2_AFN_H_

#include "protocol_gw1376_2_data.h"

/**
 *  \brief 计算fn
 *  \param DT1
 *  \param DT2
 *  \return unsigned char
 */
unsigned char protocol_gw1376_2_cal_fn(unsigned char DT1, unsigned char DT2);

/**
 *  \brief 分解fn
 *  \param DT1
 *  \param DT2
 *  \return 0 成功
 *  \return <0 失败
 */
int protocol_gw1376_2_dec_fn(unsigned char fn, unsigned char *DT1,
                             unsigned char *DT2);

/*****************************************************************************/
/*                                   AFN00 数据确认                          */
/*****************************************************************************/
int protocol_gw1376_AFN00_up(PROTOCOL_GW1376_2_DATA *pdata);

/*****************************************************************************/
/*                                   AFN01 数据初始化                        */
/*****************************************************************************/
/**
 *  \brief  AFN01 F1 参数初始化（复位）
 *  \param param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN01_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief  AFN01 F1 硬件初始化（恢复出厂设置）
 *  \param param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN01_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief  AFN01 F1 数据区初始化（节点侦听信息）
 *  \param param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN01_Fn03_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AF01 上送
 *
 *  跳转AFN00
 *
 *  \param
 *  \return
 */
int protocol_gw1376_AFN01_up(PROTOCOL_GW1376_2_DATA *pdata);
/*****************************************************************************/
/*                                   AFN02 数据转发                          */
/*****************************************************************************/
/**
 *  \brief AFN02 F1 数据下发
 *
 *  数据转发
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN02_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN02 F1 数据处理
 *
 *  数据转发
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN02_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata);

/*****************************************************************************/
/*                                   AFN03 查询数据                          */
/*****************************************************************************/
/**
 *  \brief AFN02 F1 数据下发
 *
 *  读取主节点地址
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN03_Fn04_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN02 F1 数据处理
 *
 *  读取主节点地址
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN03_Fn04_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN02 F1 数据下发
 *
 *  本地通信模块运行模式信息
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN03_Fn10_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN02 F1 数据处理
 *
 *  本地通信模块运行模式信息
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN03_Fn10_up(PROTOCOL_GW1376_2_DATA *pdata);

/*****************************************************************************/
/*                                   AFN05 控制命令                          */
/*****************************************************************************/
/**
 *  \brief AFN02 F1 数据下发
 *
 *  读取主节点地址
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN02 F1 数据处理
 *
 *  读取主节点地址
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN05 F2 数据下发
 *
 *  子节点主动上报
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata);


/**
 *  \brief AFN05 F2 数据处理
 *
 *  子节点主动上报
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn02_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN05 F3 广播处理
 *
 *  读取主节点地址
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn03_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN05 F2 广播处理
 *
 *  子节点主动上报
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn03_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN02 F4 数据下发
 *
 *  设置超时时间
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn04_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN05 F4 数据处理
 *
 *  设置超时时间
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN05_Fn04_up(PROTOCOL_GW1376_2_DATA *pdata);

/*****************************************************************************/
/*                                   AFN06 主动上报                         */
/*****************************************************************************/
/**
 *  \brief AFN06 05
 *
 *  上报从节点事件
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN06_Fn05_up(PROTOCOL_GW1376_2_DATA *pdata);

int protocol_gw1376_AFN06_Fn05_down(PROTOCOL_GW1376_2_DATA *pdata);

/*****************************************************************************/
/*                                   AFN10 路由查询                         */
/*****************************************************************************/
/**
 *  \brief AFN10 F1 数据下发
 *
 *  读取从节点数量
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN10_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN10 F1 数据处理
 *
 *  读取从节点数量
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN10_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN10 F2 数据下发
 *
 *  读取从节点数量
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN10_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN10 F2 数据处理
 *
 *  读取从节点数量
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN10_Fn02_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN10 F4 路由运行状态
 *
 *  读取从节点数量
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN10_Fn04_down(PROTOCOL_GW1376_2_DATA *pdata);
int protocol_gw1376_AFN10_Fn04_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN10 F21 路由拓扑状态
 *
 *  读取从节点数量
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN10_Fn21_down(PROTOCOL_GW1376_2_DATA *pdata);
int protocol_gw1376_AFN10_Fn21_up(PROTOCOL_GW1376_2_DATA *pdata);
int protocol_gw1376_AFN10_Fn21_down_all(PROTOCOL_GW1376_2_DATA *pdata);
int protocol_gw1376_AFN10_Fn21_up_all(PROTOCOL_GW1376_2_DATA *pdata);


/**
 *  \brief AFN10 F2 路由运行状态
 *
 *  读取从节点数量
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN10_Fn04_up(PROTOCOL_GW1376_2_DATA *pdata);

/*****************************************************************************/
/*                                   AFN11 路由设置                         */
/*****************************************************************************/
/**
 *  \brief AFN11 F1 数据下发
 *
 *  添加从节点
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN11_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN11 F1 数据处理
 *
 *  添加从节点
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN11_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN11 F1 数据下发
 *
 *  添加从节点中继
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN11_Fn03_down(PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief AFN11 F1 数据处理
 *
 *  添加从节点中继
 *
 *  \param
 *  \return <0 失败
 *  \return =0 成功
 */
int protocol_gw1376_AFN11_Fn03_up(PROTOCOL_GW1376_2_DATA *pdata);

#endif /* _PROTOCOL_GW1376_2_AFN_H_ */
