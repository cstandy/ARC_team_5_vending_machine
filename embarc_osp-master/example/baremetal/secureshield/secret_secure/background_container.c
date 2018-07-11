/*------------------------------------------
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
 * \defgroup	EMBARC_APP_BAREMETAL_SECURESHIELD_SECRET_SECURE embARC SecureShield Secret Secure Example
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_BAREMETAL
 * \brief	embARC SecureShield secret secure example
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     - Designware nSIM Tool or
 *     - EMSK 2.2 arcem7d configuration
 *
 * ### Design Concept
 *     This example is designed to show how data could be protected with SecureShield. The application demonstrates a secure keystore component.
 *     - The secret and password are protected in the context of container 1 which is a container with secure privileges.
 *     - You need to input the right password to get the secret from the non-trusted background container; container 2 is trusted and can access the secret without password.
 *     - The init value of secret and password are stored in the data section of secure container 1. Non-secure containers can not access them directly.
 *     - Secure container 2 can use the container 1 secret internally for implementing other services. The background application can request such a service, without knowing or obtaining the secret itself.
 *     - For demonstration purpose, the background container can obtain the secret as well (using the password). Since the background container stores the result in global, public memory, this is normally not secure and not recommended. Instead see Container 2 for how to securely use the secret.
 *
 * ### Usage Manual
 *     When this example start to run, please try the following ntshell commands:
 *     - Run ntshell command **main -h** to see how to run this example.
 *     - Run ntshell command **main -p embarc** or **main -t** to get the secret.

 *     ![ScreenShot of secureshield example](pic/images/example/emsk/baremetal_secureshield_secret.jpg)
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_BAREMETAL_SECURESHIELD_SECRET_SECURE
 * \brief	secureshield secret secure example background container source file
 */

/**
 * \addtogroup	EMBARC_APP_BAREMETAL_SECURESHIELD_SECRET_SECURE
 * @{
 */
/* embARC HAL */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_assert.h"

#include "container1.h"
#include "container2.h"
#include "background_container.h"

static uint8_t public_data[SECRET_LEN];

static void main_help(void)
{
	EMBARC_PRINTF("Usage: main -p password -s secret -n new_pwd\r\n"
		"Operate the secret\r\n"
		"  -h/H/?    Show the help information\r\n"
		"  -p <pwd>  password\r\n"
		"  -s <secret> set new secret\r\n"
		"  -n <new_pwd> set new password\r\n"
		"  -t ask trusted container to access the secret for you\r\n"
		"  challenge 0, 1, or 2 : attempt obtaining the secret illegally\r\n"
		"Examples: \r\n"
		"  main challenge 0,1, or 2 : try to steal the secret method 0,1,2\r\n"
		"  main -t\r\n"
		"  main -p <pwd>  get the secret\r\n"
		"  main -p <pwd> -s <secret>   set the new secret\r\n"
		"  main -p <pwd> -n <new_pwd>  set the new password\r\n");
}

void default_interrupt_handler(void *p_exinf)
{
	EMBARC_PRINTF("default interrupt handler\r\n");
}

void soft_interrupt1(void *p_exinf)
{
	EMBARC_PRINTF("soft interrupt 1 interrupt handler\r\n");
}


int main(int argc, char **argv)
{
	int32_t opt;
	char *pwd = NULL;
	char *secret = NULL;
	char *new_pwd = NULL;

	int_handler_install(INTNO_SWI, soft_interrupt1);
	int_enable(INTNO_SWI);
	int_sw_trigger(INTNO_SWI);


	if (container_call(container1, init_secret)) {
		EMBARC_PRINTF("secret key initialized error\r\n");
		return E_SYS;
	}

	if (!strcmp(argv[1], "challenge")) {
		EMBARC_PRINTF("try to steal the secret\r\n");
		switch (argv[2][0]) {
			case '0' :
				EMBARC_PRINTF("the secret from container 1 context is:%s\r\n", container1_context.secret);
				break;
			case '1' :
			 	EMBARC_PRINTF("the secret from direct memory read is:%s\r\n", container1_secret);
			 	break;
			case '2' :
			 	EMBARC_PRINTF("the password from direct memory read is:%s\r\n", container1_pwd);
			 	break;
			default:
				EMBARC_PRINTF("Please use challenge 0, 1, or 2\r\n");
				return E_OK;
		}
		return E_OK;
	}

	opterr = 0;
	optind = 1;

	while ((opt=getopt(argc, argv, "tp:s:n:hH?")) != -1) {
		switch (opt) {
			case 'h':
			case '?':
			case 'H':
				main_help();
				goto error_exit;
				break;
			case 'p':
				pwd = optarg;
				break;
			case 's':
				secret = optarg;
				if (strlen(secret) > SECRET_LEN) {
					EMBARC_PRINTF("secret is too long\r\n");
					goto error_exit;
				}
				break;
			case 't':
				EMBARC_PRINTF("Requesting operation from trusted container\r\n");
				container_call(container2, trusted_ops);
				return E_OK;
				break;
			case 'n':
				new_pwd = optarg;
				if (strlen(new_pwd) > PWD_LEN) {
					EMBARC_PRINTF("max password length 6 bytes\r\n");
					goto error_exit;
				}
				break;
			default:
				main_help();
				goto error_exit;
				break;
		}
	}

	/* UART is in background container, so a hacker could hijack or access the UART and public_data to
	get the secret. Here it's just for demo. In a real application, container 2's method should be used */
	if (pwd != NULL) {
		if (secret == NULL && new_pwd == NULL) {
			if(container_call(container1, operate_secret, pwd, GET_SECRET, public_data)) {
				EMBARC_PRINTF("get secret failed\r\n");
			} else {
				EMBARC_PRINTF("the secret is:%s\r\n", public_data);
				memset(public_data, 0, SECRET_LEN);
			}
		} else if (secret != NULL && new_pwd == NULL) {
			if(container_call(container1, operate_secret, pwd, SET_SECRET, secret)) {
				EMBARC_PRINTF("set new secret failed\r\n");
			}
		} else if (secret == NULL && new_pwd != NULL) {
			if(container_call(container1, operate_secret, pwd, SET_PWD, new_pwd)) {
				EMBARC_PRINTF("set new password failed\r\n");
			}
		} else {
			EMBARC_PRINTF("cannot set password and secret at the same time\r\n");
		}
	} else {
		EMBARC_PRINTF("no password input\r\n");
	}

error_exit:
	return E_OK;
}

/** @} */