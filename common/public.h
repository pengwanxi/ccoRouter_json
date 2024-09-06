/**
 *   \file public.h
 *   \brief 部分公用含数部分
 */
#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* #define pname "modbusapp" */
#include "datatype.h"

/* /\** */
/*  *  \brief 计算校验和 */
/*  *  \param buf */
/*  *  \param len */
/*  *  \return 校验和 */
/*  *\/ */
/* unsigned char cal_cs(const unsigned char *buf, int len); */

/**
 *  \brief 字符串转bcd
 *  \param 字符串
 *  \param bcd
 *  \param 长度
 *  \return void
 */
void public_stob(const char *pstr, unsigned char *pbcd,
                 int len);

// 反转数组
void reverseHexArray(char *outArray, char *hexArray, int length);
/*校验和*/
unsigned char cal_cs(const unsigned char *buf, int len);

/* /\** */
/*  *  \brief 10进制转bcd */
/*  *  \param 10进制字符 */
/*  *  \return bcd 码 */
/*  *\/ */
/* u08_t dec2bcd(u08_t d); */

/* /\** */
/*  *  \brief bcd转16进制 */
/*  *  \param bcd */
/*  *  \return 16进制 */
/*  *\/ */
/* u08_t bcd2dec(u08_t c); */

/* /\** */
/*  *  \brief 获取并赋值crc */
/*  *  \param 缓冲区 */
/*  *  \param 长度 */
/*  *  \return void */
/*  *\/ */
/* void get_crc_buf(u08_t *pbuf, int *len); */

/**
 *  \brief 打印16进制报文
 *  \param buf
 *  \param len
 *  \return
 */
void log_info_frame(char *buf, int len);
void log_debug_frame(char *buf, int len);

/**
 *  \brief bcd 转 字符
 *  \param bcd
 *  \param 字符串
 *  \param 长度
 *  \return void
 */
void btos(const char *inBCD, char *outStr, int len);

#endif /* _PUBLIC_H_ */
