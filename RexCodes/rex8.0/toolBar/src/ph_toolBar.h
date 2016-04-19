/* Header "ph_toolBar.h" for toolBar Application */

#ifndef __PH_TOOBAR_H_INCLUDED
#define __PH_TOOBAR_H_INCLUDED


#ifndef __PT_BASIC_H_INCLUDED
#include <photon/PtBasic.h>
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

#ifndef __REX_H_INCLUDED
#define __REX_H_INCLUDED
#include "rexhdrs.h"
#include "int.h"
#include "tty.h"
#include <ctype.h>
#endif

#ifdef PCS_SOCKET
#include "../../hdr/pcsSocket.h"
#include "../../hdr/timerFunction.h"
#endif

#ifdef PCMESSAGE
#include "../../hdr/pcmsg.h"
#endif

#define MAX_STATE_NAME_LEN 32

#ifdef __cplusplus
};
#endif
#endif
