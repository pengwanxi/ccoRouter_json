#include "protocol_gw1376_2_data.h"
#include "maths.h"
#include "zlog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PROTOCOL_GW1376_2_DATA gs_data;

PROTOCOL_GW1376_2_DATA *get_pdata(void)
{
    return &gs_data;
}

PROTOCOL_GW1376_2_SEND_DATA *
protocol_gw1376_2_send_data_get(PROTOCOL_GW1376_2_DATA *pdata)
{
    return &pdata->send;
}

PROTOCOL_GW1376_2_RECV_DATA *
protocol_gw1376_2_recv_data_get(PROTOCOL_GW1376_2_DATA *pdata)
{
    return &pdata->recv;
}

PROTOCOL_GW1376_2_APPLY_REGION *
protocol_gw1376_2_send_apply_region_get(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);

    return &psend->apply_region;
}

PROTOCOL_GW1376_2_APPLY_REGION *
protocol_gw1376_2_recv_apply_region_get(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    return &precv->apply_region;
}

int protocol_gw1376_2_set_send_apply_region_unit_buf(
    PROTOCOL_GW1376_2_DATA *pdata, unsigned char *buf, int len)
{
    PROTOCOL_GW1376_2_APPLY_REGION *pregin =
        protocol_gw1376_2_send_apply_region_get(pdata);

    if (len >= PROTOCOL_GW1376_2_APPLY_DATA_LEN)
    {
        len = PROTOCOL_GW1376_2_APPLY_DATA_LEN;
    }

    memcpy(pregin->unit_buf + 2, buf, len);
    pregin->unit_len = len + 2;
    pregin->unit_buf[1] = len;

    return pregin->unit_len;
}

int protocol_gw1376_2_send_apply_region_unit_buf(PROTOCOL_GW1376_2_DATA *pdata,
                                                 unsigned char *buf)
{
    if (NULL == buf)
    {
        return -1;
    }
    PROTOCOL_GW1376_2_APPLY_REGION *pregin =
        protocol_gw1376_2_send_apply_region_get(pdata);

    memcpy(buf, pregin->unit_buf, pregin->unit_len);

    return pregin->unit_len;
}

int protocol_gw1376_2_set_recv_apply_region_unit_buf(
    PROTOCOL_GW1376_2_DATA *pdata, unsigned char *buf, int len)
{
    PROTOCOL_GW1376_2_APPLY_REGION *pregin =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    if (len >= PROTOCOL_GW1376_2_APPLY_DATA_LEN)
    {
        len = PROTOCOL_GW1376_2_APPLY_DATA_LEN;
    }

    /* memcpy(pregin->unit_buf + 2, buf, len); */
    /* pregin->unit_len = len + 2; */

    memcpy(pregin->unit_buf, buf, len);

    return pregin->unit_len;
}

int protocol_gw1376_2_recv_apply_region_unit_buf(PROTOCOL_GW1376_2_DATA *pdata,
                                                 unsigned char *buf)
{
    if (NULL == buf)
    {
        return -1;
    }
    PROTOCOL_GW1376_2_APPLY_REGION *pregin =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    memcpy(buf, pregin->unit_buf, pregin->unit_len);

    return pregin->unit_len;
}

void protocol_gw1376_2_data_set_send_dir(PROTOCOL_GW1376_2_DATA *pdata,
                                         PROTOCOL_GW1376_2_DIR dir)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->dir = dir;

    BIT_CLR(psend->ctrl_region, 7);

    if (dir == GW1376_2_DIR_UP)
        BIT_SET(psend->ctrl_region, 7);
}

void protocol_gw1376_2_data_set_send_prm(PROTOCOL_GW1376_2_DATA *pdata,
                                         PROTOCOL_GW1376_2_PRM prm)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->prm = prm;

    BIT_SET(psend->ctrl_region, 6);

    if (prm == GW1376_2_PRM_SLAVE)
        BIT_CLR(psend->ctrl_region, 6);
}

bool protocol_gw1376_2_data_set_recv_prm(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    return precv->prm;
}

void protocol_gw1376_2_data_set_send_comm_type(
    PROTOCOL_GW1376_2_DATA *pdata, PROTOCOL_GW1376_2_COMM_TYPE comm_type)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    unsigned char type = comm_type;
    psend->comm_type = comm_type;

    if (type != (psend->ctrl_region & 0x3f))
    {
        psend->ctrl_region = psend->ctrl_region & 0xc0;
        psend->ctrl_region = psend->ctrl_region | (type & 0x3f);
    }
}

