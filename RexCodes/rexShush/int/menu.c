/* This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "sac.h"
#include "ramp.h"
#include "int.h"

/* null help message for menus */
char hm_null[] = "";

/*
 *-----------------------------------------------------------------------*
 * Nouns, menus, access functions for int.
 *
 * Note that for many of the menu variable-access functions below an
 * action corresponding to the menu variable is called with NULLI
 * as an argument.  The reason the action is called to make the change,
 * instead of having the menu system change the variable directly, is
 * due to critical sections between the upper and lower level.  The action
 * makes the change inside a section that is protected from interrupts.
 * Routines in both int.c and clock.c can interrupt the upper level
 * and cause problems.
 *-----------------------------------------------------------------------*
 */

/* 
 * Menu: "control_param"
 *-----------------------------------------------------------------------*
 */
#define CP_VL_SIZE 1000
char hm_cp_vl[CP_VL_SIZE];
#ifdef MEXWEX_UNITS
char hm_cp_mexwex[] = "\
\nmexwex_units-\n\
  0:	off\n\
  1:    comm ver 1\n\
  2:	comm ver 2";
#endif
#pragma off (unreferenced)
int
cp_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    char *hm= &hm_cp_vl;
    char *hm_end= &hm_cp_vl[CP_VL_SIZE];
    extern char hm_cp_proto_vl[];

    hm= stufs(hm_cp_proto_vl, hm, hm_end);
#if (!defined AD_CGLIST)
    if(ad_gain[0].ad_gain_factor != -1) {
		int i;
		char s_tmp[10];	

		hm= stufs("\n", hm, hm_end);
		hm= stufs("gains:\n  ", hm, hm_end);
		for(i= 0;;) {
			itoa_RL(ad_gain[i].ad_gain_factor, 'd', &s_tmp,
					&s_tmp[P_LNAME]);
			hm= stufs(s_tmp, hm, hm_end);
			if(ad_gain[++i].ad_gain_factor == -1) break;
			hm= stufs(", ", hm, hm_end);
		}
    }
#endif
#ifdef MEXWEX_UNITS
    hm= stufs(hm_cp_mexwex, hm, hm_end);
#endif
    hm= stufs("\n", hm, hm_end);
    return(0);
}

#pragma off (unreferenced)
int
cp_col_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    INT_BLOCK *ibp;

    if(flag & ME_BEF) {

		/*
		 * Set up global base pointer for this menu access.
		 */
		ibp= i_b;
		mp->me_basep= (unsign)ibp;
		return(0);
    } else return(0);
}

char hm_cp_proto_vl[] = "\
Unit window control-\n\
  -:	default or stateset\n\
  0:	units ignored\n\
  1:	units saved";

#pragma off (unreferenced)
int
cp_aw_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	pre_post(NULLI, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
cp_uw_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	uw_set(NULLI);
	return(0);
}

