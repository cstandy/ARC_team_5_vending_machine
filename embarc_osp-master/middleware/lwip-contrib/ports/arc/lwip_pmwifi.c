/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/*
 * Modified for port to ARC processor
 * by Huaqi Fang huaqi.fang@synopsys.com, Synopsys, Inc.
 */

/**
 * \defgroup	MID_LWIP_CONTRIB_PMWIFI		LWIP PMOD WIFI Interface
 * \ingroup	MID_LWIP_CONTRIB
 * \brief	lwip pmod wifi interface
 * @{
 *
 * \file
 * \brief	lwip pmod wifi interface
 */
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "lwip/opt.h"

#include "lwip/init.h"
#include "netif/etharp.h"
#include "lwip/autoip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/tcpip.h"

#include "lwip/ip_addr.h"

#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"

#include "lwip/sys.h"

#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "dev_wnic.h"
#include "arc_exception.h"

#include "pmwifi_netif.h"
#include "lwip_pmwifi.h"

#define DBG_LESS
#define DEBUG
#include "embARC_debug.h"

#include "board.h"

#define LWIP_PMWIFI_CHECK_EXP(EXPR, ERROR_CODE)        CHECK_EXP(EXPR, ercd, ERROR_CODE, error_exit)

/** MRF24W WIFI Module Network Interface Add & Enable */
static struct netif pmwifi_netif_local;
static PMWIFI_IF     pmwifi_if;
static int32_t pmwifi_isup = 0;

static void lwip_pmwifi_status_callback(struct netif *netif);

#define mac_addr_dump(macaddr)                                              \
                LWIP_DEBUGF(LWIP_DBG_ON, ("MAC ADDRESS:%02x-%02x-%02x-%02x-%02x-%02x\r\n", \
                    ((macaddr != NULL) ? ((uint8_t *)(macaddr))[0] : 0),    \
                    ((macaddr != NULL) ? ((uint8_t *)(macaddr))[1] : 0),    \
                    ((macaddr != NULL) ? ((uint8_t *)(macaddr))[2] : 0),    \
                    ((macaddr != NULL) ? ((uint8_t *)(macaddr))[3] : 0),    \
                    ((macaddr != NULL) ? ((uint8_t *)(macaddr))[4] : 0),    \
                    ((macaddr != NULL) ? ((uint8_t *)(macaddr))[5] : 0)))

/** WNIC ON OPERATIONS */
static int32_t lwip_pmwifi_on_init_success(DEV_WNIC *wnic_ptr)
{
    int32_t ercd = E_OK;
    PMWIFI_IF *wifi_if;

    wifi_if = (PMWIFI_IF *)wnic_ptr->wnic_info.extra;
    wnic_ptr->set_network_type(wnic_ptr->wnic_info.network_type);
#if WF_ENABLE_MANUAL_SET_MAC
    wnic_ptr->set_macaddr(wifi_if->netif->hwaddr);
#else /* get pmod wifi device mac address */
    wnic_ptr->get_macaddr(wifi_if->netif->hwaddr);
    LWIP_DEBUGF(LWIP_DBG_ON, ("%s ", wnic_ptr->wnic_info.name));
    mac_addr_dump((uint8_t *)(wifi_if->netif->hwaddr));
#endif
    return ercd;
}

static int32_t lwip_pmwifi_on_init_fail(DEV_WNIC *wnic_ptr)
{
    int32_t ercd = E_OK;

    wnic_ptr->wnic_info.init_status = WNIC_INIT_FAILED;
    wnic_ptr->wnic_info.power_status = WNIC_POWER_OFF;
    wnic_ptr->wnic_info.conn_status = WNIC_NOT_CONNECTED;
    LWIP_DEBUGF(LWIP_DBG_ON, ("\r\n%s Init Failed\r\n", wnic_ptr->wnic_info.name));

    return ercd;
}

