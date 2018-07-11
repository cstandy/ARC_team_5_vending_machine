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
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIA\BILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

 /**
 * \file
 * \brief register arc kernel operation commands
 */
#include "cmd_cfg.h"
#if NTSHELL_USE_CMDS_ARC

#include "embARC.h"

#include "cmds_arc_cfg.h"

extern CMD_TABLE_T * register_ntshell_cmd_swape(CMD_TABLE_T *prev);
extern CMD_TABLE_T * register_ntshell_cmd_peek(CMD_TABLE_T *prev);
extern CMD_TABLE_T * register_ntshell_cmd_map(CMD_TABLE_T *prev);
extern CMD_TABLE_T * register_ntshell_cmd_dump(CMD_TABLE_T *prev);
extern CMD_TABLE_T * register_ntshell_cmd_bcr(CMD_TABLE_T *prev);
extern CMD_TABLE_T * register_ntshell_cmd_aux(CMD_TABLE_T *prev);
extern CMD_TABLE_T * register_ntshell_cmd_mem(CMD_TABLE_T *prev);

/** indicate whether the commands were registered */
CMD_TABLE_T* ntshell_register_cmds_arc(CMD_TABLE_T *prev)
{
	if (prev == NULL) return NULL;

	/* swape register */
#if NTSHELL_USE_CMDS_ARC_SWAPE
	prev = register_ntshell_cmd_swape(prev);
#endif
	/* peek, poke register */
#if NTSHELL_USE_CMDS_ARC_PEEK
	prev = register_ntshell_cmd_peek(prev);
#endif
	/* map register */
#if NTSHELL_USE_CMDS_ARC_MAP
	prev = register_ntshell_cmd_map(prev);
#endif
	/* dump register */
#if NTSHELL_USE_CMDS_ARC_DUMP
	prev = register_ntshell_cmd_dump(prev);
#endif
	/* bcr register */
#if NTSHELL_USE_CMDS_ARC_BCR
	prev = register_ntshell_cmd_bcr(prev);
#endif
	/* aux register */
#if NTSHELL_USE_CMDS_ARC_AUX
	prev = register_ntshell_cmd_aux(prev);
#endif
	/* mem register */
#if NTSHELL_USE_CMDS_ARC_MEM
	prev = register_ntshell_cmd_mem(prev);
#endif

	return prev;
}
#endif /* NTSHELL_USE_CMDS_ARC*/
