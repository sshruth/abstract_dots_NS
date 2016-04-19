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

#include <stdio.h>
#include <sys/types.h>
#include "rexhdrs.h"

/*
 * Actions to control digital output interfaces.
 */
int
dio_on(DIO_ID id)
{
    dio_set(id, 0, D_ON);
    return(0);
}

int Pdio_on(DIO_ID *id)
{
	dio_on(*id);
	return(0);
}
    
int
dio_off(DIO_ID id)
{
    dio_set(id, 0, D_OFF);
    return(0);
}

int Pdio_off(DIO_ID *id)
{
	dio_off(*id);
	return(0);
}
    
int
dio_onoff(DIO_ID id_on, DIO_ID id_off)
{
    dio_set(id_on, 0, D_ON);
    dio_set(id_off, 0, D_OFF);
    return(0);
}

int Pdio_onoff(DIO_ID *id_on, DIO_ID *id_off)
{
	dio_onoff(*id_on, *id_off);
}

int
dio_out(DIO_ID id, long value)
{
    dio_set(id, value, D_VAL);
    return(0);
}

int Pdio_out(DIO_ID *id, long *value)
{
	dio_out(*id, *value);
	return(0);
}
