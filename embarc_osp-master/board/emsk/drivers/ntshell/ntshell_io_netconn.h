/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
/**
 * \file
 * \ingroup	BOARD_EMSK_DRV_MID_NTSHELL_IO_NETCONN
 * \brief	header file of middleware ntshell io netconn interface driver
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_MID_NTSHELL_IO_NETCONN
 * @{
 */
#ifndef _NTSHELL_IO_NETCONN_H_
#define _NTSHELL_IO_NETCONN_H_

#include "embARC_toolchain.h"

#if defined(MID_NTSHELL) && defined(MID_LWIP) \
	&& defined(ENABLE_OS)  /* only available when enable ntshell & lwip middleware */
#include "ntshell_task.h"

#define USE_EMSK_NTSHELL_NETCONN_0	1

#define NT_NETCONN_0_PRIO		9

#ifdef __cplusplus
extern "C" {
#endif

#if USE_EMSK_NTSHELL_NETCONN_0
extern NTSHELL_IO ntshell_netconn_0;
#endif /** USE_EMSK_NTSHELL_NETCONN_0 */

#ifdef __cplusplus
}
#endif

#endif /** MID_NTSHELL  MID_LWIP */

#endif /** _NTSHELL_IO_NETCONN_H_ */

/** @} end of group BOARD_EMSK_DRV_MID_NTSHELL_IO_NETCONN */
