/*******************************************************************************
 *
 * Copyright (c) 2014 Bosch Software Innovations GmbH Germany. 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

/*
 *  This Connectivity Monitoring object is optional and has a single instance
 * 
 *  Resources:
 *
 *          Name             | ID | Oper. | Inst. | Mand.|  Type   | Range | Units |
 *  Network Bearer           |  0 |  R    | Single|  Yes | Integer |       |       |
 *  Available Network Bearer |  1 |  R    | Multi |  Yes | Integer |       |       |
 *  Radio Signal Strength    |  2 |  R    | Single|  Yes | Integer |       | dBm   |
 *  Link Quality             |  3 |  R    | Single|  No  | Integer | 0-100 |   %   |
 *  IP Addresses             |  4 |  R    | Multi |  Yes | String  |       |       |
 *  Router IP Addresses      |  5 |  R    | Multi |  No  | String  |       |       |
 *  Link Utilization         |  6 |  R    | Single|  No  | Integer | 0-100 |   %   |
 *  APN                      |  7 |  R    | Multi |  No  | String  |       |       |
 *  Cell ID                  |  8 |  R    | Single|  No  | Integer |       |       |
 *  SMNC                     |  9 |  R    | Single|  No  | Integer | 0-999 |   %   |
 *  SMCC                     | 10 |  R    | Single|  No  | Integer | 0-999 |       |
 *
 */

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lwip/ip.h"

// Resource Id's:
#define RES_M_NETWORK_BEARER            0
#define RES_M_AVL_NETWORK_BEARER        1
#define RES_M_RADIO_SIGNAL_STRENGTH     2
#define RES_O_LINK_QUALITY              3
#define RES_M_IP_ADDRESSES              4
#define RES_O_ROUTER_IP_ADDRESS         5
#define RES_O_LINK_UTILIZATION          6
#define RES_O_APN                       7
#define RES_O_CELL_ID                   8
#define RES_O_SMNC                      9
#define RES_O_SMCC                      10

#define VALUE_NETWORK_BEARER_GSM    0   //GSM see 
#define VALUE_AVL_NETWORK_BEARER_1  0   //GSM
#define VALUE_AVL_NETWORK_BEARER_2  21  //WLAN
#define VALUE_AVL_NETWORK_BEARER_3  41  //Ethernet
#define VALUE_AVL_NETWORK_BEARER_4  42  //DSL
#define VALUE_AVL_NETWORK_BEARER_5  43  //PLC
#define VALUE_IP_ADDRESS_1              "192.168.1.101"
#define VALUE_IP_ADDRESS_2              "192.168.1.102"
#define VALUE_ROUTER_IP_ADDRESS_1       "192.168.1.001"
#define VALUE_ROUTER_IP_ADDRESS_2       "192.168.1.002"
#define VALUE_APN_1                     "embARC.org"
#define VALUE_APN_2                     "www.synopsys.com"
#define VALUE_CELL_ID                   69696969
#define VALUE_RADIO_SIGNAL_STRENGTH     80                  //dBm
#define VALUE_LINK_QUALITY              98
#define VALUE_LINK_UTILIZATION          666
#define VALUE_SMNC                      33
#define VALUE_SMCC                      44

typedef struct
{
    char ipAddresses[2][16];        // limited to 2!
    char routerIpAddresses[2][16];  // limited to 2!
    long cellId;
    int signalStrength;
    int linkQuality;
    int linkUtilization;
} conn_m_data_t;

