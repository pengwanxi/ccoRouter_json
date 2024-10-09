#include "protocol_gw1376_2_public.h"
#include "maths.h"
#include "public.h"
#include "protocol_gw1376_2_AFN.h"
#include "gw13762_task.h"
#include "zlog.h"
#include "Dlist.h"

unsigned char protocol_gw1376_2_cal_fn(unsigned char DT1, unsigned char DT2)
{
    int i;

    for (i = 7; i >= 0; i--)
    {
        if (BIT_GET(DT1, i))
        {
            return (DT2 * 8) + i + 1;
        }
    }

    return 0;
}

int protocol_gw1376_2_dec_fn(unsigned char fn, unsigned char *DT1,
                             unsigned char *DT2)
{
    *DT1 = 1 << ((fn - 1) % 8);
    *DT2 = (fn - 1) / 8;
    return 0;
}

int protocol_gw1376_2_get_data_type_by_msg_state(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* MSG_DATA *pmsg = app_msg_data_get(); */
    /* COMM_DATA *pcomm_data = &pdata->pcomm->data; */
    /* MSG_PROC_DATA *pmsg_data = comm_msg_get(pdata->pcomm); */

    /* int state = comm_data_state_get(pcomm_data); */
    /* switch (state) { */
    /* case COMM_STATE_TOPOLOGICAL: { */
    /*     return GW1376_2_DATA_TYPE_TRANS_DATA; */
    /* } break; */

    /* default: */
    /*     break; */
    /* } */

    return GW1376_2_DATA_TYPE_NULL;
}

int protocol_gw1376_2_get_data_type_by_comm_state(PROTOCOL_GW1376_2_DATA *pdata)
{
}

int protocol_gw1376_2_get_data_type_by_state(PROTOCOL_GW1376_2_DATA *pdata)
{
    int type = protocol_gw1376_2_data_get_state(pdata);
    dzlog_debug("%s type=%d", __FUNCTION__, type);

    return type;
}

int protocol_gw1376_2_get_data_type_by_dev_state(PROTOCOL_GW1376_2_DATA *pdata)
{

    int type = GW1376_2_DATA_TYPE_NULL;

    return type;
}

int protocol_gw1376_2_get_data_type_by_common(PROTOCOL_GW1376_2_DATA *pdata)
{
    return GW1376_2_DATA_TYPE_TRANS_DATA;
}

int protocol_gw1376_2_get_data_type(PROTOCOL_GW1376_2_DATA *pdata)
{
    int type = 0;
    /*
    if (pdata->issending)
    {
        dzlog_info("waiting recv");
        pdata->sending_time +=
            (pdata->pcomm->data.interval + UART_RECV_TIMEOUT);
        if (pdata->sending_time >=
            (PROTOCOL_GW1376_2_SENGDING_TIMEOUT * 1000))
        {
            pdata->issending = false;
        }
        comm_data_set_next_dev_index(&pdata->pcomm->data,
                                     pdata->psend_dev->index);
        return 0;
    }

    DEV_DATA *pdev_data = comm_data_get_current_dev(&pdata->pcomm->data);
    if (BETWEEN(pdev_data->send_num, 2, DEV_SEND_UNNORAML_NUM))
    {
        dzlog_warn("resend=%d", pdev_data->send_num);

        pdata->isresend = 1;
        type = pdata->resend_type;
        return type;
    }
    */
    /* 消息 */
    type = protocol_gw1376_2_get_data_type_by_msg_state(pdata);
    if (GW1376_2_DATA_TYPE_NULL != type)
    {
        dzlog_debug("msg_type=%d", type);
        return type;
    }

    if (pdata->send.subnode_auto_up_enable)
    {
        type = GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP;
        pdata->send.subnode_auto_up = false;
        pdata->send.subnode_auto_up_enable = false;
        return type;
    }

    /* 特殊 */
    /* type = protocol_gw1376_2_get_data_type_by_comm_state(pdata); */
    /* if (GW1376_2_DATA_TYPE_NULL != type) { */
    /*     dzlog_debug("comm_state_type=%d", type); */
    /*     return type; */
    /* } */

    type = protocol_gw1376_2_get_data_type_by_state(pdata);
    if (GW1376_2_DATA_TYPE_NULL != type)
    {
        dzlog_debug("state_type=%d", type);
        return type;
    }

    type = protocol_gw1376_2_get_data_type_by_dev_state(pdata);
    if (GW1376_2_DATA_TYPE_NULL != type)
    {
        dzlog_debug("dev_state_type=%d", type);
        return type;
    }

    /* 正常循环 */
    return protocol_gw1376_2_get_data_type_by_common(pdata);
}

