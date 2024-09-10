
#include "common_utils.h"
#include "globalbase.h"
#include "cJSON.h"

/**
 *  \brief bcd 转 字符
 *  \param bcd
 *  \param 字符串
 *  \param 长度
 *  \return void
 */
void com_btos(const char *inBCD, char *outStr, int len)
{
	int i = 0, j = 0;

	if (NULL == inBCD || NULL == outStr || len < 1)
		return;

	for (i = len - 1, j = 0; i >= 0; i--)
	{
		char c0 = inBCD[i] & 0xF;
		char c1 = (inBCD[i] >> 4) & 0xF;
		if (c1 >= 0 && c1 <= 9)
			outStr[j++] = c1 + '0';
		else
			outStr[j++] = c1 + 'A';

		if (c0 >= 0 && c0 <= 9)
			outStr[j++] = c0 + '0';
		else
			outStr[j++] = c0 + 'A';
	}
}

// 反转数组

void reverseHexArraycom(char *outArray, char *hexArray, int length)
{
	int start = 0;
	int end = length - 1;

	while (start < length)
	{
		// 交换hexArray[start]和hexArray[end]
		outArray[start] = hexArray[end];
		start++;
		end--;
	}
}

// 将单个十六进制字符转换为对应的数值
char hexCharToValue(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else
	{
		return 0; // 非法字符处理
	}
}

// 将十六进制字符串转换为字节数组
void stringToHexArray(const char *hexStr, char *hexArray, int *length)
{
	int hexStrLen = 0;
	while (hexStr[hexStrLen] != '\0')
	{
		hexStrLen++;
	}

	if (hexStrLen % 2 != 0)
	{
		// 如果字符串长度不是偶数，则无法正确转换为字节
		printf("Error: Hex string length is not even.\n");
		*length = 0;
		return;
	}

	*length = hexStrLen / 2; // 每两个字符表示一个字节

	for (int i = 0; i < *length; i++)
	{
		// 将每两个字符转换为一个字节
		unsigned char highNibble = hexCharToValue(hexStr[2 * i]) << 4; // 高四位
		unsigned char lowNibble = hexCharToValue(hexStr[2 * i + 1]);   // 低四位
		hexArray[i] = highNibble | lowNibble;						   // 合并为一个字节
	}
}

// 16进制数组转字符串
void hexArrayToString(const char *hexArray, int length, char *outStr)
{
	if (hexArray == NULL)
	{
		return;
	}
	const char hexDigits[] = "0123456789ABCDEF"; // 用于将每个字节转换为对应的十六进制字符

	for (int i = 0; i < length; i++)
	{
		// 将高四位和低四位转换为对应的十六进制字符
		outStr[i * 2] = hexDigits[(hexArray[i] >> 4) & 0x0F]; // 高四位
		outStr[i * 2 + 1] = hexDigits[hexArray[i] & 0x0F];	  // 低四位
	}

	// 字符串结束符
	outStr[length * 2] = '\0';
}

void zlog_print(zlog_category_t *zlogU, char *msg, const char *buf, int bufSize)
{
	/*
	char *printBuf = (char *)malloc(bufSize * sizeof(char));
	if (printBuf == NULL)
	{
		zlog_error(zlogU, "malloc failed!");
		return;
	}*/
	char printBuf[1024] = {0};
	for (int i = 0; i < bufSize; i++)
	{
		sprintf(printBuf + (i * 3), "%02hhx ", buf[i]);
	}
	zlog_info(zlogU, msg, printBuf);
}

int getProcessIdByName(const std::string &processName)
{
	std::string command = "pidof " + processName;
	std::string result;
	char buffer[128];
	FILE *pipe = popen(command.c_str(), "r");
	if (!pipe)
	{
		std::cerr << "Error: Unable to execute command." << std::endl;
		return -1;
	}
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe) != nullptr)
		{
			result += buffer;
		}
	}
	pclose(pipe);
	if (result == "")
	{
		return -1;
	}
	return std::stoi(result);
}