static uint8_t prv_set_value(lwm2m_tlv_t * tlvP,
                             conn_m_data_t * connDataP)
{
    struct netif *netif = netif_list;
    switch (tlvP->id)
    {
    case RES_M_NETWORK_BEARER:
        lwm2m_tlv_encode_int(VALUE_NETWORK_BEARER_GSM, tlvP);
        tlvP->type = LWM2M_TYPE_RESOURCE;
        if (0 != tlvP->length) return COAP_205_CONTENT ;
        else return COAP_500_INTERNAL_SERVER_ERROR ;
        break;

    case RES_M_AVL_NETWORK_BEARER:
    {
        int riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_tlv_t * subTlvP;
        subTlvP = lwm2m_tlv_new(riCnt);
        subTlvP[0].flags = 0;
        subTlvP[0].id    = 0;
        subTlvP[0].type  = LWM2M_TYPE_RESOURCE_INSTANCE;
        lwm2m_tlv_encode_int(VALUE_AVL_NETWORK_BEARER_1, subTlvP);
        if (0 == subTlvP[0].length)
        {
            lwm2m_tlv_free(riCnt, subTlvP);
            return COAP_500_INTERNAL_SERVER_ERROR ;
        }
        lwm2m_tlv_include(subTlvP, riCnt, tlvP);
        return COAP_205_CONTENT ;
    }
        break;

    case RES_M_RADIO_SIGNAL_STRENGTH: //s-int
        lwm2m_tlv_encode_int(connDataP->signalStrength, tlvP);
        tlvP->type = LWM2M_TYPE_RESOURCE;
        if (0 != tlvP->length)
            return COAP_205_CONTENT ;
        else
            return COAP_500_INTERNAL_SERVER_ERROR ;
        break;

    case RES_O_LINK_QUALITY: //s-int
        lwm2m_tlv_encode_int(connDataP->linkQuality, tlvP);
        tlvP->type = LWM2M_TYPE_RESOURCE;
        if (0 != tlvP->length) return COAP_205_CONTENT ;
        else return COAP_500_INTERNAL_SERVER_ERROR ;
        break;

    case RES_M_IP_ADDRESSES:
    {
        int ri, riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_tlv_t* subTlvP = lwm2m_tlv_new(riCnt);
        netif = netif_list;
        for (ri=0; ri<riCnt; ri++)
        {
            subTlvP[ri].flags  = LWM2M_TLV_FLAG_STATIC_DATA;
            subTlvP[ri].id     = 0;
            subTlvP[ri].type   = LWM2M_TYPE_RESOURCE_INSTANCE;
            subTlvP[ri].dataType = LWM2M_TYPE_STRING;
            if (netif != NULL) {
                sprintf(connDataP->ipAddresses[ri], "%d.%d.%d.%d", \
                    ip4_addr1_16(&(netif->ip_addr)), ip4_addr2_16(&(netif->ip_addr)), \
                    ip4_addr3_16(&(netif->ip_addr)), ip4_addr4_16(&(netif->ip_addr)) );
                netif = netif->next;
            }
            subTlvP[ri].value  = (uint8_t*) connDataP->ipAddresses[ri];
            subTlvP[ri].length = strlen(connDataP->ipAddresses[ri]);
            if (subTlvP[ri].length == 0)
            {
                lwm2m_tlv_free(riCnt, subTlvP);
                return COAP_500_INTERNAL_SERVER_ERROR ;
            }
        }
        lwm2m_tlv_include(subTlvP, riCnt, tlvP);
        return COAP_205_CONTENT ;
    }
        break;

    case RES_O_ROUTER_IP_ADDRESS:
    {
        int ri, riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_tlv_t* subTlvP = lwm2m_tlv_new(riCnt);
        netif = netif_list;
        for (ri=0; ri<riCnt; ri++)
        {
            subTlvP[ri].flags  = LWM2M_TLV_FLAG_STATIC_DATA;
            subTlvP[ri].id     = 0;
            subTlvP[ri].type   = LWM2M_TYPE_RESOURCE_INSTANCE;
            subTlvP[ri].dataType = LWM2M_TYPE_STRING;
            if (netif != NULL) {
                sprintf(connDataP->routerIpAddresses[ri], "%d.%d.%d.%d", \
                    ip4_addr1_16(&(netif->gw)), ip4_addr2_16(&(netif->gw)), \
                    ip4_addr3_16(&(netif->gw)), ip4_addr4_16(&(netif->gw)) );
                netif = netif->next;
            }

            subTlvP[ri].value  = (uint8_t*) connDataP->routerIpAddresses[ri];
            subTlvP[ri].length = strlen(connDataP->routerIpAddresses[ri]);
            if (subTlvP[ri].length == 0)
            {
                lwm2m_tlv_free(riCnt, subTlvP);
                return COAP_500_INTERNAL_SERVER_ERROR ;
            }
        }
        lwm2m_tlv_include(subTlvP, riCnt, tlvP);
        return COAP_205_CONTENT ;
    }
        break;

    case RES_O_LINK_UTILIZATION:
        lwm2m_tlv_encode_int(connDataP->linkUtilization, tlvP);
        tlvP->type = LWM2M_TYPE_RESOURCE;
        if (0 != tlvP->length)
            return COAP_205_CONTENT ;
        else
            return COAP_500_INTERNAL_SERVER_ERROR ;
        break;

    case RES_O_APN:
    {
        int riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_tlv_t * subTlvP;
        subTlvP = lwm2m_tlv_new(riCnt);
        subTlvP[0].flags  = LWM2M_TLV_FLAG_STATIC_DATA;
        subTlvP[0].id     = 0;
        subTlvP[0].type   = LWM2M_TYPE_RESOURCE_INSTANCE;
        subTlvP[0].dataType = LWM2M_TYPE_STRING;
        subTlvP[0].value  = (uint8_t*) VALUE_APN_1;
        subTlvP[0].length = strlen(VALUE_APN_1);
        if (0 == subTlvP[0].length)
        {
            lwm2m_tlv_free(riCnt, subTlvP);
            return COAP_500_INTERNAL_SERVER_ERROR ;
        }
        lwm2m_tlv_include(subTlvP, riCnt, tlvP);
        return COAP_205_CONTENT;
    }
        break;

    case RES_O_CELL_ID:
        lwm2m_tlv_encode_int(connDataP->cellId, tlvP);
        tlvP->type = LWM2M_TYPE_RESOURCE;
        if (0 != tlvP->length) return COAP_205_CONTENT ;
        else return COAP_500_INTERNAL_SERVER_ERROR ;
        break;

    case RES_O_SMNC:
        lwm2m_tlv_encode_int(VALUE_SMNC, tlvP);
        tlvP->type = LWM2M_TYPE_RESOURCE;
        if (0 != tlvP->length) return COAP_205_CONTENT ;
        else return COAP_500_INTERNAL_SERVER_ERROR ;
        break;

    case RES_O_SMCC:
        lwm2m_tlv_encode_int(VALUE_SMCC, tlvP);
        tlvP->type = LWM2M_TYPE_RESOURCE;
        if (0 != tlvP->length) return COAP_205_CONTENT ;
        else return COAP_500_INTERNAL_SERVER_ERROR ;
        break;

    default:
        return COAP_404_NOT_FOUND ;
    }
}

