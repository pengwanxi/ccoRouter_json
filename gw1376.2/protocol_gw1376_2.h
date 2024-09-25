/**
 *   \file protocol_gw1376.2.h
 *   \brief 1376.2协议
 */
#ifndef _PROTOCOL_GW1376_2_H_
#define _PROTOCOL_GW1376_2_H_

#include "protocol_gw1376_2_data.h"

/**
 *  \brief 获取13762类型 函数指针
 *  \param void
 *  \return int
 */
typedef int (*get13762Type_t)(int);

void set_gettype_func(get13762Type_t func);

/**
 *  \brief 获取发送报文
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
int protocol_gw1376_2_get_sendbuf(char *buf, int *len, void *p);

/**
 *  \brief 处理数据
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
int protocol_gw1376_2_process_buf(char *buf, int len, void *p);

/**
 *  \brief 初始化
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
int protocol_gw1376_2_init(void *p);

/**
 *  \brief 初始化
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
void protocol_gw1376_2_exit(void *p);

#endif /* _PROTOCOL_GW1376_2_H_ */
