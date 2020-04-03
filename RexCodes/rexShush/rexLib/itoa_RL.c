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

#include <sys/types.h>
#include "sys.h"

/*
 * Convert integer, long to ascii.  Accepts flags for decimal,
 * unsigned decimal, octal and hex.  Returns pointer to null termination.
 * This is done here rather than calling library routines so
 * that these routines can be called from the interrupt level.
 * These routines are called from interrupt level, and must be compiled
 * -Wc,-zu and -Wc,-s.
 */

char ascii_val[] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
};

/*
 * Flag: 'd' signed decimal, 'u' unsigned decimal, 'o' unsigned octal,
 * 'x' hex.
 *
 */

/*
char
*itoa_RL(int num, char flag, char *to, char *ep)
{
    return((char *)f_itoa_RL(num, flag, to, ep));
}
*/

/*
 * This version that takes far pointers is needed for when called
 * from interrupt level.
 */
/*
char far
*f_itoa_RL(int num, char flag, char far *to, char far *ep)
*/
char *itoa_RL(int num, char flag, char *to, char *ep)
{
/*
 * Pointers to local automatic storage must be far when routine is
 * called from interrupt level.
 */
	/*
	char far tmp[100];
  	char far *p= tmp;
	*/
	char tmp[100];
	char *p = tmp;
	int base, cnt= 0, negsign= 0, mv;

	switch(flag) {
	case 'd':
		/*
		 * Do decimal conversion with neg numbers so that -32768
		 * will be converted properly. (There is no +32768)
		 */
		base= -10;
		if(num >= 0) num = -num;
		else negsign= 1;
		break;
	case 'o':
		base= 8;
		break;
	case 'u':
		base= 10;
		break;
	case 'x':
		base= 16;
		break;
	default:
		flag= 'x';
		base= 16;
		break;
	}

	do {
	    if(flag == 'd') {
		if((mv= (num % base)) < 0) mv= -mv;
		*p++ = ascii_val[mv];
		num= num / base;
	    } else {
		*p++ = ascii_val[(((unsigned)num) % (unsigned)base)];
		num = ((unsigned)(num)) / (unsigned)base;
	    }
	    cnt++;	    
	} while(num);

	if(to >= ep) {
		to= ep;
		goto out;
	}
	if(negsign) {
		*p++ = '-';
		cnt++;
	}
	num= cnt;
	while(num--) {
		*to++ = *--p;

		/*
		 * Leave to pointing at ep when string is full.
		 */
		if(to == ep) {
			*(to - 1) = '\0';
			goto out;
		}
	}
	*to= '\0';

out:
	return(to);
}

/*
 * Flag: 'D' signed decimal, 'U' unsigned decimal, 'O' unsigned octal,
 * 'X' hex.
 */
/*
char *ltoa_RL(long num, char flag, char *to, char *ep)
{
     return((char *)f_ltoa_RL(num, flag, to, ep));
}
*/

/*
 * This version that takes far pointers is needed for when called
 * from interrupt level.
 */
/*
char far
*f_ltoa_RL(long num, char flag, char far *to, char far *ep)
*/

char *ltoa_RL(long num, char flag, char *to, char *ep)
{
/*
 * Pointers to local automatic storage must be far when routine is
 * called from interrupt level.
 */
	/*
	char far tmp[100];
	char far *p= &tmp[0];
	*/
	char tmp[100];
	char *p = &tmp[0];
	int base, cnt= 0, negsign= 0, mv;

	switch(flag) {
	case 'D':

		/*
		 * Do decimal conversion with neg numbers so that largest
		 * negative number (which has not positive counterpart in
		 * 2's complement) will be converted properly.
		 */
		base= -10;
		if(num >= 0) num = -num;
		else negsign= 1;
		break;
	case 'O':
		base= 8;
		break;
	case 'U':
		base= 10;
		break;
	case 'X':
		base= 16;
		break;
	default:
		flag= 'X';
		base= 16;
		break;
	}

	do {
	    if(flag == 'D') {
		if((mv= (num % (long)base)) < 0) mv= -mv;
		*p++ = ascii_val[mv];
		num= num / (long)base;
	    } else {
		*p++ = ascii_val[(((unsigned long)num) % (unsigned long)base)];
		num = ((unsigned long)(num)) /
					(unsigned long)base;
	    }
	    cnt++;	    
	} while(num);

	if(to >= ep) {
		to= ep;
		goto out;
	}
	if(negsign) {
		*p++ = '-';
		cnt++;
	}
	num= cnt;
	while(num--) {
		*to++ = *--p;

		/*
		 * Leave to pointing at ep when string is full.
		 */
		if(to == ep) {
			*(to - 1) = '\0';
			goto out;
		}
	}
	*to= '\0';

out:
	return(to);
}
