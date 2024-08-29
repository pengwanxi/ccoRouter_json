#include "protocol_gw1376_2_AFN.h"
#include "protocol_gw1376_2_public.h"
#include "protocol_gw1376_2_sta.h"
#include "gw13762_task.h"
#include "public.h"
#include "maths.h"

/**
 *  \brief 下行CCO 无单元数据
 *  \param pdata
 *  \param AFN
 *  \param FN
 *  \return 0
 */
static int protoco_gw1376_2_cco_down_no_unit(PROTOCOL_GW1376_2_DATA *pdata,
                                             unsigned char AFN, unsigned Fn)
{
    /* PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata); */

    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);

    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = AFN;
    papply_region->Fn = Fn;
    papply_region->unit_len = 0;

    return 0;
}

/**
 *  \brief 下行CCO 无单元数据
 *  \param pdata
 *  \param AFN
 *  \param FN
 *  \return 0
 */
static int
protoco_gw1376_2_cco_down_unit(PROTOCOL_GW1376_2_DATA *pdata,
                               PROTOCOL_GW1376_2_APPLY_REGION *pregin)
{
    /* PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata); */

    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);

    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = pregin->AFN;
    papply_region->Fn = pregin->Fn;
    papply_region->unit_len = 0;
    if (BETWEEN(pregin->unit_len, 1, PROTOCOL_GW1376_2_APPLY_DATA_LEN))
    {
        papply_region->unit_len = pregin->unit_len;
        memcpy(papply_region->unit_buf, pregin->unit_buf, pregin->unit_len);
    }

    return 0;
}

int protocol_gw1376_AFN00_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata); */
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    if (pdata->recv.apply_region.AFN != 0x00)
    {
        dzlog_notice("%s AFN=%d error", __FUNCTION__,
                     pdata->recv.apply_region.AFN);
        return -1;
    }

    if (papply_region->Fn == 0x01)
    {
        dzlog_debug("%s ok", __FUNCTION__);
        return 0;
    }

    dzlog_notice("%s error", __FUNCTION__);
    return -1;
}

int protocol_gw1376_AFN01_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    return protoco_gw1376_2_cco_down_no_unit(pdata, 0x01, 0x01);
}

int protocol_gw1376_AFN01_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    return protoco_gw1376_2_cco_down_no_unit(pdata, 0x01, 0x02);
}

int protocol_gw1376_AFN01_Fn03_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    return protoco_gw1376_2_cco_down_no_unit(pdata, 0x01, 0x03);
}

int protocol_gw1376_AFN01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn = protocol_gw1376_AFN00_up(pdata);

    if (rtn < 0)
    {
        return -1;
    }

    if (pdata->type == GW1376_2_DATA_TYPE_HARD_INIT)
    {
        pdata->type = GW1376_2_DATA_TYPE_PARAM_INIT;
    }
    else if (pdata->type == GW1376_2_DATA_TYPE_PARAM_INIT)
    {
        pdata->type = GW1376_2_DATA_TYPE_DATA_INIT;
        /* if (comm_data_get_state(pcomm_data) == COMM_STATE_STARTUP) { */
        // comm_data_set_step(pcomm_data,GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE);

        /* } */
    }
    else if (pdata->type == GW1376_2_DATA_TYPE_DATA_INIT)
    {
        pdata->type = GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR;
    }

    dzlog_debug("switch new type=%d", pdata->type);

    return 0;
}

int protocol_gw1376_AFN02_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);

    protocol_gw1376_2_sta_get_protocol_frame(papply_region->unit_buf,
                                             &papply_region->unit_len, pdata);

    papply_region->AFN = 0x02;
    papply_region->Fn = 0x01;

    if (papply_region->unit_len > 2)
    {
        if (NULL != psend->ptask_data)
        {
            if (psend->ptask_data->isbroadcast)
            {
                protocol_gw1376_2_data_set_data_type(
                    pdata, GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA);
            }
        }
        return 0;
    }

    return -1;
}

int protocol_gw1376_AFN02_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata); */
    int rtn;
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    if (papply_region->AFN != 02 || papply_region->Fn != 01)
    {
        dzlog_notice("%s AFN=%d Fn=%d error", __FUNCTION__, papply_region->AFN,
                     papply_region->Fn);
        return -1;
    }

    if (papply_region->unit_len < 2)
    {
        dzlog_notice("%s unit_len=%d error", __FUNCTION__,
                     papply_region->unit_len);
        return -1;
    }

    if ((u08_t)papply_region->unit_buf[1] < 3)
    {
        dzlog_notice("%s frame_buf_len=%d error", __FUNCTION__,
                     papply_region->unit_buf[1]);
        return -1;
    }

    rtn = protocol_gw1376_2_sta_process_protocol_frame(
        papply_region->unit_buf, papply_region->unit_len, pdata);

    return rtn;
}