#if (!defined AD_CGLIST)
#pragma off (unreferenced)
int
cp_ad_ov_gain_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
    if(i_b->i_flags & I_GO) {
		rxerr("cp_adgain_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_ad_gain(-1, *tvadd));
}
#endif

#pragma off (unreferenced)
int
cp_ad_max_rate_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
    if(i_b->i_flags & I_GO) {
		rxerr("cp_ad_max_rate_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_ad_max_rate(-1, *tvadd));
}

#ifdef TC_UNITS
#pragma off (unreferenced)
int
cp_tc_units_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
    if(i_b->i_flags & I_GO) {
		rxerr("cp_tc_units_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(0);
}
#endif

#ifdef MEXWEX_UNITS
#pragma off (unreferenced)
int
cp_mexwex_units_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
    if(i_b->i_flags & I_GO) {
		rxerr("cp_mexwex_units_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    if(*tvadd < 0 || *tvadd > 2) {
		/*		rxerr("cp_mexwex_units_vaf: Illegal specification"); */
		return(-1);
    }
    return(0);
}
#endif

#ifdef SMI_TRACK
#pragma off (unreferenced)
int
cp_smi_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
    if(i_b->i_flags & I_GO) {
		rxerr("cp_smi_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(0);
}
#endif

#ifdef SMI_TRACK
VLIST smi_vl[] = {
NS,
};
#endif

/*
 * int_displays submenu menu struct.
 */
extern int errprt;
VLIST cp_vl[] = {
#if (!defined AD_CGLIST)
"ad_overall_gain",	&ad_ov_gain, NP, cp_ad_ov_gain_vaf, ME_BEF, ME_DEC,
#endif
"ad_max_rate",		&ad_max_rate, NP, cp_ad_max_rate_vaf, ME_BEF, ME_DEC,
"pre_time_msec",	&m_pre, NP, cp_aw_vaf, ME_AFT, ME_NVALD,
"post_time_msec",	&m_post, NP, cp_aw_vaf, ME_AFT, ME_NVALD,
"unit_wind_ctl",	&m_uwind, NP, cp_uw_vaf, ME_AFT, ME_NVALD,
"state_proc_rate",	&st_rate, NP, NP, 0, ME_DEC,
"debug_state",		&errprt, NP, NP, 0, ME_DEC,
"doutput_inmem",	&doutput_inmem, NP, NP, 0, ME_DEC,
"doutput_rotate",	&doutput_rot, NP, NP, 0, ME_DEC,
"doutput_tofile",	&doutput_tofile, NP, NP, 0, ME_DEC,
#ifdef TC_UNITS
"plexon_units",		&tc_units, NP, cp_tc_units_vaf, ME_BEF, ME_DEC,
#endif
#ifdef MEXWEX_UNITS
"mexwex_units",		&mexwex_units, NP, cp_mexwex_units_vaf, ME_BEF, ME_DEC,
#endif
#ifdef SMI_TRACK
"smi_on",		&smi_on, NP, cp_smi_vaf, ME_BEF, ME_DEC,
"smi_offset",		&smi_offset, NP, NP, 0, ME_DEC,
"smi_gain",			&smi_gain, NP, NP, 0, ME_DEC,
"smi_overrun",		&smi_overrun, NP, NP, 0, ME_DEC,
"smi_debtog",		&smi_debtog, NP, NP, 0, ME_DEC,
"smi_debtog2",		&smi_debtog2, NP, NP, 0, ME_DEC,
"smi_deblval",		&smi_deblval, NP, NP, 0, ME_DEC,
"smi_debhval",		&smi_debhval, NP, NP, 0, ME_DEC,
"v0",			&smi_counts[0], NP, NP, 0, ME_DEC,
"v1",			&smi_counts[1], NP, NP, 0, ME_DEC,
"v2",			&smi_counts[2], NP, NP, 0, ME_DEC,
"v3",			&smi_counts[3], NP, NP, 0, ME_DEC,
"v4",			&smi_counts[4], NP, NP, 0, ME_DEC,
"v5",			&smi_counts[5], NP, NP, 0, ME_DEC,
"v6",			&smi_counts[6], NP, NP, 0, ME_DEC,
"v7",			&smi_counts[7], NP, NP, 0, ME_DEC,
"v8",			&smi_counts[8], NP, NP, 0, ME_DEC,
"v9",			&smi_counts[9], NP, NP, 0, ME_DEC,
"later",		&smi_late, NP, NP, 0, ME_DEC,
#endif
NS,
};

/*
 * Menu: "signals"
 *-----------------------------------------------------------------------*
 */
#define SIG_VL_SIZE 1000
char hm_sig_vl[SIG_VL_SIZE];
static int signum;		/* signal number of current access */
#pragma off (unreferenced)
int
sig_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    int i;
    SIG *sigp;
    char *hm= &hm_sig_vl;
    char *hm_end= &hm_sig_vl[SIG_VL_SIZE];
    
    if(flag & ME_BEF) {

		/*
		 * Set up global base pointer for this menu access.
		 */
		if(*astr == '\0') signum= 0;
		else signum= atoi(astr);
		if(signum < 0 || signum >= SA_MAXSIG) {
			/*			rxerr("sig_maf(): Bad signal number"); */
			return(-1);
		}
		sigp= &sig[signum];
		mp->me_basep= (unsign)sigp;

		/*
		 * Set up help message for this menu access.
		 */
		hm= stufs("enable- 0: off, 1: a/d, 2: mem", hm, hm_end);
		if(gv_ad_mem[0].gv_ptr != NULL) {
			hm= stufs("\na/d mem variables:\n  ", hm, hm_end);
			for(i= 0;;) {
				hm= stufs(gv_ad_mem[i].gv_name, hm, hm_end);
				if(gv_ad_mem[++i].gv_ptr == NULL) break;
				if(!(i % 4)) hm= stufs("\n  ", hm, hm_end);
				else hm= stufs("  ", hm, hm_end);
			}
		}
		hm= stufs("\nrates:\n  ", hm, hm_end);
		sig_compute_ad_rates();
		for(i= 0;;) {
			char s_tmp[10];
	    
			itoa_RL(ad_acq_rates[i].ad_acq_rate, 'd', &s_tmp,
					&s_tmp[P_LNAME]);
			hm= stufs(s_tmp, hm, hm_end);
			if(++i >= SA_SPREAD_LOG2+1) {
				hm= stufs(", -", hm, hm_end);
				break;
			}
			hm= stufs(", ", hm, hm_end);
		}
		hm= stufs("\ncalib (deg FS)- 0: 204, 1: 102", hm, hm_end);
		hm= stufs("\n  2: 51, 3: 25, 4: 12, 5: 6", hm, hm_end);
		hm= stufs("\n  6: Noise test for 16 bit a/d", hm, hm_end);
#if (defined AD_CGLIST)
		if(ad_gain[0].ad_gain_factor != -1) {
			char s_tmp[10];	

			hm= stufs("\ngains:\n  ", hm, hm_end);
			for(i= 0;;) {
				itoa_RL(ad_gain[i].ad_gain_factor, 'd', &s_tmp,
						&s_tmp[P_LNAME]);
				hm= stufs(s_tmp, hm, hm_end);
				if(ad_gain[++i].ad_gain_factor == -1) break;
				hm= stufs(", ", hm, hm_end);
			}
		}
#endif
		if(gv_mem_src[0].gv_ptr != NULL) {
			hm= stufs("\nnon-a/d memory sources:\n  ", hm, hm_end);
			for(i= 0;;) {
				hm= stufs(gv_mem_src[i].gv_name, hm, hm_end);
				if(gv_mem_src[++i].gv_ptr == NULL) break;
				if(!(i % 3)) hm= stufs("\n  ", hm, hm_end);
				else hm= stufs("  ", hm, hm_end);
			}
		}
		return(0);
    } else return(0);
}

/*
 * Signal menu root argument generation function.  The purpose of this
 * function is to generate ascii strings that correspond to the
 * argument strings that would be used to access all the signal menus.
 */
#pragma off (unreferenced)
int
sig_agf(int call_cnt, MENU *mp, char *astr)
{
#pragma on (unreferenced)

	if(call_cnt >= SA_MAXSIG) {

		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	} else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

/*
 * Variable access functions.
 */
#pragma off (unreferenced)
int
sig_sig_enable_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_sig_enable_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_sig_enable(signum, *tvadd));
}

#pragma off (unreferenced)
int
sig_ad_chan_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_ad_chan_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_chan(signum, *tvadd));
}

