/*
 *  Copyright (c) 2017, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "openthread/platform/misc.h"
#include "platform-arc.h"
#include "mrf24j40.h"

static MRF24J40_DEF mrf24j40_def = 
{
	.spi = MRF24J40_SPI_ID,
	.spi_cs = MRF24J40_SPI_CS,
	.gpio_pin_wake = DEV_GPIO_PORT_PIN_DEF(MRF24J40_GPIO_PORT_WAKE, MRF24J40_GPIO_PIN_WAKE),    // DEV_GPIO_PORT_0 --  DW_GPIO_PORT_A
	.gpio_pin_reset = DEV_GPIO_PORT_PIN_DEF(MRF24J40_GPIO_PORT_RESET, MRF24J40_GPIO_PIN_RESET),
	.gpio_pin_intr = DEV_GPIO_PORT_PIN_DEF(MRF24J40_GPIO_PIN_INTR, MRF24J40_GPIO_PIN_INTR),
};

void otPlatReset(otInstance *aInstance)
{
	// Default
	(void)aInstance;
	mrf24j40_reset(&mrf24j40_def);
}

otPlatResetReason otPlatGetResetReason(otInstance *aInstance)
{
	(void)aInstance;
	// TODO: Write me!
	return OT_PLAT_RESET_REASON_POWER_ON;
}

void otPlatWakeHost(void)
{
	// TODO: implement an operation to wake the host from sleep state.
}
