#include "protocol_gw1376_2.h"
#include "protocol_gw1376_2_public.h"
#include "protocol_gw1376_2_AFN.h"
#include <stdio.h>
#include "public.h"

/**
 *  \brief 获取数据类型的报文
 *  \param BYTE *buf
 *  \param int *len
 *  \param pdata 协议指针
 *  \return 报文长度
 */
static int protocol_gw1376_2_get_data(char *buf, int *len,
                                      PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn = -1;
    int auto_flag = 0;

    dzlog_debug("%s type=%d", __FUNCTION__, pdata->type);
    switch (pdata->type)
    {
    case GW1376_2_DATA_TYPE_HARD_INIT:
    {
        dzlog_debug("%s 硬件初始化", __FUNCTION__);
        rtn = protocol_gw1376_AFN01_Fn01_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_PARAM_INIT:
    {
        dzlog_debug("%s 参数初始化", __FUNCTION__);
        rtn = protocol_gw1376_AFN01_Fn02_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_DATA_INIT:
    {
        dzlog_debug("%s 数据初始化", __FUNCTION__);
        rtn = protocol_gw1376_AFN01_Fn03_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR:
    {
        dzlog_debug("%s 读cco 地址", __FUNCTION__);
        rtn = protocol_gw1376_AFN03_Fn04_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_READ_MAIN_RUN_MODE:
    {
        dzlog_debug("%s 读cco 运行模式", __FUNCTION__);
        rtn = protocol_gw1376_AFN03_Fn10_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR:
    {
        dzlog_debug("%s 写cco 主节点地址", __FUNCTION__);
        rtn = protocol_gw1376_AFN05_Fn01_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP:
    {
        dzlog_debug("%s 写cco 自动上送", __FUNCTION__);
        rtn = protocol_gw1376_AFN05_Fn02_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT:
    {
        dzlog_debug("%s 写cco 超时时间", __FUNCTION__);
        rtn = protocol_gw1376_AFN05_Fn04_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_TRANS_DATA:
    {
        dzlog_debug("%s cco 透传", __FUNCTION__);
        rtn = protocol_gw1376_AFN02_Fn01_down(pdata);
    }
    break;

    case GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA:
    {
        rtn = protocol_gw1376_AFN05_Fn03_down(pdata);
    }
    break;

    case GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM:
    {
        dzlog_debug("%s 读cco 子节点数量", __FUNCTION__);
        rtn = protocol_gw1376_AFN10_Fn01_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO:
    {
        dzlog_debug("%s 读cco 子节点信息", __FUNCTION__);
        rtn = protocol_gw1376_AFN10_Fn02_down(pdata);
    }
    break;

    case GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE:
    case GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE:
    {
        dzlog_debug("%s 添加cco 子节点信息", __FUNCTION__);
        rtn = protocol_gw1376_AFN11_Fn01_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_ROUTE_SET_SUBNODE_RELAY:
    {
        dzlog_debug("%s 设置 cco 子节点中继器", __FUNCTION__);
        rtn = protocol_gw1376_AFN11_Fn03_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_ROUTE_NET_TOPO_INFO:
    {
        dzlog_debug("%s 查询单个网络拓扑信息", __FUNCTION__);
        rtn = protocol_gw1376_AFN10_Fn21_down(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_ROUTE_ALL_NET_TOPO_INFO:
    {
        dzlog_debug("%s 查询所有网络拓扑信息", __FUNCTION__);
        rtn = protocol_gw1376_AFN10_Fn21_down_all(pdata);
    }
    break;
    case GW1376_2_DATA_TYPE_AUTOUP:
    {

        printf("autoup\n");
        /*
GW13762_TASK_DATA *ptdata = pdata->send.ptask_data;
if (ptdata->protocol_send) {
    auto_flag = 1;
    if (ptdata->protocol_recv(ptdata->buf, ptdata->len, pdata->pcomm)) {
        dzlog_debug("%s 主动处理645报文", __FUNCTION__);
    }
    dzlog_debug("%s 主动上送回复", __FUNCTION__);
    rtn = protocol_gw1376_AFN06_Fn05_down(pdata);
}
else {
}*/
    }
    break;
    default:
        break;
    }

    if (0 == rtn)
    {
        rtn = protocol_gw1376_pack_frame_data(buf, len, pdata);

        if (auto_flag > 0)
        {
            if (pdata->send.ptask_data)
            {
                GW13762_TASK_DATA *ptdata = pdata->send.ptask_data;
                gw13762_task_remove(&pdata->task, ptdata->index);
            }
        }

        return rtn;
    }

    return 0;
}

/**
 *  \brief 获取数据类型的报文
 *  \param BYTE *buf
 *  \param int *len
 *  \param pdata 协议指针
 *  \return 0 成功
 *  \return <0 失败
 */
static int protocol_gw1376_2_process_data(char *buf, int len,
                                          PROTOCOL_GW1376_2_DATA *pdata)
{
    int rtn;
    bool prm;
    if (protocol_gw1376_unpack_frame_data(buf, len, pdata) < 0)
    {
        pdata->unpack_frame_error_count++;
        /* 异常太多次，重新初始化HPLC */

        /*
        if (pdata->unpack_frame_error_count >= 10)
        {
            comm_data_set_state(&pdata->pcomm->data, COMM_STATE_STARTUP);
            comm_data_set_step(&pdata->pcomm->data, 0);
            pdata->unpack_frame_error_count = 0;
            dzlog_notice("%s set type=%d", __FUNCTION__,
                         GW1376_2_DATA_TYPE_PARAM_INIT);
        } */
        dzlog_notice("%s unpack error", __FUNCTION__);
        return -1;
    }
    pdata->unpack_frame_error_count = 0;

    prm = protocol_gw1376_2_data_set_recv_prm(pdata);
    /* 询问回复的报文 */
    if (!prm)
    {
        GW13762_TASK_DATA *ptdata = &pdata->recv.task_data;
        /* switch (pdata->type) { */
        switch (ptdata->type)
        {
        case GW1376_2_DATA_TYPE_HARD_INIT:
        case GW1376_2_DATA_TYPE_PARAM_INIT:
        case GW1376_2_DATA_TYPE_DATA_INIT:
        {
            rtn = protocol_gw1376_AFN01_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_READ_MAIN_NODE_ADDR:
        {
            rtn = protocol_gw1376_AFN03_Fn04_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_READ_MAIN_RUN_MODE:
        {
            rtn = protocol_gw1376_AFN03_Fn10_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_WRITE_MAIN_NODE_ADDR:
        {
            rtn = protocol_gw1376_AFN05_Fn01_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP:
        {
            rtn = protocol_gw1376_AFN05_Fn02_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_WRITE_SUBNODE_TIMEOUT:
        {
            rtn = protocol_gw1376_AFN05_Fn04_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_TRANS_DATA:
        {
            rtn = protocol_gw1376_AFN02_Fn01_up(pdata);
            if (ptdata->protocol_recv != NULL)
            {
                char proto_buf[PROTOCOL_BUF_LEN];
                int proto_len;

                proto_len = protocol_gw1376_2_recv_apply_region_unit_buf(
                    pdata, (unsigned char *)proto_buf);
                dzlog_debug("recv %d %d", proto_len, proto_buf[1]);

                /*
                if (len > 1 && buf[1] > 0)
                {
                    log_debug_frame((char *)proto_buf + 2, proto_len - 2);
                    // 处理协议报文
                    rtn = pdata->recv.task_data.protocol_recv(
                        (unsigned char *)proto_buf + 2, proto_len - 2,
                        pdata->pcomm);

                    if (rtn)
                    {
                        DEV_DATA *p =
                            comm_data_get_current_dev(&pdata->pcomm->data);
                        if (NULL != p)
                        {
                            dev_chg_data_update(p);
                        }
                    }
                }*/
            }
        }
        break;

        case GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA:
        {
            rtn = protocol_gw1376_AFN05_Fn03_up(pdata);
        }
        break;

        case GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_NUM:
        {
            dzlog_debug("%s 读cco 子节点数量处理", __FUNCTION__);
            rtn = protocol_gw1376_AFN10_Fn01_up(pdata);
        }
        break;

        case GW1376_2_DATA_TYPE_ROUTE_READ_SUBNODE_INFO:
        {
            rtn = protocol_gw1376_AFN10_Fn02_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_ROUTE_ADD_ALL_SUBNODE:
        case GW1376_2_DATA_TYPE_ROUTE_ADD_SUBNODE:
        {
            rtn = protocol_gw1376_AFN11_Fn01_up(pdata);
        }
        break;

        case GW1376_2_DATA_TYPE_ROUTE_SET_SUBNODE_RELAY:
        {
            rtn = protocol_gw1376_AFN11_Fn03_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_ROUTE_NET_TOPO_INFO:
        {
            dzlog_debug("%s 查询网络拓扑信息回复", __FUNCTION__);
            rtn = protocol_gw1376_AFN10_Fn21_up(pdata);
        }
        break;
        case GW1376_2_DATA_TYPE_ROUTE_ALL_NET_TOPO_INFO:
        {
            dzlog_debug("%s 查询所有网络拓扑信息", __FUNCTION__);
            rtn = protocol_gw1376_AFN10_Fn21_up_all(pdata);
        }
        break;
        default:
            rtn = -1;
            break;
        }

        protocol_gw1376_2_data_set_data_type(pdata, GW1376_2_DATA_TYPE_NULL);
    } /* 自动上送的报文 */
    else
    {
        PROTOCOL_GW1376_2_RECV_DATA *precv =
            protocol_gw1376_2_recv_data_get(pdata);
        PROTOCOL_GW1376_2_APPLY_REGION *papplydata = &precv->apply_region;

        dzlog_debug("auto up afn=%d fn=F%d", papplydata->AFN,
                    papplydata->Fn);
        switch (papplydata->AFN)
        {
        case 6:
        { /* 主动上报的报文 */
            switch (papplydata->Fn)
            {
            case 1:
            { /* 节点信息 */
            }
            break;
            case 2:
            { /* 抄读信息 */
            }
            break;
            case 3:
            { /* 路由工况变动信息 */
            }
            break;
            case 4:
            { /* 从节点及设备类型 */
            }
            break;
            case 5:
            { /* 从节点事件 */
                dzlog_debug("%s 主动上送", __FUNCTION__);
                protocol_gw1376_AFN06_Fn05_up(pdata);
            }
            break;
            default:
                break;
            }

            break;
        }
        default:
            break;
        }
        return 0;
    }

    /* PROTOCOL_GW1376_2_SEND_DATA *psend = &pdata->send; */
    /* psend->subnode_auto_up_enable = true; */

    /* if (pdata->type == GW1376_2_DATA_TYPE_TRANS_DATA || */
    /*     pdata->type == GW1376_2_DATA_TYPE_TRANS_BROADCAST_DATA) { */
    /*     /\* 禁止子节点主动上报 *\/ */
    /*     /\* PROTOCOL_GW1376_2_SEND_DATA *psend = &pdata->send; *\/ */
    /*     psend->subnode_auto_up = false; */
    /*     protocol_gw1376_2_data_set_data_type( */
    /*         pdata, GW1376_2_DATA_TYPE_WRITE_SUBNODE_AUTO_UP); */
    /* } */

    return rtn;
}

int protocol_gw1376_2_get_sendbuf(char *buf, int *len, void *p)
{
    PROTOCOL_GW1376_2_DATA *pprotocol_data =
        NULL; // 修改为全局变量

    /* int type = protocol_gw1376_2_get_data_type(pprotocol_data); */
    /* if (type <= 0 || type >= GW1376_2_DATA_TYPE_SIZE) { */
    /*     dzlog_notice("error type = %d", type); */
    /*     return 0; */
    /* } */

    /* pprotocol_data->type = type; */
    return protocol_gw1376_2_get_data(buf, len, pprotocol_data);
}

int protocol_gw1376_2_process_buf(char *buf, int len, void *p)
{
    PROTOCOL_GW1376_2_DATA *pprotocol_data =
        NULL; // protocol_gw1376_2_data_get_by_comm(p);  修改
    PROTOCOL_GW1376_2_RECV_DATA *precv =
        protocol_gw1376_2_recv_data_get(pprotocol_data);

    int pos = 0;
    int error = 0;

    while (pos <= len && pos >= 0)
    {
        int rtn;
        /* 找到有效报文 */
        pos = protocol_gw1376_2_find_valid_buf(buf, len, pprotocol_data, pos);
        if (pos < 0)
        {
            return -1;
        }

        dzlog_debug("value pos=%d", pos);
        rtn = protocol_gw1376_2_process_data(buf + pos, precv->value_len,
                                             pprotocol_data);

        if (rtn < 0)
        {
            error = -1;
        }

        pos += precv->value_len;
    }

    return error;
}

int protocol_gw1376_2_init(void *p)
{
    /*
    COMM *pcomm = (COMM *)p;
    COMM_DATA *pcomm_data = &pcomm->data;

    if (pcomm_data->protocol.init == 1)
    {
        return 0;
    }*/

    protocol_gw1376_2_data_init(p);

    /* /\* 初始化下面协议 *\/ */
    /* for (i = 0; i < pcomm_data->dev_num; i++) { */
    /*     /\* 初始化下面协议 *\/ */
    /*     DEV_DATA *pdev = pcomm_data->pdev[i]; */
    /*     MODEL_DATA *pmodel = pdev->pmodel; */

    /*     switch (pmodel->p_type) { */
    /*     case PROTOCOL_DLT645: { */
    /*         protocol_dlt645_07_init(p); */
    /*     } break; */
    /*     default: */
    /*         break; */
    /*     } */
    /* } */

    // pcomm_data->protocol.init = 1;
    return 0;
}

void protocol_gw1376_2_exit(void *p)
{

    /*
    COMM *pcomm = (COMM *)p;
    COMM_DATA *pcomm_data = &pcomm->data;
    int i;

    // 初始化下面协议
    for (i = 0; i < pcomm_data->dev_num; i++)
    {
        //初始化下面协议

        DEV_DATA *pdev = pcomm_data->pdev[i];
        MODEL_DATA *pmodel = pdev->pmodel;

        switch (pmodel->p_type)
        {
        case PROTOCOL_DLT645:
        {
            protocol_dlt645_07_exit(p);
        }
        break;
        default:
            break;
        }
    }
    */
    protocol_gw1376_2_data_exit(p);
    //pcomm_data->protocol.init = 0;
}