static int32_t lwip_pmwifi_on_connected(DEV_WNIC *wnic_ptr)
{
    int32_t ercd = E_OK;
    static int once_flag = 0;
    PMWIFI_IF *wifi_if;

    wifi_if = (PMWIFI_IF *)wnic_ptr->wnic_info.extra;

    if (!netif_is_link_up(wifi_if->netif)) {
        netif_set_link_up(wifi_if->netif);
        LWIP_DEBUGF(LWIP_DBG_ON, ("Link is UP!\r\n"));
    }
    if (once_flag == 0) {
        once_flag = 1;
#if WF_IPADDR_DHCP && !NO_SYS   /* use dhcp to get address */
        netif_set_up(wifi_if->netif);
        netif_set_status_callback(wifi_if->netif, lwip_pmwifi_status_callback);
        dhcp_start(wifi_if->netif);
        LWIP_DEBUGF(LWIP_DBG_ON, ("\r\nNow start get ip address using DHCP, Please wait about 30s!\r\n"));
#else
        netif_set_status_callback(wifi_if->netif, lwip_pmwifi_status_callback);
        netif_set_up(wifi_if->netif);
#endif
    }

    return ercd;
}

static int32_t lwip_pmwifi_on_disconnected(DEV_WNIC *wnic_ptr)
{
    int32_t ercd = E_OK;
    PMWIFI_IF *wifi_if;

    wifi_if = (PMWIFI_IF *)wnic_ptr->wnic_info.extra;
    if (netif_is_link_up(wifi_if->netif)) {
        netif_set_link_down(wifi_if->netif);
        LWIP_DEBUGF(LWIP_DBG_ON, ("Link is Down!\r\n"));
    }

    return ercd;
}

static int32_t lwip_pmwifi_on_mac_updated(DEV_WNIC *wnic_ptr)
{
    int32_t ercd = E_OK;
    PMWIFI_IF *wifi_if;
    struct netif *netif;

    LWIP_DEBUGF(LWIP_DBG_ON, ("\r\nNow update mac address!\r\n"));

    wifi_if = (PMWIFI_IF *)wnic_ptr->wnic_info.extra;
    netif = wifi_if->netif;
    wnic_ptr->get_macaddr(netif->hwaddr);
    LWIP_DEBUGF(LWIP_DBG_ON, ("%s ", wnic_ptr->wnic_info.name));
    mac_addr_dump((uint8_t *)(netif->hwaddr));

    return ercd;
}

static int32_t lwip_pmwifi_on_scan_finished(DEV_WNIC *wnic_ptr)
{
    int32_t ercd = E_OK;

    LWIP_DEBUGF(LWIP_DBG_ON, ("\r\n%s scan finished, and find %d wireless hotspots\r\n", \
            wnic_ptr->wnic_info.name, wnic_ptr->wnic_info.scan_results));

    return ercd;
}

static int32_t lwip_pmwifi_on_assert(DEV_WNIC *wnic_ptr)
{
    int32_t ercd = E_OK;

    LWIP_DEBUGF(LWIP_DBG_ON, ("\r\nNow start reset the pmod wifi!\r\n"));

    wnic_ptr->wnic_reset();

    return ercd;
}

static void lwip_pmwifi_showipinfo(struct netif *netif)
{
#if LWIP_DNS
    const ip_addr_t *dns_server;
    int i = 0;
#endif

    LWIP_DEBUGF(LWIP_DBG_ON, ("\r\n-----PMOD WIFI IP ADDRESS INFO-----\r\n"));
    LWIP_DEBUGF(LWIP_DBG_ON, (" ipaddr "));
    ip_addr_debug_print(LWIP_DBG_ON, &(netif->ip_addr));
    LWIP_DEBUGF(LWIP_DBG_ON, (" netmask "));
    ip_addr_debug_print(LWIP_DBG_ON, &(netif->netmask));
    LWIP_DEBUGF(LWIP_DBG_ON, (" gw "));
    ip_addr_debug_print(LWIP_DBG_ON, &(netif->gw));

#if LWIP_DNS
    for (i=0; i<DNS_MAX_SERVERS; i++) {
    	dns_server = dns_getserver(i);
    	LWIP_DEBUGF(LWIP_DBG_ON, ("\r\n dns server %d :", i));
    	ip_addr_debug_print(LWIP_DBG_ON, dns_server);
    }
#endif

    LWIP_DEBUGF(LWIP_DBG_ON, ("\r\n-----------------------------------\r\n"));
}