int protocol_gw1376_AFN03_Fn04_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    return protoco_gw1376_2_cco_down_no_unit(pdata, 0x03, 0x04);
}

int protocol_gw1376_AFN03_Fn04_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    unsigned char addr[6];

    if (precv->apply_region.AFN != 3 || precv->apply_region.Fn != 4)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        return -1;
    }

    // 地址转换  修改
    public_stob((char *)addr, addr, 6);

    gw13762_task_set_queue_init(&pdata->task);

    /*
        if (0 != memcmp(addr, precv->apply_region.unit_buf, 6))
        {
            gw13762_task_set_queue_init(&pdata->task);
        }
        else
        {
            protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_NULL);
        }
    */
    return 0;
}

int protocol_gw1376_AFN03_Fn10_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    return protoco_gw1376_2_cco_down_no_unit(pdata, 0x03, 10);
}

int protocol_gw1376_AFN03_Fn10_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata); */
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);
    unsigned char addr[6];

    if (papply_region->AFN != 3 || papply_region->Fn != 10)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        return -1;
    }

    if (papply_region->unit_len < 20)
    {
        dzlog_notice("%s unit_len error", __FUNCTION__);
        return -1;
    }

    /* 判断通信方式 */
    if ((papply_region->unit_buf[0] & 0x0f) != 2 &&
        (papply_region->unit_buf[0] & 0x0f) != 4)
    {
        dzlog_notice("%s comm_flag != 2 && 4", __FUNCTION__);
        return -1;
    }

    if (BIT_GET(papply_region->unit_buf[0], 4))
    {
        protocol_gw1376_2_data_set_send_route_mode(pdata, GW1376_2_ROUTE_MODE);
    }
    else
    {
        protocol_gw1376_2_data_set_send_route_mode(pdata,
                                                   GW1376_2_ROUTE_MODE_NO);
    }

    // 地址转换  修改
    public_stob((char *)addr, addr, 6);

    /*
    if (0 != memcmp(addr, &papply_region->unit_buf[14], 6))
    {
        dzlog_notice("addr not equal");
        //comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR);
    }
    else
    {
        dzlog_notice("addr equal");
        comm_data_set_step(pcomm_data,
                           GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM);
    }*/

    return 0;
}

int protocol_gw1376_AFN05_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata); */
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = 0x05;
    papply_region->Fn = 0x01;
    // 设置主节点地址  修改
    public_stob(NULL, (unsigned char *)papply_region->unit_buf, 6);
    papply_region->unit_len = 6;

    return 0;
}

int protocol_gw1376_AFN05_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn = protocol_gw1376_AFN00_up(pdata);
    if (rtn < 0)
    {
        return -1;
    }

    /*
    comm_data_set_step(&pdata->pcomm->data,
                       GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM);*/
    protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_NULL);
    return 0;
}

int protocol_gw1376_AFN05_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);
    /* APP_DATA *papp_data = app_data_get(); */
    /* APP_CFG *pcfg = &papp_data->cfg; */

    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = 0x05;
    papply_region->Fn = 0x02;

    papply_region->unit_buf[0] =
        psend->subnode_auto_up; /* 禁止子节点主动上报 */
    papply_region->unit_len = 1;

    return 0;
}

int protocol_gw1376_AFN05_Fn02_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn = protocol_gw1376_AFN00_up(pdata);
    if (rtn < 0)
    {
        return -1;
    }

    pdata->send.subnode_auto_up_enable = false;
    protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_NULL);
    return 0;
}

int protocol_gw1376_AFN05_Fn03_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);
    /* APP_DATA *papp_data = app_data_get(); */
    /* APP_CFG *pcfg = &papp_data->cfg; */

    protocol_gw1376_2_sta_get_protocol_frame(papply_region->unit_buf,
                                             &papply_region->unit_len, pdata);

    papply_region->AFN = 0x05;
    papply_region->Fn = 0x03;

    if (papply_region->unit_len > 2)
    {
        protocol_gw1376_2_data_set_data_type(
            pdata, GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA);
        return 0;
    }

    return -1;
}