#pragma off (unreferenced)
int
sig_ad_mem_var_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_ad_mem_var_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_ad_mem_var(signum, (char *)tvadd));
}
 
#pragma off (unreferenced)
int
sig_ad_acq_rate_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_ad_acq_rate_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_ad_acq_rate(signum, *tvadd));
}
 
#pragma off (unreferenced)
int
sig_ad_calib_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_ad_calib_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_ad_calib(signum, *tvadd));
}
 
#if (defined AD_CGLIST)
#pragma off (unreferenced)
int
sig_ad_gain_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_ad_gain_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_ad_gain(signum, *tvadd));
}
#endif

#pragma off (unreferenced)
int
sig_mem_src_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_mem_src_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_mem_src_var(signum, (char *)tvadd));
}
 
#pragma off (unreferenced)
int
sig_ad_delay_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_ad_delay_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(0);
}
 
#pragma off (unreferenced)
int
sig_sig_store_rate_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_sig_store_rate_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_sig_store_rate(signum, *tvadd));
}
 
#pragma off (unreferenced)
int
sig_sig_title_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

    if(i_b->i_flags & I_GO) {
		rxerr("sig_sig_title_vaf(): cannot change when clock is on");
		return(1);	    /* soft error- dont abort root reads */
    }
    return(sig_ck_sig_title_var(signum, (char *)tvadd));
}
 
