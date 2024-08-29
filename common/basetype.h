/**
 *   \file basetype.h
 *   \brief 基本类型
 */
/* #ifndef _BASETYPE_H_ */
/* #define _BASETYPE_H_ */

/* #include <stdbool.h> */

//定义数据类型
typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;
typedef int HANDLE;
typedef void *LPVOID;

#define UINT8 BYTE
#define UINT16 WORD

#ifndef true
#define true 1
#endif 
#ifndef false
#define false 0
#endif 

#ifndef NULL
#define NULL 0
#endif 


#ifndef bool
#define bool unsigned char 
#endif 

typedef bool BOOL;
#define FALSE false
#define TRUE true

#define OK 0
#define ERROR -1

////////////////////////////////////////////////////////////////////////////////
// 全局数据类型及数据结构定义
typedef signed char s08_t;
typedef signed short s16_t;
typedef signed int s32_t;
typedef unsigned char u08_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef long long int s64_t;
typedef unsigned long long int u64_t;
typedef float f32_t;
typedef double f64_t;

typedef signed char s8;
typedef signed short s16;
/* typedef signed int s32_t; */
typedef unsigned char u8;
typedef unsigned short u16;
/* typedef unsigned int u32_t; */
typedef long long int s64;
typedef unsigned long long int u64;
typedef float f32;
typedef double f64;

/* #endif /\* _BASETYPE_H_ *\/ */