void protocol_gw1376_2_data_set_send_route_mode(PROTOCOL_GW1376_2_DATA *pdata,
                                                int route_mode)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->route_mode = route_mode;

    BIT_CLR(psend->info_region.data[0], 0);
    if (route_mode == GW1376_2_ROUTE_MODE_NO)
        BIT_SET(psend->info_region.data[0], 0);
}

void protocol_gw1376_2_data_set_send_subnode(PROTOCOL_GW1376_2_DATA *pdata,
                                             int subnode)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->subnode = subnode;

    BIT_CLR(psend->info_region.data[0], 1);
    if (subnode == GW1376_2_SUBNODE_HAVE)
        BIT_SET(psend->info_region.data[0], 1);
}

void protocol_gw1376_2_data_set_send_comm_module_flag(
    PROTOCOL_GW1376_2_DATA *pdata, int comm_module_flag)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->comm_module_flag = comm_module_flag;

    BIT_SET(psend->info_region.data[0], 2);
    if (comm_module_flag == GW1376_2_CCO)
        BIT_CLR(psend->info_region.data[0], 2);
}

void protocol_gw1376_2_data_set_send_conflict(PROTOCOL_GW1376_2_DATA *pdata,
                                              int conflict)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->conflict = conflict;

    BIT_CLR(psend->info_region.data[0], 3);
    if (conflict == GW1376_2_CONFLICT)
        BIT_SET(psend->info_region.data[0], 3);
}

void protocol_gw1376_2_data_set_send_relay_level(PROTOCOL_GW1376_2_DATA *pdata,
                                                 int relay_level)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->relay_level = relay_level;

    psend->info_region.data[0] &= 0xF;
    psend->info_region.data[0] |= ((relay_level & 0x0F) << 4);
}

void protocol_gw1376_2_data_set_send_channel(PROTOCOL_GW1376_2_DATA *pdata,
                                             int channel)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->channel = channel;

    psend->info_region.data[1] &= 0xF0;
    psend->info_region.data[1] |= (channel & 0x0f);
}

void protocol_gw1376_2_data_set_send_coding(PROTOCOL_GW1376_2_DATA *pdata,
                                            int coding)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->coding = coding;

    psend->info_region.data[1] &= 0x0F;
    psend->info_region.data[1] |= ((coding & 0x0F) << 4);
}

void protocol_gw1376_2_data_set_send_bytes_count(PROTOCOL_GW1376_2_DATA *pdata,
                                                 int bytes_count)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->bytes_count = bytes_count;

    psend->info_region.data[2] = bytes_count;
}

void protocol_gw1376_2_data_set_send_baud(PROTOCOL_GW1376_2_DATA *pdata,
                                          int baud)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->comm_baud = baud;

    unsigned short w = baud & 0x7fff;

    psend->info_region.data[3] = LOBYTE(w);
    psend->info_region.data[4] &= (0x80);
    psend->info_region.data[4] |= (HIBYTE(w) & 0x7F);
}

void protocol_gw1376_2_data_set_send_baud_flag(PROTOCOL_GW1376_2_DATA *pdata,
                                               int baud_flag)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    psend->comm_baud_flag = baud_flag;

    BIT_CLR(psend->info_region.data[4], 7);
    if (baud_flag == GW1376_2_BAUD_KBIT)
        BIT_SET(psend->info_region.data[4], 7);
}

void protocol_gw1376_2_data_set_send_buf_count(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    MOD_INC(pdata->buf_count, 0xff);
    psend->info_region.data[5] = pdata->buf_count;
}

void protocol_gw1376_2_data_set_send_task_data(PROTOCOL_GW1376_2_DATA *pdata,
                                               GW13762_TASK_DATA *ptask_data)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);

    psend->ptask_data = ptask_data;
}

void protocol_gw1376_2_data_set_state(PROTOCOL_GW1376_2_DATA *pdata, int state)
{
    if (pdata->state == state)
        return;

    pdata->state = state;
}

int protocol_gw1376_2_data_get_state(PROTOCOL_GW1376_2_DATA *pdata)
{
    return pdata->state;
}

void protocol_gw1376_2_data_set_data_type(PROTOCOL_GW1376_2_DATA *pdata,
                                          int type)
{
    if (pdata->type == type)
        return;

    pdata->type = type;
}