void split(const std::string &src, const std::string &separator, std::vector<std::string> &dest) // 字符串分割到数组
{
	if (src.empty())
		return;

	std::string strContent, strTemp;
	strContent = src;
	std::string::size_type nBeginPos = 0, nEndPos = 0;
	while (true)
	{
		nEndPos = strContent.find(separator, nBeginPos);
		if (nEndPos == std::string::npos)
		{
			strTemp = strContent.substr(nBeginPos, strContent.length());
			if (!strTemp.empty())
			{
				dest.push_back(strTemp);
			}
			break;
		}
		strTemp = strContent.substr(nBeginPos, nEndPos - nBeginPos);
		nBeginPos = nEndPos + separator.size();
		dest.push_back(strTemp);
	}
}
void sysOutputTopic(const char *sendName, const char *recvName, const char *set, const char *cmd, const char *interface, char *topic)
{
	sprintf(topic, "%s/%s/JSON/%s/%s/%s", sendName, recvName, set, cmd, interface);
	// h_printf(MAIN_DEBUG_EN,"topic is:  %s\n",str);
	return;
}

void sysOutputTopic(std::vector<std::string> vectopic, char *topic) // JSon有很多
{
	std::string temp;
	// temp += "/";
	for (size_t i = 0; i < vectopic.size(); i++)
	{
		if (i + 1 == vectopic.size())
			temp += vectopic.at(i).c_str();
		else
		{
			temp += vectopic.at(i).c_str();
			temp += "/";
		}
	}

	sprintf(topic, "%s", temp.c_str());
	return;
}

void sys_gettime_tm(CO_TIME *tm_t)
{
	struct tm *p;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	// p=gmtime(&tv.tv_sec);
	p = localtime(&tv.tv_sec);

	tm_t->year = 1900 + p->tm_year; // tm_year
	// printf("year=%d",tm_t->year);
	tm_t->month = p->tm_mon + 1; // tm_mon
	// printf("month=%d",tm_t->month);
	tm_t->day = p->tm_mday;
	// printf("day=%d",tm_t->day);
	tm_t->hour = p->tm_hour;
	// printf("hour=%d",tm_t->hour);
	tm_t->minute = p->tm_min;
	// printf("minute=%d",tm_t->minute);
	tm_t->second = p->tm_sec;
	// printf("second=%d",tm_t->second);
	tm_t->week = p->tm_wday;
	// printf("week=%d",tm_t->week);+
	tm_t->us = tv.tv_usec;
	// printf("us=%d\n",tm_t->us);
}

void sys_gettime_timestamp(char *szTime)
{
	//	2021-09-09T14:28:42.757+0800

	time_t _rt = time(NULL);
	// 系统时间转换为GMT时间
	tm _gtm = *gmtime(&_rt);
	// 系统时间转换为本地时间
	//	tm _ltm = *localtime(&_rt);
	time_t _gt = mktime(&_gtm);
	tm _gtm2 = *localtime(&_gt);
	// 这时的_gt已经与实际的系统时间_rt有时区偏移了,计算两个值的之差就是时区偏的秒数,除60就是分钟
	int offset = ((_rt - _gt) + (_gtm2.tm_isdst ? 3600 : 0)) / 60 / 60;

	struct tm *p;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	p = localtime(&tv.tv_sec);
	sprintf(szTime, "%04d-%02d-%02dT%02d:%02d:%02d.%03d+%02d00", p->tm_year + 1900, p->tm_mon + 1,
			p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, int(tv.tv_usec / 1000), offset);
}

void sys_gettime_CP56(CP56_TIME2A *tm_t)
{
	CO_TIME co_tm_t;
	sys_gettime_tm(&co_tm_t);

	tm_t->time_t.year = (uint8)(co_tm_t.year - 2000);
	tm_t->time_t.month = co_tm_t.month;
	tm_t->time_t.day = co_tm_t.day;
	tm_t->time_t.week = co_tm_t.week;
	tm_t->time_t.hour = co_tm_t.hour;
	tm_t->time_t.min = co_tm_t.minute;
	tm_t->time_t.ms = co_tm_t.second * 1000 + co_tm_t.us / 1000;
}

