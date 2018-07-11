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
#ifndef _DEV_PINMUX_H_
#define _DEV_PINMUX_H_

#include "embARC_toolchain.h"

typedef enum arduino_pin {
	ARDUINO_PIN_0 = 0,
	ARDUINO_PIN_1,
	ARDUINO_PIN_2,
	ARDUINO_PIN_3,
	ARDUINO_PIN_4,
	ARDUINO_PIN_5,
	ARDUINO_PIN_6,
	ARDUINO_PIN_7,
	ARDUINO_PIN_8,
	ARDUINO_PIN_9,
	ARDUINO_PIN_10,
	ARDUINO_PIN_11,
	ARDUINO_PIN_12,
	ARDUINO_PIN_13,
	ARDUINO_PIN_AD0,
	ARDUINO_PIN_AD1,
	ARDUINO_PIN_AD2,
	ARDUINO_PIN_AD3,
	ARDUINO_PIN_AD4,
	ARDUINO_PIN_AD5
} ARDUINO_PIN;

typedef enum pmod_port {
	PMOD_0 = 0,
	PMOD_1,
	PMOD_2,
	PMOD_3,
	PMOD_4,
	PMOD_5,
	PMOD_6,
	PMOD_A = 0,
	PMOD_B,
	PMOD_C,
	PMOD_D,
	PMOD_E
} PMOD_PORT;

typedef enum pmod_type {
	PMOD_GPIO = 0,
	PMOD_UART,
	PMOD_SPI,
	PMOD_I2C,
	PMOD_PWM_PIN2,
	PMOD_PWM_PIN1,
	PMOD_PWM_PIN12
} PMOD_TYPE;

typedef enum pinmux_type {
	PINMUX_TYPE_DEFAULT = 0,
	PINMUX_TYPE_ARDUINO,
	PINMUX_TYPE_PMOD,
	PINMUX_TYPE_MIKRO
} PINMUX_TYPE;


#ifdef __cplusplus
extern "C" {
#endif

extern int32_t io_arduino_config_gpio(uint32_t num, uint32_t config);
extern int32_t io_arduino_config_spi(uint32_t config);
extern int32_t io_arduino_config_uart(uint32_t config);
extern int32_t io_arduino_config_i2c(uint32_t config);
extern int32_t io_arduino_config_pwm(uint32_t num, uint32_t config);
extern int32_t io_arduino_config_ad(uint32_t num, uint32_t config);
extern int32_t io_pmod_config(uint32_t pmod, uint32_t type, uint32_t config);
extern int32_t io_mikro_config(uint32_t config);
extern void io_mux_init(void);


#ifdef __cplusplus
}
#endif

#endif /* _DEV_PINMUX_H_ */