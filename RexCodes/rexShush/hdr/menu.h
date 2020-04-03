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
 * Header for REX menu system.
 */

/*
 * Each menu's variables are specified by an array of these structs.
 */
typedef char bool;
typedef struct {
	char	*vl_name;	/* Name of variable; list terminates when points
				   to null.  If this entry is a pointer 
				   to a submenu, vl_name is the name of the
				   submenu */
	void	*vl_add;	/* Address pointer of variable;  depending
				   on value of vl_flag may not be actual add
				   of variable.  If submenu, vl_add is pointer
				   to submenu's MENU struct.  */
	void	*vl_basep;	/* base pointer possibly added to address */
	int (*vl_accf)();	/* access function of variable; use determined
				   by vl_flag;  is only called before or after
				   a variable is changed, not when value of
				   variable is printed on screen or written to
				   root file */
	char	vl_flag;
	char	vl_type;	/* type of variable */
} VLIST;

/*
 * Each process' top level menus are described by an array of these structs
 * named menus[].  Submenus are also described by an instance of this struct
 * pointed to by a VLIST entry; however, submenu MENU's are not stored in the
 * top level array.
 */
typedef struct {
	char	*me_name;	/* Name of menu; list terminates when pointer
				   to null.  If this menu is a submenu,
				   name is ignored (submenu names are kept in
				   the VLIST entry that points to the submenu's
				   MENU). */
	VLIST	*me_vlp;	/* pointer to variable list for this menu */
	unsign	me_basep;	/* base pointer may be added to addresses of
				   menu variables; use determined by vl_flag */
	int (*me_accf)();	/* access function for menu;  use determined
				   by value of me_flag */
	int	me_flag;
	int (*me_rtagen)();	/* root arg generation function;  if this menu
				   requires args this function will generate
				   these args when writing roots */
	char 	*me_help;	/* help message printed alongside menu */
} MENU;
extern MENU menus[];		/* menu list for each process */
extern MENU umenus[];
extern char hm_null[];      /* null message */
/*
 * Flags.  Note which flag bits can be legally set in.
 */
#define ME_BEF		01	/* - me_flag or vl_flag -
				   call access func before accessing menu
				   or changing variable */
#define ME_AFT		02	/* - me_flag or vl_flag -
				   call access function after changing var */
#define ME_LB		04	/* - vl_flag only -
				   add local base pointer (vl_basep) to address
				   of variable */
#define ME_ILB		010	/* - vl_flag only -
				   vl_basep is address of value to be added to
				   address of var;  vl_basep is an indirect
				   pointer */
#define ME_GB		020	/* - vl_flag only -
				   add global base pointer (me_basep) to address
				   of variable */
#define ME_IGB		040	/* - vl_flag only -
				   me_basep is address of value to be added to
				   address of var */
#define ME_ROOT		0100	/* - neither me_flag or vl_flag -
				   set in flag passed to menu access function
				   (me_accf()) when writing a root file */

/*
 * Flags for vl_type.
 */
#define ME_ACHAR	0	/* variable is a single ascii char */
#define ME_OCT		1	/* variable is an octal integer */
#define ME_DEC		2	/* variable is a decimal integer*/
#define ME_HEX		3	/* variable is a hexidecimal integer */
#define ME_LOCT		4	/* variable is an octal long integer */
#define ME_LDEC		5	/* variable is a decimal long integer */
#define ME_LHEX		6	/* variable is a hexidecimal long integer */
#define ME_NVALO	7	/* variable has a null value (-MAXINT) which
						   when true will not override actual value in
						   memory and will be printed as '-';  variable
						   treated as octal */
#define ME_NVALD	8	/* null value variable treated as decimal */
#define ME_NVALH	9	/* null value variable treated as hex */
#define ME_NVALLO	10	/* null value long octal */
#define ME_NVALLD	11	/* null value long decimal */
#define ME_NVALLH	12	/* null value long hex */
#define ME_SUBMENU	13	/* variable entry is pointer to submenu */
#define ME_STR		14	/* variable is a string */
#define ME_UDCHAR	15	/* var is unsigned decimal char */
#define ME_NVALACTARG	16	/* special for printing args to actions from
							   state set;  prints as oct, dec, hex and
							   accepts all three bases as input */
#define ME_FLOAT	17	/* variable is a decimal float */

