#include "public.h"

/* unsigned char cal_cs(const unsigned char *buf, int len) */
/* { */
/*     unsigned char byRtn = 0x00; */
/*     int i; */

/*     if (buf == NULL || len <= 0) */
/*         return byRtn; */

/*     for (i = 0; i < len; i++) { */
/*         byRtn += buf[i]; */
/*     } */

/*     return byRtn; */
/* } */

unsigned char cal_cs(const unsigned char *buf, int len)
{
    unsigned char byRtn = 0x00;
    int i;

    if (buf == NULL || len <= 0)
        return byRtn;

    for (i = 0; i < len; i++) {
        byRtn += buf[i];
    }

    return byRtn;
}


void public_stob(const char *pstr, unsigned char *pbcd, int len)
{
    unsigned char tmpValue;
    int i;
    int j;
    int m;
    int sLen;

    sLen = strlen(pstr);
    for (i = 0; i < sLen; i++)
    {
        if ((pstr[i] < '0') || ((pstr[i] > '9') && (pstr[i] < 'A')) ||
            ((pstr[i] > 'F') && (pstr[i] < 'a')) || (pstr[i] > 'f'))
        {
            sLen = i;
            break;
        }
    }

    sLen = (sLen <= (len * 2)) ? sLen : sLen * 2;
    memset((void *)pbcd, 0x00, len);

    for (i = sLen - 1, j = 0, m = 0; (i >= 0) && (m < len); i--, j++)
    {
        if ((pstr[i] >= '0') && (pstr[i] <= '9'))
        {
            tmpValue = pstr[i] - '0';
        }
        else if ((pstr[i] >= 'A') && (pstr[i] <= 'F'))
        {
            tmpValue = pstr[i] - 'A' + 0x0A;
        }
        else if ((pstr[i] >= 'a') && (pstr[i] <= 'f'))
        {
            tmpValue = pstr[i] - 'a' + 0x0A;
        }
        else
        {
            tmpValue = 0;
        }

        if ((j % 2) == 0)
        {
            pbcd[m] = tmpValue;
        }
        else
        {
            pbcd[m++] |= (tmpValue << 4);
        }

        if ((tmpValue == 0) && (pstr[i] != '0'))
        {
            break;
        }
    }
}

u16_t cal_crc(u08_t *pbuf, int len)
{
    u16_t genpoly = 0xA001;
    u16_t crc = 0xFFFF;
    u16_t index;
    while (len--)
    {
        crc = crc ^ (u16_t)*pbuf++;
        for (index = 0; index < 8; index++)
        {
            if ((crc & 0x0001) == 1)
                crc = (crc >> 1) ^ genpoly;
            else
                crc = crc >> 1;
        }
    }

    return crc;
}

/* void get_crc_buf(u08_t *pbuf, int *len) */
/* { */
/*     u16_t crc = cal_crc(pbuf, *len); */

/*     pbuf[(*len)++] = LOBYTE(crc); */
/*     pbuf[(*len)++] = HIBYTE(crc); */
/* } */

/* u08_t dec2bcd(u08_t d) */
/* { */
/*     /\* return ((d / 100) << 8) | (((d % 100) / 10) << 4) | ((d % 100) % 10); *\/ */
/*     return (((d % 100) / 10) << 4) | (d % 10); */
/* } */

/* u08_t bcd2dec(u08_t c) */
/* { */
/*     return (u08_t)((c >> 4) * 10 + (c & 0x0f)); */
/* } */

void log_info_frame(char *buf, int len)
{
    char pbuf[PROTOCOL_BUF_LEN * 3 + 32] = "";
    char *ptr = pbuf;
    if (len > PROTOCOL_BUF_LEN)
    {
        ptr = (char *)malloc(len * 3 + 1);
    }

    int i;
    for (i = 0; i < len; i++)
    {
        sprintf(&ptr[3 * i], "%.2x ", (unsigned char)buf[i]);
    }
    dzlog_info("len=%.5d:%s", len, ptr);

    if (len > PROTOCOL_BUF_LEN)
    {
        free(ptr);
    }
}

void log_debug_frame(char *buf, int len)
{
    char pbuf[PROTOCOL_BUF_LEN * 3 + 1] = "";
    char *ptr = pbuf;
    if (len > PROTOCOL_BUF_LEN)
    {
        ptr = (char *)malloc(len * 3 + 1);
    }

    int i;
    for (i = 0; i < len; i++)
    {
        sprintf(&ptr[3 * i], "%.2x ", (unsigned char)buf[i]);
    }
    dzlog_debug("len=%.5d:%s", len, ptr);

    if (len > PROTOCOL_BUF_LEN)
    {
        free(ptr);
    }
}

/**
 *  \brief bcd 转 字符
 *  \param bcd
 *  \param 字符串
 *  \param 长度
 *  \return void
 */
void btos(const char *inBCD, char *outStr, int len)
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
