/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
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
 *    David Navarro, Intel Corporation - initial API and implementation
 *
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "connection.h"

#include "embARC_debug.h"

int create_socket(uint16_t bind_flag, uint16_t local_port)
{
    int s = -1;
    struct sockaddr_in saddr;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(local_port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if ((bind_flag > 0) && (s >= 0)) {
        if (-1 == bind(s, (struct sockaddr *)(&saddr), sizeof(saddr))) {
            close(s);
            s = -1;
        }
    }

    return s;
}

connection_t * connection_find(connection_t * connList,
                               struct sockaddr_storage * addr,
                               size_t addrLen)
{
    connection_t * connP;

    connP = connList;
    while (connP != NULL)
    {
        if ((connP->addrLen == addrLen)
         && (memcmp(&(connP->addr), addr, addrLen) == 0))
        {
            return connP;
        }
        connP = connP->next;
    }

    return connP;
}

connection_t * connection_new_incoming(connection_t * connList,
                                       int sock,
                                       struct sockaddr * addr,
                                       size_t addrLen)
{
    connection_t * connP;

    connP = (connection_t *)malloc(sizeof(connection_t));
    if (connP != NULL)
    {
        connP->sock = sock;
        memcpy(&(connP->addr), addr, addrLen);
        connP->addrLen = addrLen;
        connP->next = connList;
    }

    return connP;
}

connection_t * connection_create(connection_t * connList,
                                 int sock,
                                 char * host,
                                 uint16_t port)
{
    char portStr[6];
    struct addrinfo hints;
    struct addrinfo *servinfo = NULL;
    struct addrinfo *p;
    int s;
    struct sockaddr *sa;
    socklen_t sl;
    connection_t * connP = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (0 >= sprintf(portStr, "%hu", port)) return NULL;
    if (0 != getaddrinfo(host, portStr, &hints, &servinfo) || servinfo == NULL) return NULL;

    // we test the various addresses
    s = -1;
    for(p = servinfo ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            sa = p->ai_addr;
            sl = p->ai_addrlen;
            if (-1 == connect(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }
        }
    }
    freeaddrinfo(servinfo);

    if (s >= 0)
    {
        connP = connection_new_incoming(connList, sock, sa, sl);
        close(s);
    }

    return connP;
}

void connection_free(connection_t * connList)
{
    if (connList != NULL)
    {
        connection_t * nextP;

        nextP = connList->next;
        free(connList);

        connection_free(nextP);
    }
}

int connection_send(connection_t *connP,
                    uint8_t * buffer,
                    size_t length)
{
    size_t nbSent;
    size_t offset;

    offset = 0;
    while (offset != length)
    {
        nbSent = sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
        if (nbSent == -1) return -1;
        offset += nbSent;
    }
    return 0;
}

void output_buffer(uint8_t * buffer,
                   int length)
{
    int i;
    int j;

    i = 0;
    while (i < length)
    {

        EMBARC_PRINTF("  ");

        for (j = 0 ; j < 16 && i+j < length; j++)
        {
            EMBARC_PRINTF("%02X ", buffer[i+j]);
        }
        if (i != 0)
        {
            while (j < 16)
            {
                EMBARC_PRINTF("   ");
                j++;
            }
        }
        EMBARC_PRINTF("  ");
        for (j = 0 ; j < 16 && i+j < length; j++)
        {
            if (isprint(buffer[i+j]))
            {
                EMBARC_PRINTF("%c ", buffer[i+j]);
            }
            else
            {
                EMBARC_PRINTF(". ");
            }
        }
        EMBARC_PRINTF("\n");

        i += 16;
    }
}