VLIST sig_vl[] = {
"sig_enable",		&((SIG *)NP)->sig_enable, NP, sig_sig_enable_vaf,
			    ME_BEF|ME_GB, ME_DEC,
"ad_chan",		&((SIG *)NP)->sig_ad_chan, NP, sig_ad_chan_vaf,
			    ME_BEF|ME_GB, ME_NVALD,
"ad_mem_variable",	&((SIG *)NP)->sig_ad_mem_var, NP, sig_ad_mem_var_vaf,
			    ME_BEF|ME_GB, ME_STR,
"ad_acquire_rate",	&((SIG *)NP)->sig_ad_rate, NP, sig_ad_acq_rate_vaf,
			    ME_BEF|ME_GB, ME_NVALD,
"ad_calib",		&((SIG *)NP)->sig_ad_calib, NP, sig_ad_calib_vaf,
			    ME_BEF|ME_GB, ME_NVALD,
#if (defined AD_CGLIST)
"ad_gain",		&((SIG *)NP)->sig_ad_gain, NP, sig_ad_gain_vaf,
 			    ME_BEF|ME_GB, ME_NVALD,
#endif
"ad_delay_100_us",	&((SIG *)NP)->sig_ad_delay, NP, sig_ad_delay_vaf,
 			    ME_BEF|ME_GB, ME_NVALD,
"memory_source",	&((SIG *)NP)->sig_mem_source, NP, sig_mem_src_vaf,
			    ME_BEF|ME_GB, ME_STR,
"signal_store_rate",	&((SIG *)NP)->sig_store_rate, NP,
			    sig_sig_store_rate_vaf, ME_BEF|ME_GB, ME_NVALD,
"signal_title",		&((SIG *)NP)->sig_title, NP, sig_sig_title_vaf,
			    ME_BEF|ME_GB, ME_STR,
NS,
};

/*
 * Menu: "da_conv"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
da_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    int danum;
    DA *dap;

    if(flag & ME_BEF) {

		/*
		 * Set up global base pointer for this menu access.
		 */
		if(*astr == '\0') danum= 0;
		else danum= atoi(astr);
		if(danum < 0 || danum >= DA_MAXNUM) {
			/*			rxerr("da_maf(): Bad d/a number"); */
			return(-1);
		}
		dap= &da[danum];
		mp->me_basep= (unsign)dap;
		return(0);
    } else return(0);
}

/*
 * D/A menu root argument generation function.  The purpose of this
 * function is to generate ascii strings that correspond to the
 * argument strings that would be used to access all the D/A menus.
 */
