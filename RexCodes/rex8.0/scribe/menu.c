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
 * Scribe nouns, menus.
 */

#include <sys/types.h>
#include <stdio.h>
#include "rexhdrs.h"
#include "scribe.h"

/*
 *	Scribe noun table.
 */
NOUN nouns[]= {
	{"Adata",	&n_adata},
	{"Edata",	&n_edata},
	{"Bdata",	&n_bdata},
	{"file",	&n_file},
	{"",		0},
};

VLIST sc_vl[] = {
"doutput_inmem",	&doutput_inmem, NP, NP, 0, ME_DEC,
"doutput_rotate",	&doutput_rot, NP, NP, 0, ME_DEC,
"doutput_tofile",	&doutput_tofile, NP, NP, 0, ME_DEC,
NS,
};

char hm_sc_vl[]= "";

/*
 * Menus.
 */
MENU menus[]= {
"scribe",		&sc_vl, NP, NP, 0, NP, hm_sc_vl,
NS,
};
