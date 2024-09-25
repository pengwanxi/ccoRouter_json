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
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    AFN00_INFO afnInfo = {0};
    if (precv->apply_region.AFN != 0x00)
    {
        dzlog_notice("%s AFN=%d error", __FUNCTION__,
                     pdata->recv.apply_region.AFN);
        return -1;
    }
    afnInfo.AFN = precv->apply_region.AFN;
    afnInfo.Fn = precv->apply_region.Fn;
    afnInfo.result = 0;
    if (afnInfo.Fn == 2)
    {
        afnInfo.result = (int)precv->apply_region.unit_buf[0];
    }
    dzlog_info("AFN00_INFO->AFn : [%d]  AFN00_INFO->Fn : [%d]  afnInfo->result : [%d]", afnInfo.AFN, afnInfo.Fn, afnInfo.result);
    // dzlog_info("afnInfo->result : [%d]", afnInfo.result);
    //   hdzlog_info(precv->apply_region.unit_buf, 1);
    RES_INFO resInfo;
    resInfo.index = precv->recvIndex;
    // resInfo.gw13762DataType = GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE;
    resInfo.info = (void *)&afnInfo;
    resInfo.infoSize = sizeof(AFN00_INFO);
    resInfo.gw13762DataType = pdata->type;
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));
    return 0;
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
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    AFN00_INFO afnInfo = {0};
    if (precv->apply_region.AFN != 0x00)
    {
        dzlog_notice("%s AFN=%d error", __FUNCTION__,
                     pdata->recv.apply_region.AFN);
        return -1;
    }
    afnInfo.AFN = precv->apply_region.AFN;
    afnInfo.Fn = precv->apply_region.Fn;
    afnInfo.result = 0;
    if (afnInfo.Fn == 2)
    {
        afnInfo.result = (int)precv->apply_region.unit_buf[0];
    }
    dzlog_info("AFN00_INFO->AFn : [%d]  AFN00_INFO->Fn : [%d]  afnInfo->result : [%d]", afnInfo.AFN, afnInfo.Fn, afnInfo.result);
    // dzlog_info("afnInfo->result : [%d]", afnInfo.result);
    //   hdzlog_info(precv->apply_region.unit_buf, 1);
    RES_INFO resInfo;
    resInfo.index = precv->recvIndex;
    // resInfo.gw13762DataType = GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE;
    resInfo.info = (void *)&afnInfo;
    resInfo.infoSize = sizeof(AFN00_INFO);
    resInfo.gw13762DataType = pdata->type;
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));
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

    if (precv->apply_region.AFN != 3 || precv->apply_region.Fn != 4)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        return -1;
    }
    memset(pdata->send.addr_region.src, 0, 6);
    memcpy(pdata->send.addr_region.src, precv->apply_region.unit_buf, 6); // 存主节点地址
    RES_INFO resInfo;
    char addr[6];
    // 地址转换  修改
    reverseHexArray(addr, precv->apply_region.unit_buf, 6);

    // hdzlog_info(precv->apply_region.unit_buf, 6);
    // hdzlog_info(addr, 6);
    resInfo.index = precv->recvIndex;
    resInfo.gw13762DataType = GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR;
    resInfo.info = (void *)addr;
    resInfo.infoSize = sizeof(addr);
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));
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
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = 0x05;
    papply_region->Fn = 0x01;
    // 设置主节点地址  修改
    if (psend->ptask_data != NULL && psend->ptask_data->type == GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR)
    {
        reverseHexArray(papply_region->unit_buf, psend->ptask_data->buf, 6);
        papply_region->unit_len = 6;
    }

    return 0;
}

int protocol_gw1376_AFN05_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    AFN00_INFO afnInfo = {0};
    if (precv->apply_region.AFN != 0x00)
    {
        dzlog_notice("%s AFN=%d error", __FUNCTION__,
                     pdata->recv.apply_region.AFN);
        return -1;
    }
    afnInfo.AFN = precv->apply_region.AFN;
    afnInfo.Fn = precv->apply_region.Fn;
    if (afnInfo.Fn == 2)
    {
        afnInfo.result = (int)precv->apply_region.unit_buf;
    }
    RES_INFO resInfo;
    resInfo.index = precv->recvIndex;
    resInfo.gw13762DataType = GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR;
    resInfo.info = (void *)&afnInfo;
    resInfo.infoSize = sizeof(AFN00_INFO);
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));
    return 0;
}

