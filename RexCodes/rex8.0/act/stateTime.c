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
 *	set the preset and random times in a state
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "int.h"

int
set_times(char *name, long preset, long random)
{
	int i;
	extern STATE *snames[];

	i = 0;
	while(snames[i]) {
		if(!strcmp(name, snames[i]->statename)) {
			if(preset > -1) snames[i]->preset = preset;
			if(random > -1) snames[i]->random = random;
			break;
		}
		i++;
	}
	return(0);
}

int Pset_times(char *name, long *preset, long *random)
{
	set_times(name, *preset, *random);
	return(0);
}
    
long
getClockTime(void)
{
	return(i_b->i_time);
}

int PgetClockTime(long *time)
{
	*time = getClockTime();
	return(0);
}

int PsetFlag(int *flag, int val)
{
	*flag = val;
	return(0);
}