int protocol_gw1376_2_pos_valid_buf(char *buf, int len,
                                    PROTOCOL_GW1376_2_DATA *pdata, int pos)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);

    while (len - pos > 0)
    {
        if (buf[pos] == 0x68)
        {
            int tmp_len = MAKEWORD(buf[pos + 1], buf[pos + 2]);
            if (tmp_len > (len - pos))
            {
                pos++;
                continue;
            }
            if (buf[pos + tmp_len - 1] == 0x16)
            {
                precv->value_len = tmp_len;
                return pos;
            }
        }
        pos++;
    }

    return -1;
}

int protocol_gw1376_2_check_buf(char *buf, int len,
                                PROTOCOL_GW1376_2_DATA *pdata)
{
    /* 输入 */
    if (NULL == buf || len < 11)
    {
        dzlog_notice("%s input error ", __FUNCTION__);
        return -1;
    }

    /* 头 */
    if (0x68 != buf[0])
    {
        dzlog_notice("%s head error ", __FUNCTION__);
        return -1;
    }

    /* 判断长度 */
    if (len != MAKEWORD(buf[1], buf[2]))
    {
        dzlog_notice("%s len error ", __FUNCTION__);
        return -1;
    }

    /* 判断地址 */

    /* 判断cs */
    unsigned char cs = cal_cs((unsigned char *)buf, len - 2);
    if (cs == buf[len - 2])
    {
        dzlog_notice("%s cs %.2x!=%.2x error ", __FUNCTION__, cs, buf[len - 2]);
        return -1;
    }

    /* 判断结束 */
    if (buf[len - 1] != 0x16)
    {
        dzlog_notice("%s end error ", __FUNCTION__);
        return -1;
    }

    return 0;
}

int protocol_gw1376_2_find_valid_buf(char *buf, int len,
                                     PROTOCOL_GW1376_2_DATA *pdata, int pos)
{
    pos = protocol_gw1376_2_pos_valid_buf(buf, len, pdata, pos);
    dzlog_info("pos : [ %d ]", pos);
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    if (pos < 0 || pos > len)
    {
        dzlog_notice("%s pos error", __FUNCTION__);
        return -1;
    }

    dzlog_debug("%s len=%.3d:", __FUNCTION__, precv->value_len);
    log_debug_frame(buf + pos, precv->value_len);

    if (protocol_gw1376_2_check_buf(buf + pos, precv->value_len, pdata) < 0)
    {
        dzlog_notice("%s check error", __FUNCTION__);
        return -1;
    }

    return pos;
}

/**
 *  \brief 打包控制域
 *  \param pdata
 *  \return 0xff 失败
 *  \return 其它 成功
 */
char protocol_gw1376_pack_ctrl_data(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);

    char ctrl = psend->comm_type & 0x3f;

    if (psend->dir)
        BIT_SET(ctrl, 7);

    if (psend->prm)
        BIT_SET(ctrl, 6);

    return ctrl;
}

/**
 *  \brief 解包控制域
 *  \param pdata
 *  \return 0xff 失败
 *  \return 其它 成功
 */
char protocol_gw1376_unpack_ctrl_data(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);

    precv->comm_type = precv->ctrl_region & 0x3f;

    precv->dir = BIT_GET(precv->ctrl_region, 7);
    precv->prm = BIT_GET(precv->ctrl_region, 6);

    dzlog_debug("%s : [comm_type=%d , dir=%d , prm=%d] ", __FUNCTION__, precv->comm_type, precv->dir,
                precv->prm);
    return 0;
}

/**
 *  \brief 打包信息域
 *  \param pinfo
 *  \param pdata
 *  \return 0 成功
 *  \return <0 失败
 */
