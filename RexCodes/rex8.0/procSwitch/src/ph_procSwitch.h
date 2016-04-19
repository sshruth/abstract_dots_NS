/* Header "ph_procSwitch.h" for procSwitch Application */

#ifndef __PT_PH_PROCSWITCH_H_INCLUDED
#define __PT_PH_PROCSWITCH_H_INCLUDED

#ifndef __PT_BASIC_H_INCLUDED
#include <photon/PtBasic.h>
#endif

#ifndef __REX_H_INCLUDED
#define __REX_H_INCLUDED
#include "rexhdrs.h"
#include "tty.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * If the following is defined, then signal handling is compatable with Photon.
 * This means that Photon functions can be called from the signal handler.
 * Photon accomplishes this by catching the signal first, and then calling the
 * signal handler from Photon's main loop.  Defining this removes some of
 * the REX 'alert()' functionality, however-  when processing a noun,verb preemption
 * by another message isnt possible.
 */
#define PHOTON_SIG_COMPAT

/*
 * Flags local to procSwitch kept in sysflags.
 */
#define C_QUIT		01		/* terminate REX */
#define C_SENDNAME	02		/* child is ready to send names */
#define C_BADCHILD	04		/* child forked by comm has failed */
#define C_NAMERDY	010		/* comm is ready to receive names from child */
#define C_AFFIRM	020		/* process successfully responds to a comm
							 * message (ususally run or stop) */
#define C_NEG		040		/* process cannot repsond successfully to a
							 * comm message */
#define C_STARTED	0100	/* process tells procSwitch that it has started */

#define clrack_		(sysflags &= ~(C_AFFIRM|C_NEG|C_STARTED))

enum filesEnum {
	PROCESS_RUN,
	PROCESS_CHANGE,
	PROCESS_LOAD,
	DATA_OPEN,
	ROOT_FILE_READ,
	ROOT_FILE_ALL,
	ROOT_FILE_SYSTEM_USER,
	ROOT_FILE_SYSTEM,
	ROOT_FILE_USER,
	ROOT_FILE_STATES
};

#ifdef __cplusplus
};
#endif
#endif
