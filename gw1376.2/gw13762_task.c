#include "gw13762_task.h"
#include "protocol_gw1376_2_data.h"
#include "zlog.h"
#include "maths.h"

#include <string.h>
#include <time.h>
#include <sys/time.h>

static int gs_task_index = 0;

int gw13762_task_read_queue_init(GW13762_TASK *pdata);

static char *lname()
{
    return (char *)"gw13762_task";
}

static int task_data_equal(void *a, void *b)
{
    GW13762_TASK_DATA *pa = (GW13762_TASK_DATA *)a;
    GW13762_TASK_DATA *pb = (GW13762_TASK_DATA *)b;

    /* dzlog_debug("%d %d\n", pa->index, pb->index); */

    return (pa->index == pb->index);
}

static void task_data_free(void *val)
{
    free(val);
}

int gw13762_task_init(GW13762_TASK *pdata)
{
    if (NULL == pdata)
    {
        dzlog_warn("%s pdata null", lname());
        return false;
    }

    pdata->plist = list_new();
    pdata->plist->len = 0;
    pdata->plist->match = task_data_equal;
    pdata->plist->free = task_data_free;
    // gw13762_task_read_queue_init(pdata);
    return 0;
}

int gw13762_task_destroy(GW13762_TASK *pdata)
{
    list_destroy(pdata->plist);
    pdata->plist = NULL;
    return 0;
}

bool gw13762_task_idle(GW13762_TASK *pdata)
{
    
    if (pdata->plist->len > 0)
    {
        return false;
    }

    return true;
}

int gw13762_task_push(GW13762_TASK *pdata, GW13762_TASK_DATA *ptask_data)
{
    int ret = -1;

    if (NULL == ptask_data)
    {
        dzlog_warn("%s ptask_data null", lname());
        return -1;
    }

    unsigned int size = sizeof(GW13762_TASK_DATA);
    void *val = (void *)malloc(size);
    if (val == NULL)
    {
        dzlog_warn("%s val null", lname());
        return -1;
    }

    if (!ptask_data->ismanu_index)
    {
        MOD_INC(gs_task_index, 0xff);
        ptask_data->index = gs_task_index;
    }

    if (0 == ptask_data->timeout_msec)
    {
        ptask_data->timeout_msec = PROTOCOL_GW1376_2_SUBNODE_TIMEOUT * 1000;
    }

    memcpy(val, ptask_data, size);
    list_node_t *node = list_node_new(val);

    dzlog_debug("%s push %d data", lname(), gs_task_index);
    if (NULL == list_rpush(pdata->plist, node))
    {
        dzlog_warn("%s push falid", lname());
        free(val);
        return -1;
    }

    return ptask_data->index;
}

int gw13762_task_push_head(GW13762_TASK *pdata, GW13762_TASK_DATA *ptask_data)
{
    int ret = -1;

    if (NULL == ptask_data)
    {
        dzlog_warn("%s ptask_data null", lname());
        return -1;
    }

    unsigned int size = sizeof(GW13762_TASK_DATA);
    void *val = (void *)malloc(size);
    if (val == NULL)
    {
        dzlog_warn("%s val null", lname());
        return -1;
    }

    if (!ptask_data->ismanu_index)
    {
        MOD_INC(gs_task_index, 0xff);
        ptask_data->index = gs_task_index;
    }

    if (0 == ptask_data->timeout_msec)
    {
        ptask_data->timeout_msec = PROTOCOL_GW1376_2_SUBNODE_TIMEOUT * 1000;
    }

    memcpy(val, ptask_data, size);
    list_node_t *node = list_node_new(val);

    dzlog_debug("%s push %d data", lname(), gs_task_index);
    if (NULL == list_lpush(pdata->plist, node))
    {
        dzlog_warn("%s lpush falid", lname());
        free(val);
        return -1;
    }

    return 0;
}

/**
 *  \brief 插入任务
 *  \param int
 *  \param data
 *  \return 0
 */
