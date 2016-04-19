/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * Actions to manipulate the analog storage window.
 */
#include <stdio.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "int.h"

unsigned int recvdMexMsgLen;

/*
 * Open, close, cancel window.
 */
int
awind(long flag)
{
	switch(flag) {
		case CANCEL_W:
			w_flags |= W_CANCEL;
			break;
		case CLOSE_W:
			w_flags |= W_CLOSE;
			break;
		case OPEN_W:
			w_flags |= W_OPEN;
			break;
		default:
			rxerr("awind(): bad flag");
			break;
	}
	return(0);
}

/*
 * Set pre and post time of analog window.  Can be overridden by menu.
 * Returns 0.
 */
int
pre_post(long pre, long post)
{
	unsigned eflags;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(m_pre != NULLI) w_pre= m_pre;
	else if(pre != NULLI) w_pre= pre;
	if(m_post != NULLI) w_post= m_post;
	else if(post != NULLI) w_post= post;
	if(eflags & INTR_IF) InterruptEnable();
	return(0);
}

int
Ppre_post(long *pre, long *post)
{
	pre_post(*pre, *post);
	return(0);
}

/*
 * this action sets a flag for mex units
 */
int
mwind(long flag)
{
#ifdef MEX62_UNITS
	if(!pcsMexConnected()) return(0);

	mexTime.rexClock = i_b->i_time + 10;
	mexTime.code = (++mexTime.code & 0x0007);

	switch(flag) {
	case CANCEL_W:
		mexTime.rexFlag = W_CANCEL;
		break;
	case OPEN_W:
		mexTime.rexFlag = W_OPEN;
		break;
	case CLOSE_W:
		mexTime.rexFlag = W_CLOSE;
		break;
	case CANCEL_LFP:
		mexTime.rexFlag = LFP_CANCEL;
		break;
	case OPEN_LFP:
		mexTime.rexFlag = LFP_OPEN;
		break;
	case CLOSE_LFP:
		mexTime.rexFlag = LFP_CLOSE;
		break;
	default:
		rxerr("mwind(): bad flag");
		break;
	}
	pcsSendMex((char *)&mexTime, sizeof(MEX_TIME));
	mexClockSendCtr = MEX_TIMER;
#endif
	return(0);
}