int protocol_gw1376_AFN05_Fn03_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn = protocol_gw1376_AFN00_up(pdata);
    if (rtn < 0)
    {
        /* 统一等上10秒，以保证广播成功 */
        sleep(10);
        return -1;
    }

    sleep(10);
    /* pdata->isbroadcast = false; */
    /* COMM_DATA *pcomm_data = &pdata->pcomm->data; */
    /* int state = comm_data_get_state(pcomm_data); */
    /* if (state == COMM_STATE_TOPOLOGICAL) { */
    /*     TOPOLOGICAL_DATA *ptopological_data = app_data_get_topological_data(); */

    /*     topological_set_state_by_step(ptopological_data, (void *)pcomm_data); */
    /* } */

    return 0;
}

int protocol_gw1376_AFN05_Fn04_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata); */
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);
    /* APP_DATA *papp_data = app_data_get(); */
    /* APP_CFG *pcfg = &papp_data->cfg; */

    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = 0x05;
    papply_region->Fn = 0x04;

    static char s_timeout = 0;
    papply_region->unit_buf[0] =
        PROTOCOL_GW1376_2_SUBNODE_TIMEOUT + s_timeout++;
    papply_region->unit_len = 1;

    return 0;
}

int protocol_gw1376_AFN05_Fn04_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn = protocol_gw1376_AFN00_up(pdata);
    if (rtn < 0)
    {
        return -1;
    }

    protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_NULL);

    return 0;
}

int protocol_gw1376_AFN06_Fn05_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);
    /* APP_DATA *papp_data = app_data_get(); */
    /* APP_CFG *pcfg = &papp_data->cfg; */

    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = 0x00;
    papply_region->Fn = 0x01;

    memset(papply_region->unit_buf, 0, 6);
    papply_region->unit_buf[0] = 01;
    papply_region->unit_len = 6;
    return 0;
}

int protocol_gw1376_AFN06_Fn05_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn;
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    if (papply_region->AFN != 06 || papply_region->Fn != 05)
    {
        dzlog_notice("%s AFN=%d Fn=%d error", __FUNCTION__, papply_region->AFN,
                     papply_region->Fn);
        return -1;
    }

    if (papply_region->unit_len < 2)
    {
        dzlog_notice("%s unit_len=%d error", __FUNCTION__,
                     papply_region->unit_len);
        return -1;
    }

    if ((u08_t)papply_region->unit_buf[2] < 3)
    {
        dzlog_notice("%s frame_buf_len=%d error", __FUNCTION__,
                     papply_region->unit_buf[1]);
        return -1;
    }

    /* rtn = protocol_gw1376_2_sta_process_protocol_frame( */
    /*     papply_region->unit_buf, papply_region->unit_len, pdata); */
    switch (papply_region->unit_buf[1])
    {
    case 2:
    { /* 645 2007 */
        GW13762_TASK_DATA data;
        memset(&data, 0, sizeof(data));
        data.ptype = 1; // PROTOCOL_DLT645;  修改
        data.type = GW1376_2_DATA_TYPE_AUTOUP;
        data.ismanu_index = true;
        data.index = pdata->buf_count;
        data.len = papply_region->unit_buf[2];
        memcpy(data.buf, &papply_region->unit_buf[3], data.len);
        GW13762_TASK *ptask = &pdata->task;
        gw13762_task_push(ptask, &data);
    }
    break;
    case 4:
    {
        /* int num = papply_region->unit_buf[2] / 7; */
        int num = (papply_region->unit_buf[2] - 1) / 6;
        int i;
        u08_t state = papply_region->unit_buf[3];
        for (i = 0; i < num; i++)
        {
            int start = i * 6;
            char addr[13];
            btos((const char *)(papply_region->unit_buf + 4 + start), addr, 6);
            // DEV_DATA *pdev_data = dev_data_get_by_staaddr(addr);

            GW13762_TASK_DATA data;
            memset(&data, 0, sizeof(data));
            data.ptype = 1; // PROTOCOL_DLT645;  修改
            data.type = GW1376_2_DATA_TYPE_AUTOUP;
            data.ismanu_index = true;
            data.index = pdata->buf_count;
            data.len = 7;
            data.buf[0] = state;
            memcpy(&data.buf[1], &papply_region->unit_buf[4 + start], 6);
            GW13762_TASK *ptask = &pdata->task;
            gw13762_task_push(ptask, &data);
        }
    }
    break;
    default:
        break;
    }

    return 0;
}

int protocol_gw1376_AFN10_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    return protoco_gw1376_2_cco_down_no_unit(pdata, 0x10, 1);
}