int protocol_gw1376_pack_user_info_region(PROTOCOL_GW1376_2_INFO_REGION *pinfo,
                                          PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    unsigned short baud;

    if (NULL == pinfo)
        return -1;

    memset(pinfo, 0, sizeof(PROTOCOL_GW1376_2_INFO_REGION));

    /* data0 */
    if (psend->route_mode)
        BIT_SET(pinfo->data[0], 0);

    if (psend->subnode)
        BIT_SET(pinfo->data[0], 1);

    if (psend->comm_module_flag)
        BIT_SET(pinfo->data[0], 2);

    if (psend->conflict)
        BIT_SET(pinfo->data[0], 3);

    pinfo->data[0] |= (psend->relay_level << 4);

    /* data1 */
    pinfo->data[1] = (psend->coding << 4) | psend->channel;

    /* data2 */
    pinfo->data[2] = psend->bytes_count;

    /* data3-4 */
    baud = psend->comm_baud;
    if (psend->comm_baud_flag)
        BIT_SET(baud, 15);
    pinfo->data[3] = LOBYTE(baud);
    pinfo->data[4] = HIBYTE(baud);

    /* data5 */
    if (NULL != psend->ptask_data)
    {
        /* psend->ptask_data->index = pdata->buf_count; */

        /* if (psend->ptask_data->timeout_sec) { */
        /*     struct timeval time_val; */
        /*     if (gettimeofday(&time_val, NULL)) { */
        /*         return -1; */
        /*     } */
        /*     psend->ptask_data->record_sec = time_val.tv_sec; */
        /*     printf("record_sec=%lu %p\n", psend->ptask_data->record_sec, */
        /*            psend->ptask_data); */
        /* } */
        pinfo->data[5] = psend->ptask_data->index;
    }
    else
    {
        MOD_INC(pdata->buf_count, 0xff);
        pinfo->data[5] = pdata->buf_count;
    }

    return 0;
}

/**
 *  \brief 解包信息域
 *  \param pinfo
 *  \param pdata
 *  \return 0 成功
 *  \return <0 失败
 */
int protocol_gw1376_unpack_user_info_region(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_INFO_REGION *pinfo = &precv->info_region;
    /* DEV_DATA *pdev_data = comm_data_get_current_dev(&pdata->pcomm->data); */
    unsigned short baud;

    /* data0 */
    precv->route_mode = BIT_GET(pinfo->data[0], 0);
    // dzlog_notice("rounte_mode=%d", precv->route_mode);

    precv->subnode = BIT_GET(pinfo->data[0], 1);
    // dzlog_notice("subnode=%d", precv->subnode);

    precv->comm_module_flag = BIT_GET(pinfo->data[0], 2);
    // dzlog_notice("comm_module_flag=%d", precv->comm_module_flag);

    precv->conflict = BIT_GET(pinfo->data[0], 3);
    // dzlog_notice("conflict=%d", precv->conflict);

    precv->relay_level = (pinfo->data[0] >> 4);
    // dzlog_notice("relay_level=%d", precv->relay_level);
    /* if (precv->comm_module_flag == GW1376_2_STA) { */
    /*     if (precv->relay_level != pdev_data->relay_addrs_num) { */
    /*         dzlog_notice("warn:recv relay_level=%d config relay_level=%d", */
    /*                      precv->relay_level, pdev_data->relay_addrs_num); */
    /*     } */
    /* } */
    /* data1 */
    precv->channel = pinfo->data[1] & 0x0f;
    // dzlog_debug("channel=%d", precv->channel);

    precv->coding = pinfo->data[1] >> 4;
    // dzlog_debug("coding=%d", precv->coding);

    /* data2 */
    precv->bytes_count = pinfo->data[2];
    // dzlog_debug("bytes_count=%d", precv->bytes_count);

    /* data3-4 */
    baud = MAKEWORD(pinfo->data[3], pinfo->data[4]);
    // dzlog_debug("baud=%d", precv->comm_baud);

    precv->comm_baud_flag = BIT_GET(baud, 15);
    // dzlog_debug("baud_flag=%d", precv->comm_baud_flag);

    /* data5 */
    int buf_count = (unsigned char)pinfo->data[5];
    // dzlog_debug("buf_count=%d", (unsigned char)pinfo->data[5]);
    if (!precv->prm)
    {
        // GW13762_TASK_DATA *ptask_data = gw13762_task_find(&pdata->task, buf_count);
        precv->recvIndex = buf_count;
    }
    else
    {
        pdata->buf_count = buf_count;
        dzlog_notice("%s data buf_count=%d auto up",
                     __FUNCTION__, pdata->buf_count);
    }

    dzlog_info("rounte_mode = %d , subnode = %d ,comm_module_flag = %d ,conflict = %d ,relay_level = %d , channel = %d ,coding = %d , bytes_count = %d , baud = %d , baud_flag = %d ,buf_count = %d",
               precv->route_mode, precv->subnode, precv->comm_module_flag, precv->conflict,
               precv->relay_level, precv->channel, precv->coding, precv->bytes_count, precv->comm_baud, precv->comm_baud_flag, pinfo->data[5]);
    /* if ((u08_t)pinfo->data[5] != pdata->buf_count) { */
    /*     /\* pdata->send.subnode_auto_up = true; *\/ */
    /*     /\* protocol_gw1376_2_data_set_data_type( *\/ */
    /*     /\* 	pdata, GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP); *\/ */

    /*     dzlog_notice("%s error data buf_count=%d", __FUNCTION__, */
    /*                  pdata->buf_count); */
    /*     return -1; */
    /* } */

    return 0;
}

