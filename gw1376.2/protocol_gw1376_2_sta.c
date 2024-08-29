#include "protocol_gw1376_2_sta.h"

int protocol_gw1376_2_sta_get_protocol_frame(char *buf, int *len,
                                             PROTOCOL_GW1376_2_DATA *pdata)
{
    PROTOCOL_GW1376_2_SEND_DATA *psend = protocol_gw1376_2_send_data_get(pdata);
    bool rtn = false;

    protocol_gw1376_2_data_set_send_comm_module_flag(pdata, GW1376_2_STA);;
    /* *len = protocol_gw1376_2_send_apply_region_unit_buf( */
    /*     pdata, (unsigned char *)(buf + 2)); */

    /* if (*len <= 0) */
    /*     return 0; */

    /* buf[1] = *len; */

    if (NULL != psend->ptask_data) {
        /* memcpy(papply_region->unit_buf + 2, psend->ptask_data->buf, */
        /*        psend->ptask_data->len); */
        /* papply_region->unit_len = psend->ptask_data->len+2; */
        buf[1] = gw13762_task_get_data_buf(psend->ptask_data,
                                           (unsigned char *)&buf[2], true);

        *len = buf[1] + 2;
    }

    /* *len += 2; */

    return *len;
}

int protocol_gw1376_2_sta_process_protocol_frame(char *buf, int len,
                                                 PROTOCOL_GW1376_2_DATA *pdata)
{
    /* COMM *pcomm = pdata->pcomm; */
    /* COMM_DATA *pcomm_data = &pcomm->data; */
    int rtn = 0;

    /* DEV_DATA *pdev_data = comm_data_get_current_dev(pcomm_data); */

    rtn = protocol_gw1376_2_set_recv_apply_region_unit_buf(
        pdata, (unsigned char *)buf, len);

    /* switch (pdev_data->pmodel->p_type) { */
    /* case PROTOCOL_DLT645: { */
    /*     /\* rtn = protocol_dlt645_07_process_buf((unsigned char *)buf + 2, len - 2, *\/ */
    /*     /\*                                      pcomm); *\/ */
    /*     rtn = protocol_gw1376_2_set_recv_apply_region_unit_buf( */
    /*         pdata, (unsigned char *)buf, len); */
    /* } */

    /* break; */
    /* default: */
    /*     break; */
    /* } */

    return rtn;
}
