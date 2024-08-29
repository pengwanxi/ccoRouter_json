/**
 *   \file maths.h
 *   \brief 设计一些计算
 */

/* #ifndef _MATHS_H_ */
/* #define _MATHS_H_ */

#include "basetype.h"

#define NR_BITS_BYTE (8)
#define BITMASK_BYTE (0xFF)

#define BIT_SET(num, i) ((num) |= (1 << (i))) /*  */
#define BIT_GET(num, i) (((num) & ((unsigned int)1 << (i))) > 0 ? true : false)
#define BIT_CLR(num, i) ((num) &= ~(1 << (i)))
#define BIT_INV(num, i) ((num) ^= (1 << (i)))

#define MOD_INC(x, mod) x = (((x) + 1) < (mod) ? ((x) + 1) : 0) //
#define MOD_DEC(x, mod) x = ((x) > 0 ? ((x) - 1) : (mod))       //
#define MOD_ADD(x, y, mod) \
    x = (((x) + (y)) < (mod) ? ((x) + (y)) : (((x) + (y)) - (mod))) //
#define MOD_SUB(x, y, mod) \
    x = (((x) < (y)) ? ((mod) - ((y) - (x))) : ((x) - (y))) //

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define BCD2HEX(x) (((x) >> 4) * 10 + ((x) & 0x0F))        /* 20H -> 20 */
#define HEX2BCD(x) (((x) % 10) + ((((x) / 10) % 10) << 4)) /* 20 -> 20H */

#define MAKEWORD(a, b)                      \
    ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | \
            ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)                         \
    ((DWORD)(((WORD)(((DWORD)(a)) & 0xffff)) | \
             ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define MAKEDWORD(a, b)                        \
    ((DWORD)(((WORD)(((DWORD)(a)) & 0xffff)) | \
             ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define LOWORD(l) ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w) ((BYTE)(((WORD)(w)) & 0xff))
#define HIBYTE(w) ((BYTE)((((WORD)(w)) >> 8) & 0xff))
#define WORDSWAP(l) (HIWORD(l) | (LOWORD(l) << 16))
#define BYTESWAP(w) (HIBYTE(w) | (LOBYTE(w) << 8))

#define BETWEEN(a, b, c) ((((a) >= (b)) && ((a) <= (c))) ? true : false)

/*闰年判断*/
#define IS_LEAP_YEAR(y) \
    (((((y) % 4) == 0) && (((y) % 100) != 0)) || (((y) % 400) == 0))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

/* #endif /\* _MATHS_H_ *\/ */
