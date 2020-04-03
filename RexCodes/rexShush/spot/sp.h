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
 * Note: these defines cannot be changed without also changing
 * corresponding defines in /rex/hdr/state.h.
 */
#define MAXESC	    10	    /* max number of escapes */
#define MAXSTATE    1000    /* max number of states across all chains */
#define MAXCHAIN    50	    /* max number of chains */
#define MAXLEXBUF   30000   /* size of buf lex uses to store strings */

void	go(void);
void	ilist(void);
void	includes(void);
void	highhdr(void);
void	globals_names(void);
void	usercode(void);
void	states_names(void);
void	aborts(void);
void	inittable(void);
void	varlist(void);
