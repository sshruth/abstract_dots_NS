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
 * Map shared memory area.
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "sys.h"

volatile void *
map_shared(char *name, int size)
{
    int fd;
    char *p;
    
    if((fd = shm_open(name, O_RDWR, 0777)) == -1) {
		rxerr("map_shared(): Cannot open shared memory area.");
		return((char *)-1);
    }

    if((p = mmap(0, size, PROT_READ|PROT_WRITE,
				MAP_SHARED, fd, 0)) == (char *)-1) {
		rxerr("map_shared(): Cannot map shared memory area.");
		return((char *)-1);
    }
    return(p);
}
