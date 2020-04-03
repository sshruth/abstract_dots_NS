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
 * Set saccade detector on/off with menu override.
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "int.h"
#include "sac.h"

int
sd_set(long ctlval)
{
	if(m_sdctl != NULLI) sdctl= m_sdctl;
	else if(ctlval != NULLI) sdctl= ctlval;
	return(0);
}

/*
 * return the initial position of the eye at the start of a saccade
 */
int
Psd_startPos(int *x, int *y)
{
	*x = h_sacinit;
	*y = v_sacinit;
	return(0);
}

/*
 * return the final position of the eye at the end of a saccade
 */
int
Psd_finalPos(int *x, int *y)
{
	*x = h_sacend;
	*y = v_sacend;
	return(0);
}

/*
 * return the horizontal and vertical amplitudes of a saccade
 */
int
Psd_size(int *x, int *y)
{
	*x = h_sacsize;
	*y = v_sacsize;
	return(0);
}

/*
 * Place mark on running line.
 */
int
sd_mark(long chan, long mark)
{

    if(chan > RL_MAXCHAN) {
		rxerr("sd_mark(): Illegal channel number");
		return(0);
    }
    if(mark > RL_MARKMAX) {
		rxerr("sd_mark(): Illegal mark number");
		return(0);
    }
    i_b->rl.rl_chan[chan].rl_mark_req = mark;
    return(0);
}

/*
 * Place mark on running line with signal name argument.
 * This is not an action!
 */
void
sd_mark_sig(char *sig_name, int mark)
{
    RL_CHAN *rcp;
    int chan;

    rcp= &i_b->rl.rl_chan[0];
    for(chan= 0; chan < RL_MAXCHAN; chan++, rcp++) {
		if(strncmp(rcp->rl_sig_gvname, sig_name, P_LNAME) == 0) {
			sd_mark(chan, mark);
			return;
		}
    }
    rxerr("sd_mark_sig(): Bad signal name");
}
    
