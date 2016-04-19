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
 * Actions supporting running line display.
 */
#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ramp.h"
#include "int.h"

/*
 * Put up spike.
 */
int
rl_setspike(long spike_num)
{
    if(spike_num < 0 || spike_num >= RL_MAXSPIKE) {
		rxerr("rl_setspike(): Illegal spike number");
		return(0);
    }
    i_b->rl.rl_spike[spike_num].rl_spike_req = 1;
    return(0);
}

/*
 * Put up spike, using bit pattern as argument.
 */
int
rl_setspike_bits(long spike_bits)
{
  int i;

  for(i= 0; i < RL_MAXSPIKE ;i++) {
    if(spike_bits & 01) i_b->rl.rl_spike[i].rl_spike_req = 1;
    spike_bits >>= 1;
    if(!(spike_bits)) break;
  }
  if(spike_bits) {
	rxerr("rl_setspike(): Illegal spike number");
	return(0);
  }
  return(0);
}

/*
 * Set bar value.
 */
int
rl_setbar(long val)
{
    i_b->rl.rl_bar= val;
    return(0);
}

int
rl_addbar(long val)
{
    i_b->rl.rl_bar += val;
    return(0);
}

/*
 * Trigger a trace on running line.
 */
int
rl_trig(void)
{
    return(0);
}

/*
 * Stop a trace on running line.
 */
int
rl_stop(void)
{
    return(0);
}

int
rl_erase(void)
{
    return(0);
}