int protocol_gw1376_AFN05_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    GW13762_TASK_DATA *ptask_data = psend->ptask_data;
    PROTOCOL_GW1376_2_APPLY_REGION *regin = protocol_gw1376_2_send_apply_region_get(pdata);
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);
    if (ptask_data != NULL && ptask_data->type == GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP)
    {
        regin->AFN = 0x05;
        regin->Fn = 2;
        regin->unit_len = ptask_data->len;
        memcpy(regin->unit_buf, ptask_data->buf, ptask_data->len);
    }

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
    // protocol_gw1376_2_sta_get_protocol_frame(papply_region->unit_buf, &papply_region->unit_len, pdata);

    char dstaddr[6] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
    memset(psend->addr_region.dest, 0, sizeof(psend->addr_region.dest));
    memcpy(psend->addr_region.dest, dstaddr, sizeof(dstaddr));
    papply_region->AFN = 0x05;
    papply_region->Fn = 0x03;
    memcpy(papply_region->unit_buf, psend->ptask_data->buf, psend->ptask_data->len);
    papply_region->unit_len = psend->ptask_data->len;
    return 0;
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
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    if (papply_region->AFN != 0x10 || papply_region->Fn != 1)
    {
        return -1;
    }

    if (papply_region->unit_len != 4)
    {
        dzlog_notice("%s unit_len error", __FUNCTION__);
        return -1;
    }

    int number = MAKEWORD(papply_region->unit_buf[0], papply_region->unit_buf[1]);

    RES_INFO resInfo;
    resInfo.index = precv->recvIndex;
    resInfo.gw13762DataType = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM;
    resInfo.info = (void *)&number;
    resInfo.infoSize = sizeof(int);
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));

    return 0;
}

int protocol_gw1376_AFN10_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    GW13762_TASK_DATA *ptask_data = psend->ptask_data;
    PROTOCOL_GW1376_2_APPLY_REGION regin;
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);
    if (ptask_data != NULL && ptask_data->type == GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO)
    {
        regin.AFN = 0x10;
        regin.Fn = 2;
        regin.unit_len = ptask_data->len;
        memcpy(regin.unit_buf, ptask_data->buf, 6);
    }

    return protoco_gw1376_2_cco_down_unit(pdata, &regin);
}

int protocol_gw1376_AFN10_Fn02_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    if (papply_region->AFN != 0x10 || papply_region->Fn != 2)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        return -1;
    }

    u16_t total =
        MAKEWORD(papply_region->unit_buf[0], papply_region->unit_buf[1]);
    u08_t validNum = papply_region->unit_buf[2];
    int index = 3;
    ACQ_FILES_INFO acqFilesInfo;
    acqFilesInfo.totalNum = total;
    acqFilesInfo.validNum = validNum;
    dzlog_info("total : [%d] validNum : [%d]", total, validNum);

    if (validNum != 0)
    {

        acqFilesInfo.fileInfos = (FILE_INFO *)malloc(sizeof(FILE_INFO) * validNum);
        if (acqFilesInfo.fileInfos == NULL)
        {
            dzlog_error("malloc failed!");
            return -1;
        }

        for (int i = 0; i < validNum; i++)
        {
            reverseHexArray(acqFilesInfo.fileInfos[i].addr, papply_region->unit_buf + index, 6);
            // hdzlog_info(acqFilesInfo.fileInfos[i].addr, 6);
            index += 6;
            acqFilesInfo.fileInfos[i].delayLevel = papply_region->unit_buf[index] & 0xf;
            acqFilesInfo.fileInfos[i].listenSignal = papply_region->unit_buf[index] >> 4 & 0xf;
            index++;
            acqFilesInfo.fileInfos[i].phase = papply_region->unit_buf[index] & 0x7;
            acqFilesInfo.fileInfos[i].proType = papply_region->unit_buf[index] >> 3 & 0x7;
            index++;
            // dzlog_info("delayLevel : [%d] listenSignal : [%d]  phase : [%d] proType : [%d]", acqFilesInfo.fileInfos->delayLevel, acqFilesInfo.fileInfos->listenSignal, acqFilesInfo.fileInfos->phase, acqFilesInfo.fileInfos->proType);
        }
    }
    RES_INFO resInfo;
    resInfo.index = precv->recvIndex;
    resInfo.gw13762DataType = GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO;
    resInfo.info = (void *)&acqFilesInfo;
    resInfo.infoSize = sizeof(ACQ_FILES_INFO);
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));
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
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);

    GW13762_TASK_DATA *ptask_data = psend->ptask_data;
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = 0x11;
    papply_region->Fn = 0x01;

    if (ptask_data != NULL && ptask_data->type == GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE)
    {
        memcpy(papply_region->unit_buf, ptask_data->buf, ptask_data->len);
    }
    papply_region->unit_len = ptask_data->len;

    if (protocol_gw1376_2_data_get_data_type(pdata) ==
        GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE)
    {
    }
    return 0;
}

int protocol_gw1376_AFN11_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    AFN00_INFO afnInfo = {0};
    if (precv->apply_region.AFN != 0x00)
    {
        dzlog_notice("%s AFN=%d error", __FUNCTION__,
                     pdata->recv.apply_region.AFN);
        return -1;
    }
    afnInfo.AFN = precv->apply_region.AFN;
    afnInfo.Fn = precv->apply_region.Fn;
    afnInfo.result = 0;
    if (afnInfo.Fn == 2)
    {
        afnInfo.result = (int)precv->apply_region.unit_buf[0];
    }
    dzlog_info("AFN00_INFO->AFn : [%d]  AFN00_INFO->Fn : [%d]  afnInfo->result : [%d]", afnInfo.AFN, afnInfo.Fn, afnInfo.result);
    // dzlog_info("afnInfo->result : [%d]", afnInfo.result);
    //   hdzlog_info(precv->apply_region.unit_buf, 1);
    RES_INFO resInfo;
    resInfo.index = precv->recvIndex;
    resInfo.gw13762DataType = GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE;
    resInfo.info = (void *)&afnInfo;
    resInfo.infoSize = sizeof(AFN00_INFO);
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));

    return 0;
}