int protocol_gw1376_AFN10_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* APP_DATA *papp_data = app_data_get(); */
    // COMM_DATA *pcomm_data = &pdata->pcomm->data;
    /* APP_CFG *pcfg = &papp_data->cfg; */
    /* PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata); */
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);
    /* unsigned char addr[6]; */

    if (papply_region->AFN != 0x10 || papply_region->Fn != 1)
    {
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */

        gw13762_task_set_queue_init(&pdata->task);
        return -1;
    }

    if (papply_region->unit_len != 4)
    {
        dzlog_notice("%s unit_len error", __FUNCTION__);
        gw13762_task_set_queue_init(&pdata->task);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        return -1;
    }

    pdata->subnode_num =
        MAKEWORD(papply_region->unit_buf[0], papply_region->unit_buf[1]);

    return 0;
}

int protocol_gw1376_AFN10_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_APPLY_REGION regin;

    regin.AFN = 0x10;
    regin.Fn = 2;
    regin.unit_len = 3;
    regin.unit_buf[0] = 0;
    regin.unit_buf[1] = 0;
    regin.unit_buf[2] = 0; // 修改

    return protoco_gw1376_2_cco_down_unit(pdata, &regin);
}

int protocol_gw1376_AFN10_Fn02_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* APP_DATA *papp_data = app_data_get(); */
    // COMM_DATA *pcomm_data = &pdata->pcomm->data;
    /* APP_CFG *pcfg = &papp_data->cfg; */
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);
    int i;
    unsigned char addr[6];

    if (papply_region->AFN != 0x10 || papply_region->Fn != 2)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        // gw13762_task_set_queue_init(&pdata->task);
        return -1;
    }

    u16_t total =
        MAKEWORD(papply_region->unit_buf[0], papply_region->unit_buf[1]);

    /* dzlog_debug("%s ok", __FUNCTION__); */
    /* for (i = 0; i < pcomm_data->dev_num; i++) { */
    /*     DEV_DATA *pdev_data = pcomm_data->pdev[i]; */
    /*     if (!pdev_data->addr_check) */
    /*         break; */
    /* } */

    /* if (i == pcomm_data->dev_num) */
    /*     protocol_gw1376_2_data_set_state( */
    /*         pdata, GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT); */

    return 0;
}

int protocol_gw1376_AFN10_Fn04_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_APPLY_REGION regin;

    regin.AFN = 0x10;
    regin.Fn = 4;
    regin.unit_len = 0;

    return protoco_gw1376_2_cco_down_unit(pdata, &regin);
}

int protocol_gw1376_AFN10_Fn04_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* APP_DATA *papp_data = app_data_get(); */
    // COMM_DATA *pcomm_data = &pdata->pcomm->data;
    /* APP_CFG *pcfg = &papp_data->cfg; */
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);
    int i;
    unsigned char addr[6];

    if (papply_region->AFN != 0x10 || papply_region->Fn != 4)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        // gw13762_task_set_queue_init(&pdata->task);
        return -1;
    }

    if (papply_region->unit_len != 16)
    {
        dzlog_notice("%s unit_len error", __FUNCTION__);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        // gw13762_task_set_queue_init(&pdata->task);
        return -1;
    }

    return 0;
}

int protocol_gw1376_AFN10_Fn21_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_APPLY_REGION regin;
    // COMM_DATA *pcomm_data = &pdata->pcomm->data;
    // DEV_DATA *pdev_data = comm_data_get_current_dev(pcomm_data);

    u16_t index = 0; // 修改

    regin.AFN = 0x10;
    regin.Fn = 21;
    regin.unit_len = 3;
    regin.unit_buf[0] = LOBYTE(index);
    regin.unit_buf[1] = HIBYTE(index);
    regin.unit_buf[2] = 1;

    return protoco_gw1376_2_cco_down_unit(pdata, &regin);
}

int protocol_gw1376_AFN10_Fn21_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    /* APP_DATA *papp_data = app_data_get(); */
    // COMM_DATA *pcomm_data = &pdata->pcomm->data;
    /* APP_CFG *pcfg = &papp_data->cfg; */
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);
    int i;
    unsigned char addr[6];
    u08_t base = 5;
    u08_t sulen = 11;

    if (papply_region->AFN != 0x10 || papply_region->Fn != 21)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        return -1;
    }

    if (papply_region->unit_len != ((sulen) + base))
    {
        dzlog_notice("%s unit_len error", __FUNCTION__);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        return -1;
    }

    u08_t num = papply_region->unit_buf[4];
    for (i = 0; i < num; i++)
    {
        int infopos = base + (sulen * i) + sulen - 1;
        u08_t node_info = (precv->apply_region.unit_buf[infopos] & 0xF0) >> 4;
        if (0x01 != (node_info))
        {
            dzlog_notice("%s subnode info = %d %x %x", __FUNCTION__, infopos,
                         precv->apply_region.unit_buf[infopos], node_info);
            continue;
        }
        int j = 0;

        if (0 != memcmp(addr,
                        &precv->apply_region.unit_buf[base + sulen * i],
                        6))
        {
            dzlog_notice("%s subnode addr error", __FUNCTION__);
            continue;
        }
        else
        {
            dzlog_info("%s subnode addr  ok", __FUNCTION__);
        }
    }

    /* dzlog_debug("%s ok", __FUNCTION__); */
    /* for (i = 0; i < pcomm_data->dev_num; i++) { */
    /*     DEV_DATA *pdev_data = pcomm_data->pdev[i]; */
    /*     if (!pdev_data->addr_check) */
    /*         break; */
    /* } */

    /* if (i == pcomm_data->dev_num) */
    /*     protocol_gw1376_2_data_set_state( */
    /*         pdata, GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT); */

    return 0;
}