/*
 * Function 'access_m()' is used to change both menu variables and
 * state set variables.  Following struct is passed as an argument
 * to specify whether menu variables or state set variables are being
 * accessed.
 */
typedef struct {
	int	(*ap_pmenu)();	/* pointer to print menu function */
	int	(*ap_pline)();	/* pointer to print line function */
	int	ap_type;	/* type of access; see defines below */
} ME_ACCP;

/*
 * Menu system permits nested submenus.  Submenus are processed by
 * recursively calling menu system functions.  Sometimes, however,
 * the menu system needs access to info from previous recursions.
 * This struct is a linked list that contains this info.
 */
struct me_recur {
	struct me_recur	*me_rec_bp; /* back pointer to previous recursion;
				       NP terminates chain */
	MENU	*me_rec_menup;	    /* pointer to menu struct */
	char	*me_rec_astr;	    /* pointer to arg string */
};
typedef struct me_recur ME_RECUR;

/*
 * Type defines for ap_type above.  Currently only the menu system and
 * statelist variable changer use access().
 */
#define ME_MENU		0	/* menu access */
#define ME_SLIST	1	/* state list access */

/*
 * Globals used by all processes for roots.  Defined in sys/rlib/root.c.
 */
extern int	infd;		/* input file desc;  when != 0 contains root
				   file desc */
extern int	echo;		/* when 0 process doesnt echo to tty; used
				   when reading roots to prevent echo */
extern long	lastrseek;	/* last seek pointer in root file */


/*
 * Formats of calls to access functions and root arg generation function:
 *
 * Menu access function-
 *	me_accf(flag, MENU_pointer, argstring, RECUR_pointer)
 *	Return value of < 0 results in illegal access error if flag
 *	== ME_BEF.  Return value is ignored when flag == ME_AFT.
 *
 *	flag:		Menu access function can be called before and/or
 *			after accessing menu;  its value will be either
 *			ME_BEF, ME_BEF|ME_ROOT, or ME_AFT.
 *	MENU_pointer:	Pointer to MENU struct for this access.
 *	argstring:	Argument string of menu access command.
 *	RECUR_pointer:	Pointer to recursion info struct.
 *
 * Variable access function-
 *	vl_accf(flag, MENU_pointer, argstring, VLIST_pointer, tvadd)
 *	Return value != 0 when flag == ME_BEF results in variable being
 *	left unchanged;  if return value is negative error is printed
 *	(and a root read would be aborted), if value is positive no error
 *	is printed (a root read would continue).  If flag == ME_AFT
 *	return value is ignored.
 *
 *	flag:		ME_BEF if called before accessing menu variable;
 *			ME_AFT if called after.  Note that access function
 *			is not called when printing value of variable.
 *	MENU_pointer:	Pointer to MENU struct for this variable.
 *	argstring:	Argument string of menu access command for this access.
 *	VLIST_pointer:	Pointer to VLIST for this variable.
 *	tvadd:		Address of new value of variable in temporary
 *		        location.  If flag == BE_BEF the actual variable has
 *		        not yet been changed- therefore *tvadd points to
 *			the new value of the variable BEFORE it has actually
 *			been changed.
 *
 * Root argument generation function-
 *	me_rtagen(call_cnt, MENU_pointer, astr)
 *	Return value of < 0 results in error.
 *
 *	call_cnt:	Count increments each time me_rtagen() is called
 *			for a given menu during write to root file.  'call_
 *			cnt' starts at 0; me_rtagen() will continue to be called
 *			and additional menus written as long as it returns
 *			non-null astr's.
 *	MENU_pointer:	Pointer to MENU struct.
 *	astr:		Pointer to argument string of size P_ISLEN for
 *			me_rtagen() to generate.  As long as me_rtagen()
 *			generates arg strings on successive calls, mkroot()
 *			will attempt to write menus with these args strings
 *			to root file.
 */

/*
 * Prototypes.
 */
int access_m(ME_ACCP *ap, ME_RECUR *rp);
int mkroot(MENU *mp, char *argv[], VLIST *tvlp, ME_RECUR *rp, char *procName);
int pr_line(MENU *mp, VLIST *vlp, int num, FILE *file);
void sendname(PROCTBL_P pp, NOUN *np, NOUN *unp, MENU *mp, MENU *ump);