int gw13762_task_pop(GW13762_TASK *pdata, GW13762_TASK_DATA *ptask_data)
{
    int ret = -1;

    if (NULL == ptask_data)
    {
        dzlog_warn("%s ptask_data null", lname());
        return -1;
    }

    list_node_t *p = list_lpop(pdata->plist);
    if (NULL != p)
    {
        memcpy(ptask_data, p->val, sizeof(GW13762_TASK_DATA));
        free(p->val);
    }
    else
    {
        dzlog_warn("%s no data", lname());
        return -1;
    }

    return 0;
}

/**
 *  \brief 查找任务
 *  \param int
 *  \param data
 *  \return 0
 */
GW13762_TASK_DATA *gw13762_task_find(GW13762_TASK *pdata, int index)
{
    GW13762_TASK_DATA data;
    data.index = index;

    list_node_t *p = list_find(pdata->plist, &data);
    if (NULL != p)
    {
        return (GW13762_TASK_DATA *)p->val;
    }

    dzlog_warn("%s index=%d not find", lname(), index);
    return NULL;
}

GW13762_TASK_DATA *gw13762_task_head(GW13762_TASK *pdata)
{
    GW13762_TASK_DATA data;

    if (gw13762_task_idle(pdata))
    {
        return NULL;
    }

    list_node_t *p = pdata->plist->head;
    if (NULL != p)
    {
        return (GW13762_TASK_DATA *)p->val;
    }

    dzlog_warn("%s head null", lname());
    return NULL;
}

/**
 *  \brief 查找任务
 *  \param int
 *  \param data
 *  \return 0
 */
int gw13762_task_remove(GW13762_TASK *pdata, int index)
{
    GW13762_TASK_DATA data;
    data.index = index;

    list_node_t *p = list_find(pdata->plist, &data);
    /* printf("len=%d\n", pdata->plist->len); */
    if (NULL != p)
    {
        list_remove(pdata->plist, p);
        dzlog_debug("%s index=%d removed", lname(), index);
        return 0;
    }

    dzlog_warn("%s index=%d not remove", lname(), index);
    return 0;
}

/**
 *  \brief 查看 task 空闲状态
 *  \param void
 *  \return bool
 */
int gw13762_task_clear(GW13762_TASK *pdata)
{
    GW13762_TASK_DATA data;
    while (pdata->plist->len > 0)
    {
        gw13762_task_pop(pdata, &data);
    }

    return 0;
}

/**
 *  \brief 读任务队列显示
 *  \param
 *  \return 0
 */
int gw13762_task_read_queue_display(GW13762_TASK *pdata)
{
    list_iterator_t *it = list_iterator_new(pdata->plist, LIST_HEAD);

    list_node_t *p = list_iterator_next(it);
    while (NULL != p)
    {
        GW13762_TASK_DATA *ptdata = (GW13762_TASK_DATA *)p->val;
        if (NULL != ptdata)
        {
            printf("%s index=%d type=%d \n", lname(), ptdata->index,
                   ptdata->type);
        }

        p = list_iterator_next(it);
    }

    list_iterator_destroy(it);

    return 0;
}

/**
 *  \brief 读任务队列显示
 *  \param
 *  \return 0
 */
int gw13762_task_read_queue_clear_timeout(GW13762_TASK *pdata)
{
    list_iterator_t *it = list_iterator_new(pdata->plist, LIST_HEAD);

    list_node_t *p = list_iterator_next(it);
    while (NULL != p)
    {
        GW13762_TASK_DATA *ptdata = (GW13762_TASK_DATA *)p->val;
        if (NULL != ptdata)
        {
            long timeout = 10;
            if (ptdata->timeout_msec)
            {
                timeout = ptdata->timeout_msec;
            }

            struct timeval time_val;
            if (gettimeofday(&time_val, NULL))
            {
                return -1;
            }

            if (labs(time_val.tv_sec - ptdata->record_msec) > timeout)
            {
                p = list_iterator_next(it);
                gw13762_task_remove(pdata, ptdata->index);

                continue;
            }
        }

        p = list_iterator_next(it);
    }

    list_iterator_destroy(it);

    return 0;
}

/**
 *  \brief 清除任务头队列
 *  \param
 *  \return 0
 */