static void lwip_pmwifi_status_callback(struct netif *netif)
{
    static int once_flag = 0;

    if (netif_is_up(netif)) {
        if (once_flag == 0){
            once_flag = 1;
            lwip_pmwifi_showipinfo(netif);
        }
        pmwifi_isup = 1;
    }
}

int32_t lwip_pmwifi_init(void)
{
    int32_t ercd = E_OK;
    struct netif *netif = &pmwifi_netif_local;
    PMWIFI_IF *pmwifi_if_ptr = &pmwifi_if;
    ip_addr_t addr, netmask, gw;

    memset(pmwifi_if_ptr, 0, sizeof(PMWIFI_IF));
    pmwifi_if_ptr->wnic = wnic_get_dev(BOARD_PMWIFI_0_ID);
    if (pmwifi_if_ptr->wnic == NULL) {
        ercd = E_OBJ;
        goto error_exit;
    }

    LWIP_DEBUGF(LWIP_DBG_ON, ("Start Init LWIP\r\n"));
#if NO_SYS
    lwip_init();
#else
    tcpip_init(NULL, NULL);
#endif

    /** on operations settings */
    WNIC_SET_ON_INIT_SUCCESS(pmwifi_if_ptr->wnic,   lwip_pmwifi_on_init_success);
    WNIC_SET_ON_INIT_FAIL(pmwifi_if_ptr->wnic,      lwip_pmwifi_on_init_fail);
    WNIC_SET_ON_CONNECTED(pmwifi_if_ptr->wnic,      lwip_pmwifi_on_connected);
    WNIC_SET_ON_DISCONNECTED(pmwifi_if_ptr->wnic,   lwip_pmwifi_on_disconnected);
    WNIC_SET_ON_MAC_UPDATED(pmwifi_if_ptr->wnic,    lwip_pmwifi_on_mac_updated);
    WNIC_SET_ON_SCAN_FINISHED(pmwifi_if_ptr->wnic,  lwip_pmwifi_on_scan_finished);
    WNIC_SET_ON_RXDATA_COMES(pmwifi_if_ptr->wnic,   pmwifi_on_input);
    WNIC_SET_ON_DEV_ASSERTED(pmwifi_if_ptr->wnic,   lwip_pmwifi_on_assert);

    pmwifi_if_ptr->netif = netif;
  /* get mannual set MAC hardware address */
#if WF_ENABLE_MANUAL_SET_MAC
    pmwifi_if_ptr->wnic->get_macaddr(netif->hwaddr);
#endif
    IP4_ADDR(&addr,     WF_IPADDR_1,  WF_IPADDR_2,  WF_IPADDR_3,  WF_IPADDR_4  );
    IP4_ADDR(&netmask,  WF_NETMASK_1, WF_NETMASK_2, WF_NETMASK_3, WF_NETMASK_4 );
    IP4_ADDR(&gw,       WF_GATEWAY_1, WF_GATEWAY_2, WF_GATEWAY_3, WF_GATEWAY_4 );

#if NO_SYS
    netif_add(netif, &addr, &netmask, &gw, (void *)pmwifi_if_ptr, pmwifi_if_init, ethernet_input);
#else
    netif_add(netif, &addr, &netmask, &gw, (void *)pmwifi_if_ptr, pmwifi_if_init, tcpip_input);
#endif

    netif_set_default(netif);

error_exit:
    return ercd;
}


int32_t lwip_pmwifi_isup(void)
{
    return pmwifi_isup;
}

/** @} end of group MID_LWIP_CONTRIB_PMWIFI */