#pragma off (unreferenced)
int
da_agf(int call_cnt, MENU *mp, char *astr)
{
#pragma on (unreferenced)

	if(call_cnt >= DA_MAXNUM) {

		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	} else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

#pragma off (unreferenced)
int
da_menu_enab_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	int danum;

	if(*tvadd == 0) return(0);
	if(*astr == '\0') danum= 0;
	else danum= atoi(astr);
	da_cntrl_1(danum, NULLI, NULLI);
	da_mode(danum, NULLI);
	da_offset(danum, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
da_cntrl_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	int danum;

	if(*astr == '\0') danum= 0;
	else danum= atoi(astr);
	da_cntrl_1(danum, NULLI, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
da_mode_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	int danum;

	if(*astr == '\0') danum= 0;
	else danum= atoi(astr);
	da_mode(danum, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
da_offset_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	int danum;

	if(*astr == '\0') danum= 0;
	else danum= atoi(astr);
	da_offset(danum, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
da_val_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	int danum;

	if(*astr == '\0') danum= 0;
	else danum= atoi(astr);
	da_set_1(danum, NULLI);
	return(0);
}

VLIST da_vl[] = {
"menu_enab",		&((DA *)NP)->m_da_menu, NP, da_menu_enab_vaf,
			    ME_AFT|ME_GB, ME_DEC,
"source",		&((DA *)NP)->m_da_source, NP, da_cntrl_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"source_num",		&((DA *)NP)->m_da_src_num, NP, da_cntrl_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"mode",			&((DA *)NP)->m_da_mode, NP, da_mode_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"offset",		&((DA *)NP)->m_da_offset, NP, da_offset_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"output_value",		&((DA *)NP)->m_da_val, NP, da_val_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
NS,
};

char hm_da_vl[] = "\
source-\n\
  0: standby\n\
     (not connected)\n\
  1: ramp_x\n\
  2: ramp_y\n\
  3: joystick_x\n\
  4: joystick_y\n\
  5: win_pos_x\n\
  6: win_pos_y\n\
  7: signal\n\
  8: mem_array\n\
mode (hex flag)-\n\
  0x0: direct\n\
  0x1: offset_conn\n\
  0x2: offset_stby";

/*
 * Menu: "mem_array"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
ma_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    int manum;
    MA *map;

    if(flag & ME_BEF) {

		/*
		 * Set up global base pointer for this menu access.
		 */
		if(*astr == '\0') manum= 0;
		else manum= atoi(astr);
		if(manum < 0 || manum >= MA_MAXNUM) {
			/*			rxerr("ma_maf(): Bad memory array number"); */
			return(-1);
		}
		map= &ma[manum];
		mp->me_basep= (unsign)map;
		return(0);
    } else return(0);
}

/*
 * Memory array menu root argument generation function.  The purpose of this
 * function is to generate ascii strings that correspond to the
 * argument strings that would be used to access all the memory array menus.
 */
#pragma off (unreferenced)
int
ma_agf(int call_cnt, MENU *mp, char *astr)
{
#pragma on (unreferenced)

	if(call_cnt >= MA_MAXNUM) {

		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	} else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

#pragma off (unreferenced)
int
ma_menu_enab_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	int manum;

	if(*tvadd == 0) return(0);
	if(*astr == '\0') manum= 0;
	else manum= atoi(astr);
	ma_cntrl(manum, (int *)NULLI, NULLI, NULLI, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
ma_cntrl_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	int manum;

	if(*astr == '\0') manum= 0;
	else manum= atoi(astr);
	ma_cntrl(manum, (int *)NULLI, NULLI, NULLI, NULLI);
	return(0);
}

VLIST ma_vl[] = {
"menu_enab",		&((MA *)NP)->m_ma_menu, NP, ma_menu_enab_vaf,
			    ME_AFT|ME_GB, ME_DEC,
"beg_add_pointer",	&((MA *)NP)->m_ma_bap, NP, ma_cntrl_vaf,
			    ME_AFT|ME_GB, ME_NVALH,
"count",		&((MA *)NP)->m_ma_count, NP, ma_cntrl_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"rate",			&((MA *)NP)->m_ma_rate, NP, ma_cntrl_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"repeat",		&((MA *)NP)->m_ma_repeat, NP, ma_cntrl_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
NS,
};

char hm_ma_vl[] = "\
count: number\n\
  of points in\n\
  array\n\
rate: msec/point\n\
repeat-\n\
   -1: continuous\n\
  1-n: number of\n\
       repeats";

/*
 * Menu: "offsets_eye"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
off_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	off_eye(NULLI, NULLI);
	off_oeye(NULLI, NULLI);
	return(0);
}

VLIST off_vl[] = {
"off_menu_enab",	&m_offmenu, NP, off_vaf, ME_AFT, ME_DEC,
"eyeh_off_10s",		&m_eho, NP, off_vaf, ME_AFT, ME_NVALD,
"eyev_off_10s",		&m_evo, NP, off_vaf, ME_AFT, ME_NVALD,
"oeyeh_off_10s",	&m_oeho, NP, off_vaf, ME_AFT, ME_NVALD,
"oeyev_off_10s",	&m_oevo, NP, off_vaf, ME_AFT, ME_NVALD,
NS,
};

char hm_off_vl[] = "\
Offsets in tenths\n\
of degree.  Value\n\
of 0 yields no\n\
offset.";

/*
 * Menu: "rgen(ramp)"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
ra_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    int rnum;
    RAMP *rap;

    if(flag & ME_BEF) {

		/*
		 * Set up global base pointer for this menu access.
		 */
		if(*astr == '\0') rnum= 0;
		else rnum= atoi(astr);
		if(rnum < 0 || rnum >= RA_MAXNUM) {
			/*			rxerr("rg_maf(): Bad ramp number"); */
			return(-1);
		}
		rap= &ramp[rnum];
		mp->me_basep= (unsign)rap;
		return(0);
    } else return(0);
}

/*
 * Ramp menu root argument generation function.  The purpose of this
 * function is to generate ascii strings that correspond to the
 * argument strings that would be used to access all the ramp menus.
 */
#pragma off (unreferenced)
int
ra_agf(int call_cnt, MENU *mp, char *astr)
{
#pragma on (unreferenced)

	if(call_cnt >= RA_MAXNUM) {

		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	} else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

#pragma off (unreferenced)
int
ra_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	int rnum;

	if(*astr == '\0') rnum= 0;
	else rnum= atoi(astr);
	if(rnum < 0 || rnum >= RA_MAXNUM) {
		/*	    rxerr("rg_vaf(): Bad ramp number"); */
		return(-1);
	}
	return(ra_new(rnum, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI, NULLI));
}

VLIST ra_vl[] = {
"menu_enab",		&((RAMP *)NP)->m_ra_menu, NP, ra_vaf, ME_AFT|ME_GB, ME_DEC,
"type_flag",		&((RAMP *)NP)->m_ra_type, NP, ra_vaf, ME_AFT|ME_GB, ME_NVALO,
"length_10s",		&((RAMP *)NP)->m_ra_len, NP, ra_vaf, ME_AFT|ME_GB, ME_NVALD,
"angle_deg",		&((RAMP *)NP)->m_ra_ang, NP, ra_vaf, ME_AFT|ME_GB, ME_NVALD,
"velocity_deg/sec",	&((RAMP *)NP)->m_ra_vel, NP, ra_vaf, ME_AFT|ME_GB, ME_NVALD,
"xoffset_10s",		&((RAMP *)NP)->m_ra_xoff, NP, ra_vaf, ME_AFT|ME_GB, ME_NVALD,
"yoffset_10s",		&((RAMP *)NP)->m_ra_yoff, NP, ra_vaf, ME_AFT|ME_GB, ME_NVALD,
"ramp_ecode",		&((RAMP *)NP)->m_ra_ecode, NP, ra_vaf, ME_AFT|ME_GB, ME_NVALD,
"phidevice",		&((RAMP *)NP)->ra_phidev, NP, NP, ME_GB, ME_LHEX,
"phiontime",		&((RAMP *)NP)->ra_pontime, NP, NP, ME_GB, ME_DEC,
"phiofftime",		&((RAMP *)NP)->ra_pofftime, NP, NP, ME_GB, ME_DEC,
NS,
};

char hm_ra_vl[] = "\
type_flag-\n\
  00: default- CENPT\n\
  01: NO25MS\n\
  02: CENPT\n\
  04: BEGPT\n\
 010: ENDPT";

/*
 * Menu: "run_line_chan"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
rl_chan_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    int chan;
    RL_CHAN *rcp;

    if(flag & ME_BEF) {

		/*
		 * Set up global base pointer for this menu access.
		 */
		if(*astr == '\0') chan= 0;
		else chan= atoi(astr);
		if(chan < 0 || chan >= RL_MAXCHAN) {
			/*			rxerr("rl_chan_maf(): Bad rl channel number"); */
			return(-1);
		}

		rcp= &i_b->rl.rl_chan[chan];

		mp->me_basep= (unsign)rcp;
		return(0);
    } else return(0);
}

