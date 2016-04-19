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
#include "tty.h"

/*
 * Terminal specific control sequences.  This version for VT100.
 * Fill char counts assume 4800 baud.
 * See tty.h for definitions.
 */
char tseltcur[]=	"\033[1K";
#ifdef T_VT100
char tscsav[]=		"\0337";
char tscres[]=		"\0338";
#endif
#ifdef T_Z29
char tscsav[]=		"\033[s";
char tscres[]=		"\033[u";
#endif
char tsecline[]=	"\033[2K";

/*
 * Set underline to do reverse video- nicer effect.
 */
/*
char tsulon[]=		"\033[4m";
char tsuloff[]=		"\033[m";
 */
char tsulon[]=		"\033[7m";
char tsuloff[]=		"\033[m";

  
char tshome[]=		"\033[f";
char tsrindex[]=	"\033M";
char tsecurtel[]=	"\033[K";
char tsansi[]=		"\033<";

/*
 * Erase in display strings need fill chars (even with XON/XOFF handshaking
 * enabled).
 */
#ifdef GOO
char tsebcur[]=		"\033[J\177\177\177\177\177\177\177\177\177\177\
\177\177\177\177\177\177\177\177\177\177\177\177\177";
char tseall[]=		"\033[2J\177\177\177\177\177\177\177\177\177\177\177\
\177\177\177\177\177\177\177\177\177\177\177\177";
#endif
char tsebcur[]=		"\033[J";
char tseall[]=		"\033[2J";