/**
 *  \brief 打包地址域
 *  \param pinfo
 *  \param pdata
 *  \return 0 成功
 *  \return <0 失败
 */
int protocol_gw1376_pack_user_addr_region(PROTOCOL_GW1376_2_ADDR_REGION *paddr,
                                          PROTOCOL_GW1376_2_DATA *pdata)
{

    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    /*
APP_DATA *papp_data = app_data_get();
APP_CFG *pcfg = &papp_data->cfg;
int i;
DEV_DATA *pdev_data = comm_data_get_current_dev(&pdata->pcomm->data);

if (NULL == paddr)
    return -1;

public_stob(pcfg->addr, (unsigned char *)paddr->src, 6);

for (i = 0; i < pdev_data->relay_addrs_num; i++)
{
    dzlog_debug("relay_addr=%s", pdev_data->relay_addrs[i]);
    public_stob(pdev_data->relay_addrs[i], (unsigned char *)paddr->relay[i],
                6);

    log_debug_frame(paddr->relay[i], 6);
}
protocol_gw1376_2_data_set_send_relay_level(pdata,
                                            pdev_data->relay_addrs_num);
dzlog_debug("%s %d", __FUNCTION__, psend->info_region.data[0]);
paddr->relay_num = pdev_data->relay_addrs_num;

public_stob(pdev_data->s_staaddr, (unsigned char *)paddr->dest, 6);
*/
    // 修改
    return 0;
}

/**
 *  \brief 解包地址域
 *  \param pinfo
 *  \param pdata
 *  \return 0 成功
 *  \return <0 失败
 */
int protocol_gw1376_unpack_user_addr_region(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    /* APP_DATA *papp_data = app_data_get(); */
    /* APP_CFG *pcfg = &papp_data->cfg; */
    int index = 6;
    /* DEV_DATA *pdev_data = comm_data_get_current_dev(&pdata->pcomm->data); */
    PROTOCOL_GW1376_2_ADDR_REGION *paddr = &precv->addr_region;

    if (NULL == paddr)
        return -1;

    memcpy(paddr->src, pdata->user_data_buf + index, 6);
    dzlog_debug("src addr=");
    log_debug_frame(paddr->src, 6);

    if (pdata->isbroadcast)
    {
        int j;
        for (j = 0; j < 6; j++)
        {
            if ((unsigned char)paddr->src[j] != 0x99)
            {
                break;
            }
        }

        dzlog_debug("broadcast j=%d", j);
        if (j == 6)
        {
            /* COMM_DATA *pcomm_data = &pdata->pcomm->data; */
            /* TOPOLOGICAL_DATA *ptopological_data = */
            /*     app_data_get_topological_data(); */
            /* topological_set_state_by_step(ptopological_data, */
            /*                               (void *)pcomm_data); */
            /* dzlog_debug("broadcast switch"); */
            pdata->isbroadcast = false;
        }
    }

    index += 6;

    paddr->relay_num = precv->relay_level;

    memcpy(paddr->dest, pdata->user_data_buf + index, 6);
    dzlog_debug("dest addr=");
    log_debug_frame(paddr->dest, 6);
    index += 6;

    return index;
}