#pragma off (unreferenced)
int
rl_chan_agf(int call_cnt, MENU *mp, char *astr)
{
#pragma on (unreferenced)

	if(call_cnt >= RL_MAXCHAN) {

		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	} else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

#pragma off (unreferenced)
int
rl_chan_sig_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
    RL_CHAN *rcp;

    if(flag & ME_BEF) {
		/* check to see if this is a channel is plotting a valid signal */
		if(*tvadd < 0 || *tvadd >= SA_MAXSIG) {
			/*			rxerr("rl_chan_sig_vaf(): Illegal signal number"); */
			return(-1);
		}
		if(!sig[*tvadd].sig_enable) {
			/*			rxerr("rl_chan_sig_vaf(): Signal is not enabled"); */
			return(-1);
		}
		return(0);
    } else {
		rcp = (RL_CHAN *)mp->me_basep;

		/* begin critical section */		
		InterruptDisable();
		{
			rcp->rl_addr= sa_gvptr[rcp->rl_signal];
			rcp->rl_sig_gvname = &samp.sa_gvname[rcp->rl_signal][0];
			rcp->rl_sig_title =  &samp.sa_title[rcp->rl_signal][0];
		}
		InterruptEnable();
		/* end critical section */		
		return(0);
    }
}

VLIST rl_chan_vl[] = {
"signal_num",		&((RL_CHAN *)NP)->rl_signal, NP, rl_chan_sig_vaf,
			    ME_BEF|ME_AFT|ME_GB, ME_DEC,
NS,
};

char hm_rl_chan[] = "\
erase_flag:\n\
  0: old trace is not\n\
     erased when new\n\
     trace is plotted";

/*
 * Menu: "win_disp_chan"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
wdc_chan_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    int chan;
	WIN_CURSOR *wcp;

    if(flag & ME_BEF) {

		/*
		 * Set up global base pointer for this menu access.
		 */
		if(*astr == '\0') chan= 0;
		else chan= atoi(astr);
		if(chan < 0 || chan >= WDI_NDATACURS) {
			/*			rxerr("wd_chan_maf(): Bad wd channel number"); */
			return(-1);
		}

		wcp= &i_b->cursTyp.datCurs[chan];

		mp->me_basep= (unsign)wcp;
		return(0);
    } else return(0);
}

