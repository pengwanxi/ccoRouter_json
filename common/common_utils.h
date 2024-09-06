#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include <string>
#include <vector>
#include "define.h"
#include "zlog.h"

typedef struct
{
	unsigned ms : 16;
	unsigned min : 6;
	unsigned : 2;
	unsigned hour : 5;
	unsigned : 3;
	unsigned day : 5;
	unsigned week : 3;
	unsigned month : 4;
	unsigned : 4;
	unsigned year : 7;
	unsigned : 1;
} CP56_TIME2A_T;

typedef struct
{
	uint16 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 minute;
	uint8 second;
	uint8 week;
	uint32 us;

} CO_TIME;

typedef union
{
	uint8 a_time[7];
	CP56_TIME2A_T time_t;
} CP56_TIME2A;


// 反转数组
void reverseHexArraycom(char *outArray, char *hexArray, int length);

char hexCharToValue(char c);
// 将十六进制字符串转换为字节数组
void stringToHexArray(const char *hexStr, char *hexArray, int *length);
// 16进制数组转字符串
void hexArrayToString(const char *hexArray, int length, char *outStr);

void zlog_print(zlog_category_t *zlogU, char *msg, const char *buf, int bufSize);

void sys_gettime_tm(CO_TIME *tm_t);
void sys_gettime_CP56(CP56_TIME2A *tm_t);

// 获取规定格式的时间字符串
void sys_gettime_timestamp(char *szTime);
// 二进制数据转化成为BASE64编码字符串
std::string encode_base64(unsigned char *buff, int nSize);
// BASE64编码字符串转化为二进制数据
std::string decode_base64(unsigned char *buff, int nSize);
// 按照规定的分割符 separator 分割字符串src 新的字符串数组通过dest返回
void split(const std::string &src, const std::string &separator, std::vector<std::string> &dest);
// 组织topic
void sysOutputTopic(const char *sendName, const char *recvName, const char *set, const char *cmd, const char *interface, char *topic);
void sysOutputTopic(std::vector<std::string> vectopic, char *topic);

// 计算传入时间的和现在相比的差值
int calcStrTime(const char *timestamp);

// 获取字符串中的数字
int extractNum(const char *c, char *ch);

// 整形转可变长度字符串
int numToChar(unsigned int num, char *desc);

// CRC16 校验
unsigned short CRC16Check(const unsigned char *pMsg, int length);

// 获取进程id号
int getProcessIdByName(const std::string &processName);

#endif
