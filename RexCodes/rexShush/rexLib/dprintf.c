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
 * Printing functions useful for debugging the lower level of int.
 * These routines dont call any library functions, and are safe to
 * call from the lower level.
 * These routines are called from interrupt level, and must be compiled
 * -Wc,-zu and -Wc,-s.
 */

#include <stdarg.h>
#include <sys/types.h>
#include "sys.h"

/*
 * Scaled down printf.  Recognizes following formats:
 *	%s- string	    %c- char
 *	%d- decimal	    %o- octal	    %u- unsigned	%x- hex
 */
void dprintf(char *fmt, ...)
{
	va_list ap;
#define DPF_SSIZ 100

/*
 * Pointers to local automatic storage must be far when routine is
 * called from interrupt level.
 */
	/* 	char far str[DPF_SSIZ]; */
	char str[DPF_SSIZ];
	int ival;
	char cval;
	char *sval;

	va_start(ap, fmt);
	for(;;) {
		while((cval= *fmt++) != '%') {
			if(cval == '\0') {
			    goto done;
			}
			dputchar(cval);
		}

		/*
		 * Format specifier.
		 */
		switch(*fmt) {

		case 'd':
		case 'o':
		case 'u':
		case 'x':
		        ival= va_arg(ap, int);
			
			/*
			 * Must call version that takes far pointers
			 * when called from interrupt level.
			 */
			itoa_RL(ival, *fmt, str, &str[DPF_SSIZ]);
			dputs(str);
			break;
		case 's':
		        sval= va_arg(ap, char *);
			dputs(sval);
			break;
		case 'c':
		        cval= va_arg(ap, char);
			dputchar(cval);
			break;
		default:
			dputs("Bad format spec. in dprintf\n");
		}
		fmt++;
	}
done:
	va_end(ap);
}