/**
 *  \brief 打包应用域
 *  \param pinfo
 *  \param pdata
 *  \return 0 成功
 *  \return <0 失败
 */
static int protocol_gw1376_pack_user_apply_region(
    PROTOCOL_GW1376_2_APPLY_REGION *papplydata, PROTOCOL_GW1376_2_DATA *pdata)
{
    unsigned char DT1, DT2;
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    int index = 0;

    protocol_gw1376_2_dec_fn(papplydata->Fn, &DT1, &DT2);

    psend->apply_data[index++] = papplydata->AFN;

    psend->apply_data[index++] = DT1;
    psend->apply_data[index++] = DT2;

    if (papplydata->unit_len > 0)
    {
        memcpy(&psend->apply_data[index], papplydata->unit_buf,
               papplydata->unit_len);

        index += papplydata->unit_len;
    }

    psend->apply_data_len = index;
    return 0;
}

/**
 *  \brief 解包应用域
 *  \param pinfo
 *  \param pdata
 *  \return 0 成功
 *  \return <0 失败
 */
static int
protocol_gw1376_unpack_user_apply_region(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    int index = 0;
    PROTOCOL_GW1376_2_APPLY_REGION *papplydata = &precv->apply_region;

    papplydata->AFN = precv->apply_data[index];
    index++;
    // dzlog_debug("AFN=%x", papplydata->AFN);

    papplydata->Fn = protocol_gw1376_2_cal_fn(precv->apply_data[index],
                                              precv->apply_data[index + 1]);
    index += 2;
    // dzlog_debug("Fn=F%x", papplydata->Fn);

    papplydata->unit_len = precv->apply_data_len - index;
    if (papplydata->unit_len > 0)
    {
        memcpy(papplydata->unit_buf, &precv->apply_data[index],
               papplydata->unit_len);

        index += papplydata->unit_len;
    }

    // dzlog_debug("unit_len=%d unit_buf:", papplydata->unit_len);
    // log_debug_frame(papplydata->unit_buf, papplydata->unit_len);
    dzlog_info("%s : [ AFN = %x , Fn = F%x , unit_len=%d ]", __FUNCTION__, papplydata->AFN, papplydata->Fn, papplydata->unit_len);
    return index;
}

/**
 *  \brief 打包数据
 *  \param buf 用户数据
 *  \param &len 用户数据长度
 *  \return >0 数据长度
 *  \return <=0 失败
 */
int protocol_gw1376_pack_user_data(char *buf, int *len,
                                   PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    int index = 0;

    if (psend->comm_module_flag == GW1376_2_STA)
        protocol_gw1376_pack_user_addr_region(&psend->addr_region, pdata); // 操作从节点要写地址

    protocol_gw1376_pack_user_info_region(&psend->info_region, pdata);
    memcpy(buf, psend->info_region.data, 6);
    index += 6;

    if (psend->comm_module_flag == GW1376_2_STA) // 从节点中继地址
    {
        int i;
        memcpy(buf + index, psend->addr_region.src, 6);
        index += 6;

        // 中继地址 修改
        /*
        for (i = 0; i < psend->addr_region.relay_num; i++)
        {
            memcpy(buf + index, psend->addr_region.relay[i], 6);
            index += 6;
        }   */

        memcpy(buf + index, psend->addr_region.dest, 6);
        index += 6;
    }

    protocol_gw1376_pack_user_apply_region(&psend->apply_region, pdata);
    memcpy(buf + index, psend->apply_data, psend->apply_data_len);
    index += psend->apply_data_len;

    *len = index;
    // dzlog_debug("%s %d", __FUNCTION__, psend->info_region.data[0]);
    return index;
}

