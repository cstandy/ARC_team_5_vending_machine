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
 * \defgroup	BOARD_HSDK_COMMON_INIT	HSDK Common Init Module
 * \ingroup	BOARD_HSDK_COMMON
 * \brief	HSDK Board Common Init Module
 * \details
 * 		HSDK timer/gpio/interrupt init process. Device-driver installation is done while
 *	getting the device object for the first time.
 */

/**
 * \file
 * \ingroup	BOARD_HSDK_COMMON_INIT
 * \brief	common hsdk init module
 */

/**
 * \addtogroup	BOARD_HSDK_COMMON_INIT
 * @{
 */
#include "arc_builtin.h"
#include "arc.h"
#include "arc_timer.h"
#include "embARC_debug.h"
#include "board.h"

/**
 * \brief	Board init routine MUST be called in each application
 * \note	It is better to disable interrupts when calling this function
 *	remember to enable interrupt when you want to use them
 */
void board_init(void)
{
	timer_init();
	io_mux_init();
	hsdk_hardware_init();
	hsdk_timer_init();
}

/** @} end of group BOARD_HSDK_COMMON_INIT */
