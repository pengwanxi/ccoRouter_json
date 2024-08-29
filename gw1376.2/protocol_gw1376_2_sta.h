/**
 *   \file protocol_gw1376_2_sta.h
 *   \brief 涉及转发station 的一些功能
 */
#ifndef _PROTOCOL_GW1376_2_STA_H_
#define _PROTOCOL_GW1376_2_STA_H_

#include "protocol_gw1376_2_data.h"

/**
 *  \brief 获取协议数据
 *  \param char *buf
 *  \param int *len
 *  \param PRTOCOL_GW1376_2_DATA *pdata
 *  \return len
 */
int protocol_gw1376_2_sta_get_protocol_frame(char *buf, int *len,
                                             PROTOCOL_GW1376_2_DATA *pdata);

/**
 *  \brief 处理协议数据
 *  \param char *buf
 *  \param int *len
 *  \param PRTOCOL_GW1376_2_DATA *pdata
 *  \return len
 */
int protocol_gw1376_2_sta_process_protocol_frame(char *buf, int len,
                                                 PROTOCOL_GW1376_2_DATA *pdata);

#endif /* _PROTOCOL_GW1376_2_STA_H_ */