#pragma off (unreferenced)
int
wdc_chan_agf(int call_cnt, MENU *mp, char *astr)
{
#pragma on (unreferenced)

	if(call_cnt >= WDI_NDATACURS) {

		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	} else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

#pragma off (unreferenced)
int
wdc_chan_sig_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	WIN_CURSOR *wcp;

	if(flag & ME_BEF) {
		if((*tvadd < 0) || (*tvadd >= WDI_NCHANTYPE)) {
			/*			rxerr("wdc_chan_sig_vaf(): illegal signal number"); */
			return(-1);
		}
		return(0);
	}
	else {
		wcp = (WIN_CURSOR *)mp->me_basep;
		/* begin critical section */		
		InterruptDisable();
		{
			wcp->type = *tvadd;
		}
		InterruptEnable();
		/* end critical section */		
		return(0);
	}
}

VLIST wdc_chan_vl[] = {
	"channel_num",	&((WIN_CURSOR *)NP)->type, NP, wdc_chan_sig_vaf,
	ME_BEF|ME_AFT|ME_GB, ME_DEC,
	NS,
};

char hm_wdc_vl[] = "";

/*
 * Menu: "saccade_det"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
sd_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)

	sd_set(NULLI);
	return(0);
}

VLIST sd_vl[] = {
"sd_control",		&m_sdctl, NP, sd_vaf, ME_AFT, ME_NVALO,
"h_range",		&limh.range, NP, NP, 0, ME_DEC,
"v_range",		&limv.range, NP, NP, 0, ME_DEC,
"peak_vel",		&peakvel, NP, NP, 0, ME_DEC,
"start_vel",		&velstart, NP, NP, 0, ME_DEC,
"end_vel",		&velend, NP, NP, 0, ME_DEC,
"max_vel",		&velmax, NP, NP, 0, ME_DEC,
"min_vel",		&velmin, NP, NP, 0, ME_DEC,
"max_dur",		&durmax, NP, NP, 0, ME_DEC,
"min_dur",		&durmin, NP, NP, 0, ME_DEC,
"delay_time",		&delay_time, NP, NP, 0, ME_DEC,
NS,
};

char hm_sd_vl[] = "\
sd_cont-\n\
  0: off\n\
  1: on";

/*
 * Menu: "bit"
 *-----------------------------------------------------------------------*
 */
VLIST bit_vl[] = {
"default_bit_device",	&m_bitdio, NP, NP, 0, ME_HEX,
NS,
};

char hm_bit_vl[] = "\
Default device id used\n\
for commands to set, clr\n\
the noun 'bit'";

/*
 * Menu: "trials"
 *-----------------------------------------------------------------------*
 */
VLIST tt_vl[] = {
"correct",		&tyes, NP, NP, 0, ME_DEC,
"wrong",		&tno, NP, NP, 0, ME_DEC,
NS,
};

/*
 * Menu: "windows"
 *-----------------------------------------------------------------------*
 */
#pragma off (unreferenced)
int
wd_maf(int flag, MENU *mp, char *astr, ME_RECUR *rp)
{
#pragma on (unreferenced)
    int wdnum;
    WD *wdp;

    if(flag & ME_BEF) {

	/*
	 * Set up global base pointer for this menu access.
	 */
	if(*astr == '\0') wdnum= 0;
	else wdnum= atoi(astr);
	if(wdnum < 0 || wdnum >= WD_MAXNUM) {
		/*	    rxerr("wd_maf(): Bad window number"); */
	    return(-1);
	}

	wdp= &i_b->wd[wdnum];

	mp->me_basep= (unsign)wdp;
	return(0);
    } else return(0);
}

/*
 * Window menu root argument generation function.  The purpose of this
 * function is to generate ascii strings that correspond to the
 * argument strings that would be used to access all the window menus.
 */
#pragma off (unreferenced)
int
wd_agf(int call_cnt, MENU *mp, char *astr)
{
#pragma on (unreferenced)

	if(call_cnt >= WD_MAXNUM) {

		/*
		 * Done.  Return null to terminate writing of root for
		 * this menu.
		 */
		*astr= '\0';
	} else itoa_RL(call_cnt, 'd', astr, &astr[P_ISLEN]);
	return(0);
}