int gw13762_task_read_queue_clear_head_timeout(GW13762_TASK *pdata)
{
    GW13762_TASK_DATA *ptdata = gw13762_task_head(pdata);
    if (NULL != ptdata)
    {
        long timeout = ptdata->timeout_msec;

        struct timeval time_val;
        if (gettimeofday(&time_val, NULL))
        {
            return -1;
        }

        if (!ptdata->isbroadcast)
        {
            /* printf("timeout_sec=%d\n", ptdata->timeout_sec); */
            if (ptdata->timeout_msec == 0)
            {
                if (ptdata->resend_times < 0)
                {
                    gw13762_task_remove(pdata, ptdata->index);
                    return 1;
                }
                ptdata->resend_times--;
                return 0;
            }
            else
            {
                if (ptdata->record_msec == 0)
                {
                    ptdata->record_msec =
                        time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
                    return 0;
                }

                /* printf("ptdata=%p sec=%lu %lu %lu\n", ptdata, time_val.tv_sec, */
                /*        ptdata->record_sec, timeout); */
                int cur_msec = time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
                if (labs(cur_msec - ptdata->record_msec) > timeout)
                {
                    if (ptdata->resend_times < 0)
                    {
                        gw13762_task_remove(pdata, ptdata->index);
                        return 1;
                    }
                    ptdata->resend_times--;
                }
                else
                {
                    dzlog_debug("wait\n");
                    return -1;
                }
            }
        }
        else
        {
            if (ptdata->timeout_msec == 0)
            {
                return 0;
            }

            if (ptdata->record_msec == 0)
            {
                ptdata->record_msec =
                    time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
                return 0;
            }

            /* printf("ptdata=%p sec=%lu %lu\n", ptdata, time_val.tv_sec, */
            /*        ptdata->record_sec); */
            int cur_msec = time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
            if (labs(cur_msec - ptdata->record_msec) > timeout)
            {
                gw13762_task_remove(pdata, ptdata->index);
                return 1;
            }
            else
            {
                dzlog_debug("wait\n");
                return -1;
            }
        }
    }

    printf("not clear\n");
    return 0;
}

/**
 *  \brief 读任务队列初始化
 *  \param
 *  \return 0
 */
int gw13762_task_read_queue_init(GW13762_TASK *pdata)
{
    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(data));
    gw13762_task_clear(pdata);

    data.type = GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP;
    gw13762_task_push(pdata, &data);

    return 0;
}

/**
 *  \brief 读任务队列初始化
 *  \param
 *  \return 0
 */
int gw13762_task_set_queue_init(GW13762_TASK *pdata)
{
    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(data));
    gw13762_task_clear(pdata);

    data.type = GW1376_2_DATA_TYPE_PARAM_INIT;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_DATA_INIT;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE;
    gw13762_task_push(pdata, &data);

    /*
    gw13762_task_push(pdata, &data);
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_ROUTE_SET_SUBNODE_RELAY;
    gw13762_task_push(pdata, &data);
    */

    data.type = GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT;
    gw13762_task_push(pdata, &data);

    data.type = GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP;
    gw13762_task_push(pdata, &data);

    return 0;
}

/**
 *  \brief 设置任务数据
 *  \param
 *  \return 0
 */
int gw13762_task_set_data_buf(GW13762_TASK_DATA *ptdata, unsigned char *buf,
                              int len)
{
    if (NULL != ptdata && len > 0)
    {
        ptdata->len = len;
        memcpy(ptdata->buf, buf, len);

        return len;
    }

    return 0;
}

/**
 *  \brief 拿取任务数据
 *  \param
 *  \return 0
 */
int gw13762_task_get_data_buf(GW13762_TASK_DATA *ptdata, unsigned char *buf,
                              bool bfree)
{
    if (NULL != ptdata)
    {
        if (ptdata->len > 0)
        {
            int len = ptdata->len;
            memcpy(buf, ptdata->buf, len);
            if (bfree)
            {
                memset(ptdata->buf, 0, sizeof(ptdata->buf));
                ptdata->len = 0;
            }
            return len;
        }
    }

    return 0;
}

int gw13762_task_add_query_type(GW13762_TASK *pdata, int type)
{
    GW13762_TASK_DATA data;
    memset(&data, 0, sizeof(data));

    data.type = type;
    gw13762_task_push(pdata, &data);

    return 0;
}