/* BASE 64 encode table */
static const char encode_table[] = {
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'+',
	'/',
};

/* ASCII order for BASE 64 decode, 255 in unused character */
static const unsigned char decode_table[] = {
	/* nul, soh, stx, etx, eot, enq, ack, bel, */
	255, 255, 255, 255, 255, 255, 255, 255,
	/*  bs,  ht,  nl,  vt,  np,  cr,  so,  si, */
	255, 255, 255, 255, 255, 255, 255, 255,
	/* dle, dc1, dc2, dc3, dc4, nak, syn, etb, */
	255, 255, 255, 255, 255, 255, 255, 255,
	/* can,  em, sub, esc,  fs,  gs,  rs,  us, */
	255, 255, 255, 255, 255, 255, 255, 255,
	/*  sp, '!', '"', '#', '$', '%', '&', ''', */
	255, 255, 255, 255, 255, 255, 255, 255,
	/* '(', ')', '*', '+', ',', '-', '.', '/', */
	255, 255, 255, 62, 255, 255, 255, 63,
	/* '0', '1', '2', '3', '4', '5', '6', '7', */
	52, 53, 54, 55, 56, 57, 58, 59,
	/* '8', '9', ':', ';', '<', '=', '>', '?', */
	60, 61, 255, 255, 255, 255, 255, 255,
	/* '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', */
	255, 0, 1, 2, 3, 4, 5, 6,
	/* 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', */
	7, 8, 9, 10, 11, 12, 13, 14,
	/* 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', */
	15, 16, 17, 18, 19, 20, 21, 22,
	/* 'X', 'Y', 'Z', '[', '\', ']', '^', '_', */
	23, 24, 25, 255, 255, 255, 255, 255,
	/* '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', */
	255, 26, 27, 28, 29, 30, 31, 32,
	/* 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', */
	33, 34, 35, 36, 37, 38, 39, 40,
	/* 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', */
	41, 42, 43, 44, 45, 46, 47, 48,
	/* 'x', 'y', 'z', '{', '|', '}', '~', del, */
	49, 50, 51, 255, 255, 255, 255, 255};

#define BASE64_ENCODE_OUT_SIZE(s) ((unsigned int)((((s) + 2) / 3) * 4 + 1))
#define BASE64_DECODE_OUT_SIZE(s) ((unsigned int)(((s) / 4) * 3))

std::string encode_base64(unsigned char *buff, int nSize)
{

	std::string output_str = "";
	int s;
	int i;
	unsigned char c;
	unsigned char l;

	s = 0;
	l = 0;
	for (i = 0; i < nSize; i++)
	{
		c = buff[i];
		switch (s)
		{
		case 0:
			s = 1;
			output_str.push_back(encode_table[(c >> 2) & 0x3F]);
			break;
		case 1:
			s = 2;
			output_str.push_back(encode_table[((l & 0x3) << 4) | ((c >> 4) & 0xF)]);
			break;
		case 2:
			s = 0;
			output_str.push_back(encode_table[((l & 0xF) << 2) | ((c >> 6) & 0x3)]);
			output_str.push_back(encode_table[c & 0x3F]);
			break;
		}
		l = c;
	}

	switch (s)
	{
	case 1:
		output_str.push_back(encode_table[(l & 0x3) << 4]);
		output_str.push_back('=');
		output_str.push_back('=');
		break;
	case 2:
		output_str.push_back(encode_table[(l & 0xF) << 2]);
		output_str.push_back('=');
		break;
	}
	return output_str;
}