int protocol_gw1376_AFN11_Fn02_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);

    GW13762_TASK_DATA *ptask_data = psend->ptask_data;
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_CCO);

    papply_region->AFN = 0x11;
    papply_region->Fn = 0x02;

    if (ptask_data != NULL && ptask_data->type == GW1376_2_DATA_TYPE_ROUTE_DEL_SUBNODE)
    {
        memcpy(papply_region->unit_buf, ptask_data->buf, ptask_data->len);
    }
    papply_region->unit_len = ptask_data->len;

    if (protocol_gw1376_2_data_get_data_type(pdata) ==
        GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE)
    {
    }
    return 0;
}

int protocol_gw1376_AFN11_Fn02_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    AFN00_INFO afnInfo = {0};
    if (precv->apply_region.AFN != 0x00)
    {
        dzlog_notice("%s AFN=%d error", __FUNCTION__,
                     pdata->recv.apply_region.AFN);
        return -1;
    }
    afnInfo.AFN = precv->apply_region.AFN;
    afnInfo.Fn = precv->apply_region.Fn;
    afnInfo.result = 0;
    if (afnInfo.Fn == 2)
    {
        afnInfo.result = (int)precv->apply_region.unit_buf[0];
    }
    dzlog_info("AFN00_INFO->AFn : [%d]  AFN00_INFO->Fn : [%d]  afnInfo->result : [%d]", afnInfo.AFN, afnInfo.Fn, afnInfo.result);
    // dzlog_info("afnInfo->result : [%d]", afnInfo.result);
    //   hdzlog_info(precv->apply_region.unit_buf, 1);
    RES_INFO resInfo;
    resInfo.index = precv->recvIndex;
    resInfo.gw13762DataType = GW1376_2_DATA_TYPE_ROUTE_DEL_SUBNODE;
    resInfo.info = (void *)&afnInfo;
    resInfo.infoSize = sizeof(AFN00_INFO);
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));

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

    // dev_data_set_state(pdev_data, DEV_STATE_NULL);
    protocol_gw1376_2_data_set_state(pdata, GW1376_2_DATA_TYPE_NULL);
    return 0;
}

int protocol_gw1376_AFNF1_Fn01_down(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_send_apply_region_get(pdata);

    GW13762_TASK_DATA *ptask_data = psend->ptask_data;
    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_STA);

    papply_region->AFN = 0xF1;
    papply_region->Fn = 0x01;

    if (ptask_data != NULL && ptask_data->type == GW1376_2_DATA_TYPE_CONCURRENT_METER_READING)
    {
        memset(psend->addr_region.dest, 0, sizeof(psend->addr_region.dest));
        memcpy(psend->addr_region.dest, ptask_data->sta_addr, sizeof(psend->addr_region.dest));
        memcpy(papply_region->unit_buf, ptask_data->buf, ptask_data->len);
    }
    papply_region->unit_len = ptask_data->len;
    return 0;
}

int protocol_gw1376_AFNF1_Fn01_up(PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_RECV_DATA *precv = protocol_gw1376_2_recv_data_get(pdata);
    PROTOCOL_GW1376_2_APPLY_REGION *papply_region =
        protocol_gw1376_2_recv_apply_region_get(pdata);

    if (papply_region->AFN != 0xF1 || papply_region->Fn != 0x01)
    {
        dzlog_notice("%s AFN or Fn error", __FUNCTION__);
        return -1;
    }

    CONCURRENT_INFO concurrentInfo = {0};
    concurrentInfo.proType = papply_region->unit_buf[0];

    int bufLen = MAKEWORD(papply_region->unit_buf[1], papply_region->unit_buf[2]);
    concurrentInfo.bufLen = bufLen;
    // dzlog_info("proType : [%d] bufLen : [%d] ", concurrentInfo.proType, bufLen);
    memcpy(concurrentInfo.buffer, papply_region->unit_buf + 3, bufLen);
    memcpy(concurrentInfo.addr, precv->addr_region.src, sizeof(concurrentInfo.addr));
    // hdzlog_info(concurrentInfo.buffer, bufLen);
    RES_INFO resInfo;
    resInfo.isReport = true;
    resInfo.index = precv->recvIndex;
    resInfo.gw13762DataType = GW1376_2_DATA_TYPE_CONCURRENT_METER_READING;
    resInfo.info = (void *)&concurrentInfo;
    resInfo.infoSize = sizeof(CONCURRENT_INFO);
    ListAddNode(precv->res_data_head, (void *)&resInfo, sizeof(RES_INFO));
    return 0;
}
