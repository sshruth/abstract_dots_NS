#include  <stdio.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <x86/inline.h>
#include  "rexhdrs.h"
#include  "dio_lab.h"
#include "../sset/ldev.h"

int dio_check_joybut() {
#ifdef JOYBUT
	   return dio_in(JOYBUT);
#else
       return 0;
#endif
}

/* This gets called in the interrupt routine, be careful */
int dio_check_touch() {
#ifdef TOUCH
	return dio_in(TOUCH);
#else
	return 0;
#endif
}

int dio_check_photodetector() {
#ifdef PHOTODETECTOR
	   //printf("Photodetector = %d\n",dio_in(PHOTODETECTOR));
	   return dio_in(PHOTODETECTOR);
#else
       return 0;
#endif
}