std::string decode_base64(unsigned char *buff, int nSize)
{
	std::string output_str = "";

	int i;
	unsigned int j;
	unsigned char c;

	char out[3] = {0};

	if (nSize & 0x3)
	{
		return output_str;
	}

	for (i = j = 0; i < nSize; i++)
	{
		if (buff[i] == '=')
		{
			break;
		}
		if (buff[i] < '+' || buff[i] > 'z')
		{
			break;
		}

		c = decode_table[(unsigned char)buff[i]];
		if (c == 255)
		{
			break;
		}

		switch (i & 0x3)
		{
		case 0:
			out[j] = (c << 2) & 0xFF;
			break;
		case 1:
			out[j++] |= (c >> 4) & 0x3;
			out[j] = (c & 0xF) << 4;
			break;
		case 2:
			out[j++] |= (c >> 2) & 0xF;
			out[j] = (c & 0x3) << 6;
			break;
		case 3:
			out[j++] |= c;
			output_str.append(out, 3);
			j = 0;
			break;
		}
	}
	int diff = BASE64_DECODE_OUT_SIZE(nSize) - output_str.size();
	if (diff > 0)
		output_str.append(out, diff);
	return output_str;
}

int calcStrTime(const char *timestamp)
{
	int nyear, nmon, nday, hour, min, sec, ns;
	sscanf(timestamp, "%04d-%02d-%02dT%02d:%02d:%02d.%03d+00", &nyear,
		   &nmon, &nday, &hour, &min, &sec, &ns);
	if (nyear == 0 && nmon == 0 && nday == 0)
	{
		return -1;
	}
	tm strtm;
	strtm.tm_year = nyear - 1900;
	strtm.tm_mon = nmon - 1;
	strtm.tm_mday = nday;
	strtm.tm_hour = hour;
	strtm.tm_min = min;
	strtm.tm_sec = sec;
	time_t t_str = mktime(&strtm);

	time_t _rt = time(NULL);

	int diff = _rt - t_str;
	if (diff < 0) // 代表当前时间是字符串时间之前
	{
		diff = 0;
	}

	return diff;
}

int extractNum(const char *c, char *ch)
{
	int amount = 0;
	int i = 0;
	while (c[i] != '\0')
	{
		if (c[i] >= '0' && c[i] <= '9')
		{
			ch[amount] = c[i];
			amount++;
		}
		i++;
	}
	return amount;
}

int numToChar(unsigned int num, char *desc)
{
	int nRet = 1;
	if (num > 0xffffff)
	{
		desc[0] = 0x84;
		desc[1] = (num >> 24) & 0xff;
		desc[2] = (num >> 16) & 0xff;
		desc[3] = (num >> 8) & 0xff;
		desc[4] = num & 0xff;
		nRet = 5;
	}
	else if (num > 0xffff)
	{
		desc[0] = 0x83;
		desc[1] = (num >> 16) & 0xff;
		desc[2] = (num >> 8) & 0xff;
		desc[3] = num & 0xff;
		nRet = 4;
	}
	else if (num > 0xff)
	{
		desc[0] = 0x82;
		desc[1] = (num >> 8) & 0xff;
		desc[2] = num & 0xff;
		nRet = 3;
	}
	else if (num > 0x7f) // 127
	{
		desc[0] = 0x81;
		desc[1] = num & 0xff;
		nRet = 2;
	}
	else
	{
		desc[0] = num & 0xff;
		nRet = 1;
	}
	return nRet;
}

static const unsigned char auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40};
/* Table of CRC values for low–order byte*/
static const unsigned char auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
	0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
	0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
	0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
	0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
	0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
	0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
	0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
	0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40};

/*******************************************************************************************************
 *function name:  GetCRC16
 *description:  16 bits crc
 *input parameter:  *pMsg(the first index of the crc datas);length(the number of the crc datas)
 *modify:
 ********************************************************************************************************/

unsigned short CRC16Check(const unsigned char *pMsg, int length) // CRC校验
{
	/*查表法*/
	unsigned char uchCRCHi = 0xff; // high byte of CRC initialized
	unsigned char uchCRCLo = 0xff; // low byte of CRC initialized
	unsigned char uIndex;		   // will index into CRC lookup table

	while (length--) // pass through message buffer
	{
		uIndex = uchCRCHi ^ *pMsg++; // calculate the CRC
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}