#pragma off (unreferenced)
int
wd_menu_enab_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	int wdnum;

	if(*tvadd == 0) return(0);
	if(*astr == '\0') wdnum= 0;
	else wdnum= atoi(astr);
	wd_cntrl(wdnum, NULLI);
	wd_src_pos(wdnum, NULLI, NULLI, NULLI, NULLI);
	wd_src_check(wdnum, NULLI, NULLI, NULLI, NULLI);
	wd_siz(wdnum, NULLI, NULLI);
	wd_pos(wdnum, NULLI, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
wd_cntrl_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	int wdnum;

	if(*astr == '\0') wdnum= 0;
	else wdnum= atoi(astr);
	wd_cntrl(wdnum, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
wd_pos_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	int wdnum;

	if(*astr == '\0') wdnum= 0;
	else wdnum= atoi(astr);
	wd_src_pos(wdnum, NULLI, NULLI, NULLI, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
wd_check_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	int wdnum;

	if(*astr == '\0') wdnum= 0;
	else wdnum= atoi(astr);
	wd_src_check(wdnum, NULLI, NULLI, NULLI, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
wd_dirpos_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	int wdnum;

	if(*astr == '\0') wdnum= 0;
	else wdnum= atoi(astr);
	wd_pos(wdnum, NULLI, NULLI);
	return(0);
}

#pragma off (unreferenced)
int
wd_siz_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
#pragma on (unreferenced)
	int wdnum;

	if(*astr == '\0') wdnum= 0;
	else wdnum= atoi(astr);
	wd_siz(wdnum, NULLI, NULLI);
	return(0);
}

VLIST wdm_vl[] = {
"menu_enab",		&((WD *)NP)->m_wd_menu, NP, wd_menu_enab_vaf,
			    ME_AFT|ME_GB, ME_DEC,
"window_control",	&((WD *)NP)->m_wd_cntrl, NP, wd_cntrl_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"x_position_source",	&((WD *)NP)->m_wd_xpos_source, NP, wd_pos_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"x_position_src_num",	&((WD *)NP)->m_wd_xpos_src_num, NP, wd_pos_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"y_position_source",	&((WD *)NP)->m_wd_ypos_source, NP, wd_pos_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"y_position_src_num",	&((WD *)NP)->m_wd_ypos_src_num, NP, wd_pos_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"x_check_source",	&((WD *)NP)->m_wd_xcheck_source, NP, wd_check_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"x_check_src_num",	&((WD *)NP)->m_wd_xcheck_src_num, NP, wd_check_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"y_check_source",	&((WD *)NP)->m_wd_ycheck_source, NP, wd_check_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"y_check_src_num",	&((WD *)NP)->m_wd_ycheck_src_num, NP, wd_check_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"x_dir_pos_10s",	&((WD *)NP)->m_wd_xdirpos, NP, wd_dirpos_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"y_dir_pos_10s",	&((WD *)NP)->m_wd_ydirpos, NP, wd_dirpos_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"xsiz_10s",		&((WD *)NP)->m_wd_xsiz, NP, wd_siz_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
"ysiz_10s",		&((WD *)NP)->m_wd_ysiz, NP, wd_siz_vaf,
			    ME_AFT|ME_GB, ME_NVALD,
NS,
};

char hm_wd_vl[] = "\
window_control-\n\
  0: off\n\
  1: on\n\
sources-\n\
  0: dir pos\n\
  1: ramp_x\n\
  2: ramp_y\n\
  3: joystick_x\n\
  4: joystick_y\n\
  5: d/a\n\
  6: signal\n\
  7: mem_array";

/*
 *-----------------------------------------------------------------------*
 * Top level menu table.
 *-----------------------------------------------------------------------*
 */
extern VLIST state_vl[];
MENU menus[] = {
	"control_param",	&cp_vl, NP, cp_maf, ME_BEF, NP, hm_cp_vl,
	"signals",		&sig_vl, NP, sig_maf, ME_BEF, sig_agf, hm_sig_vl,
	"da_conv",		&da_vl, NP, da_maf, ME_BEF, da_agf, hm_da_vl,
	"mem_array",		&ma_vl, NP, ma_maf, ME_BEF, ma_agf, hm_ma_vl,
	"offsets_eye",		&off_vl, NP, NP, 0, NP, hm_off_vl,
	"rgen(ramps)",		&ra_vl, NP, ra_maf, ME_BEF, ra_agf, hm_ra_vl,
	"run_line_chan",	&rl_chan_vl, NP, rl_chan_maf, ME_BEF, rl_chan_agf, hm_rl_chan,
	"win_disp_chan",	&wdc_chan_vl, NP, wdc_chan_maf, ME_BEF, wdc_chan_agf, hm_wdc_vl,
	"saccade_det",		&sd_vl, NP, NP, 0, NP, hm_sd_vl,
	"bit",			&bit_vl, NP, NP, 0, NP, hm_bit_vl,
	"trials",		&tt_vl, NP, NP, 0, NP, NS,
	"windows",		&wdm_vl, NP, wd_maf, ME_BEF, wd_agf, hm_wd_vl,
	NS,
};