static uint8_t prv_read(uint16_t instanceId,
                        int * numDataP,
                        lwm2m_tlv_t ** dataArrayP,
                        lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND ;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
                RES_M_NETWORK_BEARER,
                RES_M_AVL_NETWORK_BEARER,
                RES_M_RADIO_SIGNAL_STRENGTH,
                RES_O_LINK_QUALITY,
                RES_M_IP_ADDRESSES,
                RES_O_ROUTER_IP_ADDRESS,
                RES_O_LINK_UTILIZATION,
                RES_O_APN,
                RES_O_CELL_ID,
                RES_O_SMNC,
                RES_O_SMCC
        };
        int nbRes = sizeof(resList) / sizeof(uint16_t);

        *dataArrayP = lwm2m_tlv_new(nbRes);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR ;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }
    }

    i = 0;
    do
    {
        result = prv_set_value((*dataArrayP) + i, (conn_m_data_t*) (objectP->userData));
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT );

    return result;
}

static void prv_close(lwm2m_object_t * objectP)
{
    lwm2m_free(objectP->userData);
    lwm2m_list_free(objectP->instanceList);
}

lwm2m_object_t * get_object_conn_m(void)
{
    /*
     * The get_object_conn_m() function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * connObj;

    connObj = (lwm2m_object_t *) lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != connObj)
    {
        memset(connObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns his unique ID
         */
        connObj->objID = LWM2M_CONN_MONITOR_OBJECT_ID;
        
        /*
         * and its unique instance
         *
         */
        connObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != connObj->instanceList)
        {
            memset(connObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(connObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        connObj->readFunc  = prv_read;
        connObj->closeFunc = prv_close;
        connObj->executeFunc = NULL;
        connObj->userData  = lwm2m_malloc(sizeof(conn_m_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != connObj->userData)
        {
            conn_m_data_t *myData = (conn_m_data_t*) connObj->userData;
            myData->cellId          = VALUE_CELL_ID;
            myData->signalStrength  = VALUE_RADIO_SIGNAL_STRENGTH;
            myData->linkQuality     = VALUE_LINK_QUALITY;
            myData->linkUtilization = VALUE_LINK_UTILIZATION;
            strcpy(myData->ipAddresses[0],       VALUE_IP_ADDRESS_1);
            strcpy(myData->ipAddresses[1],       VALUE_IP_ADDRESS_2);
            strcpy(myData->routerIpAddresses[0], VALUE_ROUTER_IP_ADDRESS_1);
            strcpy(myData->routerIpAddresses[1], VALUE_ROUTER_IP_ADDRESS_2);
        }
        else
        {
            lwm2m_free(connObj);
            connObj = NULL;
        }
    }
    return connObj;
}

uint8_t connectivity_moni_change(lwm2m_tlv_t * dataArray,
                                 lwm2m_object_t * objectP)
{
    int64_t value;
    uint8_t result;
    conn_m_data_t * data;

    data = (conn_m_data_t*) (objectP->userData);

    switch (dataArray->id)
    {
    case RES_M_RADIO_SIGNAL_STRENGTH:
        if (1 == lwm2m_tlv_decode_int(dataArray, &value))
        {
            data->signalStrength = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
        break;

    case RES_O_LINK_QUALITY:
        if (1 == lwm2m_tlv_decode_int(dataArray, &value))
        {
            data->linkQuality = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
        break;

    case RES_M_IP_ADDRESSES:
        if (sizeof(data->ipAddresses[0]) <= dataArray->length)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else
        {
            memset(data->ipAddresses[0], 0, sizeof(data->ipAddresses[0]));
            memcpy(data->ipAddresses[0], dataArray->value, dataArray->length);
            data->ipAddresses[0][dataArray->length] = 0;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_ROUTER_IP_ADDRESS:
        if (sizeof(data->routerIpAddresses[0]) <= dataArray->length)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else
        {
            memset(data->routerIpAddresses[0], 0, sizeof(data->routerIpAddresses[0]));
            memcpy(data->routerIpAddresses[0], dataArray->value, dataArray->length);
            data->routerIpAddresses[0][dataArray->length] = 0;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_CELL_ID:
        if (1 == lwm2m_tlv_decode_int(dataArray, &value))
        {
            data->cellId = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
        break;

    default:
        result = COAP_405_METHOD_NOT_ALLOWED;
    }

    return result;
}