int protocol_gw1376_AFN10_Fn21_down_all(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_APPLY_REGION regin;

    int valid_dev_num = 0; // 修改

    regin.AFN = 0x10;
    regin.Fn = 21;
    regin.unit_len = 3;
    regin.unit_buf[0] = 0;
    regin.unit_buf[1] = 0;
    regin.unit_buf[2] = valid_dev_num + 1; /* 包函本身地址 */

    return protoco_gw1376_2_cco_down_unit(pdata, &regin);
}

int protocol_gw1376_AFN10_Fn21_up_all(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);
    int i;
    unsigned char addr[6];
    u08_t base = 5;
    u08_t sulen = 11;

    if (papply_region->AFN != 0x10 || papply_region->Fn != 21)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        return -1;
    }

    int valid_dev_num = 0; // 修改
    valid_dev_num += 1;    /* 增加主模块 */
    if (papply_region->unit_len > ((valid_dev_num * sulen) + base))
    {
        dzlog_notice("%s unit_len error", __FUNCTION__);
        /* comm_data_set_step(pcomm_data, GW1376_2_DATA_TYPE_PARAM_INIT); */
        return -1;
    }

    u16_t total =
        MAKEWORD(papply_region->unit_buf[0], papply_region->unit_buf[1]);

    u16_t start = 0;

    u08_t num = papply_region->unit_buf[4];
    for (i = 0; i < num; i++)
    {
        int infopos = base + (sulen * i) + sulen - 1;
        u08_t node_info = (precv->apply_region.unit_buf[infopos] & 0xF0) >> 4;
        if (0x01 != (node_info))
        {
            dzlog_notice("%s subnode info = %d %x %x", __FUNCTION__, infopos,
                         precv->apply_region.unit_buf[infopos], node_info);
            continue;
        }
    }

    return 0;
}

int protocol_gw1376_AFN11_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_APPLY_REGION regin;
    int start = 0;

    regin.AFN = 0x11;
    regin.Fn = 1;

    dzlog_debug("type=%d", protocol_gw1376_2_data_get_data_type(pdata));
    if (protocol_gw1376_2_data_get_data_type(pdata) ==
        GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE)
    {
    }
    return protoco_gw1376_2_cco_down_unit(pdata, &regin);
}

int protocol_gw1376_AFN11_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    int rtn = protocol_gw1376_AFN00_up(pdata);
    if (rtn < 0)
    {
        if (papply_region->unit_buf[0] != 0x06)
        {
            return -1;
        }
    }

    /* protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_NULL); */
    // protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT);

    return 0;
}

int protocol_gw1376_AFN11_Fn03_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_APPLY_REGION regin;
    int i;

    regin.AFN = 0x11;
    regin.Fn = 3;

    regin.unit_len = 7 + 0; //(pdev_data->relay_addrs_num * 6);  修改
    regin.unit_buf[0] = 0;  // pcomm_data->dev_num; 修改

    public_stob(NULL, (unsigned char *)&regin.unit_buf[0], 6); // 修改
    regin.unit_buf[6] = 0;                                     // pdev_data->relay_addrs_num;  修改

    for (i = 0; i < 2; i++) // 2 修改
    {
        // dzlog_debug("relay_addr[i]=%s", pdev_data->relay_addrs[i]);
        public_stob(NULL,
                    (unsigned char *)&regin.unit_buf[7 + 6 * i], 6);
    }

    return protoco_gw1376_2_cco_down_unit(pdata, &regin);
}

int protocol_gw1376_AFN11_Fn03_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn = protocol_gw1376_AFN00_up(pdata);
    if (rtn < 0)
    {
        return -1;
    }

    //dev_data_set_state(pdev_data, DEV_STATE_NULL);
    protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_NULL);
    return 0;
}