/**
 *  \brief 打包数据
 *  \param buf 用户数据
 *  \param &len 用户数据长度
 *  \return >0 数据长度
 *  \return <=0 失败
 */
int protocol_gw1376_unpack_user_data(char *buf, int len,
                                     PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    int index = 0;

    // dzlog_debug("parse user_data :");
    // log_debug_frame(buf, len);

    memcpy(precv->info_region.data, buf + index, 6);
    if (protocol_gw1376_unpack_user_info_region(pdata) < 0)
    {
        dzlog_notice("protocol_gw1376_unpack_user_info_region error");
        return -1;
    }
    index += 6;

    if (precv->comm_module_flag == GW1376_2_STA)
    {
        index = protocol_gw1376_unpack_user_addr_region(pdata);
    }

    precv->apply_data_len = len - index;
    memcpy(precv->apply_data, buf + index, precv->apply_data_len);
    protocol_gw1376_unpack_user_apply_region(pdata);

    return pdata->user_data_len;
}

int protocol_gw1376_pack_frame_data(char *buf, int *len,
                                    PROTOCOL_GW1376_2_DATA *pdata)
{
    int index = 0;
    unsigned short data_len;
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    GW13762_TASK_DATA *ptdata = pdata->send.ptask_data;

    protocol_gw1376_pack_user_data(pdata->user_data_buf, &pdata->user_data_len,
                                   pdata);

    buf[index++] = 0x68;

    data_len = 6 + pdata->user_data_len;
    buf[index++] = LOBYTE(data_len);
    buf[index++] = HIBYTE(data_len);

    if (ptdata->type == GW1376_2_DATA_TYPE_AUTOUP)
    {
        protocol_gw1376_2_data_set_send_prm(pdata, GW1376_2_PRM_SLAVE);
    }
    else
    {
        protocol_gw1376_2_data_set_send_prm(pdata, GW1376_2_PRM_MASTER);
    }
    buf[index++] = psend->ctrl_region;

    memcpy(&buf[index], psend->info_region.data, 6);
    memcpy(&buf[index], pdata->user_data_buf, pdata->user_data_len);
    index += pdata->user_data_len;

    buf[index] = cal_cs((unsigned char *)(buf + 3), index - 3);
    index++;

    buf[index++] = 0x16;
    *len = index;

    dzlog_debug("%s %d", __FUNCTION__, psend->info_region.data[0]);
    pdata->issending = true;
    pdata->sending_time = 0;
    // pdata->psend_dev = comm_data_get_current_dev(&pdata->pcomm->data);

    pdata->resend_len = *len;
    memcpy(pdata->resend_buf, buf, pdata->resend_len);
    pdata->resend_type = pdata->type;

    return index;
}

int protocol_gw1376_unpack_frame_data(char *buf, int len,
                                      PROTOCOL_GW1376_2_DATA *pdata)
{
    int index;
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);

    precv->frame_len = MAKEWORD(buf[1], buf[2]);

    precv->ctrl_region = buf[3];
    protocol_gw1376_unpack_ctrl_data(pdata);
    if (!precv->prm)
    {
        pdata->issending = false;
    }
    else
    {
        /* 不处理prm 的报文 */
        pdata->issending = false;
        /* return 0; */
    }

    index = 4;
    pdata->user_data_len = precv->frame_len - index - 2;
    memcpy(pdata->user_data_buf, &buf[index], pdata->user_data_len);

    if (protocol_gw1376_unpack_user_data(pdata->user_data_buf,
                                         pdata->user_data_len, pdata) < 0)
    {
        dzlog_notice("%s unpack error", __FUNCTION__);
        return -1;
    }

    return precv->frame_len;
}

int protocol_gw1376_res_ListAddNode(PROTOCOL_GW1376_2_DATA *pdata, void *Info, int infoSize)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    pthread_mutex_lock(&precv->resHeadLock);
    ListAddNode(precv->res_data_head, Info, infoSize);
    pthread_cond_signal(&precv->resHeadCond);
    pthread_mutex_unlock(&precv->resHeadLock);
    return 0;
}