int protocol_gw1376_2_data_get_data_type(PROTOCOL_GW1376_2_DATA *pdata)
{
    return pdata->type;
}

/**
 *  \brief 初始化发送控制参数
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
static int
protocol_gw1376_2_send_control_data_init(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = &pdata->send;

    psend->subnode_auto_up = false;
    psend->subnode_auto_up_enable = true;

    protocol_gw1376_2_data_set_send_dir(pdata, GW1376_2_DIR_DOWN);
    protocol_gw1376_2_data_set_send_prm(pdata, GW1376_2_PRM_MASTER);
    protocol_gw1376_2_data_set_send_comm_type(pdata,
                                              GW1376_2_COMM_TYPE_HPLC_CARRIER);

    /* protocol_gw1376_2_data_set_send_route_mode(pdata, GW1376_2_ROUTE_MODE); */
    protocol_gw1376_2_data_set_send_route_mode(pdata, GW1376_2_ROUTE_MODE_NO);
    protocol_gw1376_2_data_set_send_subnode(pdata, GW1376_2_SUBNODE_NO);
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);
    protocol_gw1376_2_data_set_send_conflict(pdata, GW1376_2_CONFLICT_NO);
    protocol_gw1376_2_data_set_send_relay_level(pdata, GW1376_2_RELAY_LEVEL0);
    protocol_gw1376_2_data_set_send_channel(pdata, GW1376_2_CHANNEL0);
    protocol_gw1376_2_data_set_send_coding(pdata, GW1376_2_CODING_NO);
    protocol_gw1376_2_data_set_send_bytes_count(pdata, GW1376_2_BYTES_DEFAULT);
    protocol_gw1376_2_data_set_send_baud(pdata, 0);
    protocol_gw1376_2_data_set_send_baud_flag(pdata, GW1376_2_BAUD_BIT);

    return 0;
}

/**
 *  \brief 退出发送控制参数
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
static void
protocol_gw1376_2_send_control_data_exit(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = &pdata->send;
    memset(psend, 0, sizeof(PROTOCOL_GW1376_2_SEND_DATA));
}

/**
 *  \brief 初始化发送控制参数
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
static int
protocol_gw1376_2_recv_control_data_init(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = &pdata->recv;

    precv->dir = GW1376_2_DIR_UP;
    precv->prm = GW1376_2_PRM_MASTER;
    precv->comm_type = GW1376_2_COMM_TYPE_HPLC_CARRIER;
    precv->route_mode = GW1376_2_ROUTE_MODE;
    precv->subnode = GW1376_2_SUBNODE_NO;
    precv->comm_module_flag = GW1376_2_CCO;
    precv->conflict = GW1376_2_CONFLICT_NO;
    precv->relay_level = GW1376_2_RELAY_LEVEL0;
    precv->channel = GW1376_2_CHANNEL0;
    precv->coding = GW1376_2_CODING_NO;
    precv->bytes_count = GW1376_2_BYTES_DEFAULT;
    precv->comm_baud = 0;
    precv->comm_baud_flag = GW1376_2_BAUD_BIT;

    precv->res_data_head = CreateList();
    return 0;
}

/**
 *  \brief 退出发送控制参数
 *  \param BYTE *buf
 *  \param int *len
 *  \param void
 *  \return bool
 */
static void
protocol_gw1376_2_recv_control_data_exit(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = &pdata->recv;
    memset(precv, 0, sizeof(PROTOCOL_GW1376_2_RECV_DATA));
}

int protocol_gw1376_2_data_init(void *p)
{
    PROTOCOL_GW1376_2_DATA *pprotocol_data = (PROTOCOL_GW1376_2_DATA *)p;
    protocol_gw1376_2_send_control_data_init(pprotocol_data);
    protocol_gw1376_2_recv_control_data_init(pprotocol_data);
    gw13762_task_init(&pprotocol_data->task);

    return 0;
}

void protocol_gw1376_2_data_exit(void *p)
{

    PROTOCOL_GW1376_2_DATA *pprotocol_data = (PROTOCOL_GW1376_2_DATA *)p;

    if (NULL != pprotocol_data)
    {
        gw13762_task_destroy(&pprotocol_data->task);
        protocol_gw1376_2_send_control_data_exit(pprotocol_data);
        protocol_gw1376_2_recv_control_data_exit(pprotocol_data);

        free(pprotocol_data);
        pprotocol_data = NULL;
    }
}
