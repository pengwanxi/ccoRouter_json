/**
 *   \file 任务管理
 *   \brief 13762会根据任务队列工作
 */
#ifndef _GW13762_TASK_DATA_H_
#define _GW13762_TASK_DATA_H_
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define PROTOCOL_GW1376_2_TASK_NUM (256)

typedef bool (*PROTOCOL_SEND)(unsigned char *buf, int *len, void *p);
typedef bool (*PROTOCOL_RECV)(unsigned char *buf, int len, void *p);
#define PROTOCOL_BUF_LEN 512

/*  */
typedef struct _GW13762_TASK_DATA
{
	bool use;
	int index;
	int type;
	int ptype;
	bool isbroadcast;
	bool ismanu_index;
	int resend_times;

	char sta_addr[32];
	PROTOCOL_SEND protocol_send;
	PROTOCOL_RECV protocol_recv;

	char buf[PROTOCOL_BUF_LEN];
	int len;

	long timeout_msec;
	long record_msec;
} GW13762_TASK_DATA;

/*  */
typedef struct _GW13762_TASK
{
	list_t *plist;
} GW13762_TASK;

/**
 *  \brief 初始化
 *  \param void
 *  \return bool
 */
int gw13762_task_init(GW13762_TASK *pdata);

/**
 *  \brief 销毁
 *  \param void
 *  \return bool
 */
int gw13762_task_destroy(GW13762_TASK *pdata);

/**
 *  \brief 查看 task 空闲状态
 *  \param void
 *  \return bool
 */
bool gw13762_task_idle(GW13762_TASK *pdata);

/**
 *  \brief 查找任务
 *  \param int
 *  \param data
 *  \return 0
 */
GW13762_TASK_DATA *gw13762_task_find(GW13762_TASK *pdata, int index);

/**
 *  \brief 查找任务头
 *  \param int
 *  \param data
 *  \return 0
 */
GW13762_TASK_DATA *gw13762_task_head(GW13762_TASK *pdata);

/**
 *  \brief 删除任务
 *  \param int
 *  \param data
 *  \return 0
 */
int gw13762_task_remove(GW13762_TASK *pdata, int index);

/**
 *  \brief 插入任务
 *  \param int
 *  \param data
 *  \return 0
 */
int gw13762_task_push(GW13762_TASK *pdata, GW13762_TASK_DATA *ptask_data);

/**
 *  \brief 插入头任务
 *  \param int
 *  \param data
 *  \return 0
 */
int gw13762_task_push_head(GW13762_TASK *pdata, GW13762_TASK_DATA *ptask_data);

/**
 *  \brief 读任务队列显示
 *  \param
 *  \return 0
 */
int gw13762_task_read_queue_display(GW13762_TASK *pdata);

/**
 *  \brief 清除任务头队列
 *  \param
 *  \return 0
 */
int gw13762_task_read_queue_clear_head_timeout(GW13762_TASK *pdata);

/**
 *  \brief 设置任务数据
 *  \param
 *  \return 0
 */
int gw13762_task_set_data_buf(GW13762_TASK_DATA *ptdata, unsigned char *buf,
							  int len);

/**
 *  \brief 拿取任务数据
 *  \param
 *  \return 0
 */
int gw13762_task_get_data_buf(GW13762_TASK_DATA *ptdata, unsigned char *buf,
							  bool bfree);

/**
 *  \brief 写任务队列初始化
 *  \param
 *  \return 0
 */
int gw13762_task_set_queue_init(GW13762_TASK *pdata);

/**
 *  \brief 新增加类型
 *  \param
 *  \return 0
 */
int gw13762_task_add_query_type(GW13762_TASK *pdata, int type);

#endif /* _GW13762_TASK_DATA_H_ */
