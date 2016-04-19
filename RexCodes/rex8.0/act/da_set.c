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
 * Actions supporting D/A converters.
 */
#include <stdio.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ramp.h"
#include "int.h"

int da_valid_true= 1;

/*
 * Control d/a's.  Note: this is *not* an action and cannot be called
 * from the state set becuase the arguments are not longs.
 * Uses far pointers because it can be called from the interrupt level
 * with pointers to storage on the stack.
 */
static char da_cntrl_snum_err[]= "da_cntrl(): bad source number specifier";

int da_cntrl(int cnt, DA_CNTRL_ARG *ap)
{
	DA *dap;
	int menu;
	char *err= NP;
	int tmp_source, tmp_src_num, danum;
	unsigned eflags;

	/*
	 * Change d/a parameters in loop with interrupts locked out so that
	 * changes for all d/a's will occur as simultaneously as possible.
	 */
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	for(; cnt > 0; cnt--, ap++) {
		danum= ap->da_cntrl_num;
		if(danum < 0 || danum >= DA_MAXNUM) {
			err= "da_cntrl(): Bad d/a number";
			goto done_err;
		}
		dap= &da[danum];
		menu= dap->m_da_menu;

		if(menu && (dap->m_da_source != NULLI)) tmp_source= dap->m_da_source;
		else if(ap->da_cntrl_src != NULLI) tmp_source= ap->da_cntrl_src;
		else tmp_source= dap->da_source;
		if(menu && (dap->m_da_src_num != NULLI)) tmp_src_num= dap->m_da_src_num;
		else if(ap->da_cntrl_src_num != NULLI) tmp_src_num= ap->da_cntrl_src_num;
		else tmp_src_num= dap->da_src_num;

		switch(tmp_source) {

		case DA_STBY:

			/*
			 * Unlink from active list.
			 */
			if(dap->da_back == NP) goto done;
			else {
				dap->da_back->da_for= dap->da_for;
				dap->da_for->da_back= dap->da_back;
				dap->da_back= NP;
			}
			goto done;

		case DA_RAMP_X:

			if(tmp_src_num < 0 || tmp_src_num >= RA_MAXNUM) {
				err= da_cntrl_snum_err;
				goto done_err;
			}
			dap->da_validp= &ramp[tmp_src_num].ra_x.ra_valid;
			dap->da_posp= &ramp[tmp_src_num].ra_x.ra_pos;
			break;

		case DA_RAMP_Y:

			if(tmp_src_num < 0 || tmp_src_num >= RA_MAXNUM) {
				err= da_cntrl_snum_err;
				goto done_err;
			}
			dap->da_validp= &ramp[tmp_src_num].ra_y.ra_valid;
			dap->da_posp= &ramp[tmp_src_num].ra_y.ra_pos;
			break;

		case DA_JOY_X:
    
			dap->da_validp= &da_valid_true;
			dap->da_posp= &joyh;
			break;

		case DA_JOY_Y:
    
			dap->da_validp= &da_valid_true;
			dap->da_posp= &joyv;
			break;

		case DA_WIND_X:
    
			if(tmp_src_num < 0 || tmp_src_num >= DA_MAXNUM) {
				err= da_cntrl_snum_err;
				goto done_err;
			}
			dap->da_validp= &da_valid_true;
			dap->da_posp= i_b->wd[tmp_src_num].wd_xposp;
			break;

		case DA_WIND_Y:
    
			if(tmp_src_num < 0 || tmp_src_num >= DA_MAXNUM) {
				err= da_cntrl_snum_err;
				goto done_err;
			}
			dap->da_validp= &da_valid_true;
			dap->da_posp= i_b->wd[tmp_src_num].wd_yposp;
			break;

		case DA_SIGNAL:

			if(tmp_src_num < 0 || tmp_src_num >= SA_MAXSIG) {
				err= da_cntrl_snum_err;
				goto done_err;
			}
			dap->da_validp= &da_valid_true;
			dap->da_posp= sa_gvptr[tmp_src_num];
			break;
	
		case DA_MEM:

			if(tmp_src_num < 0 || tmp_src_num >= MA_MAXNUM) {
				err= da_cntrl_snum_err;
				goto done_err;
			}
			dap->da_validp= &ma[tmp_src_num].ma_valid;
			dap->da_posp= &ma[tmp_src_num].ma_cur_val;
			break;

		default:
			err= "da_cntrl(): Bad source specifier";
			goto done_err;
		}

		/*
		 * Link da into active list at end.
		 */
		if(dap->da_back == NP) {
			dap->da_back= da_last.da_back;
			dap->da_back->da_for= dap;
			dap->da_for= &da_last;
			da_last.da_back= dap;
		}

	done:
		dap->da_source= tmp_source;
		dap->da_src_num= tmp_src_num;
	}

	/*
	 * Cannot call rxerr when interrupts are disabled.
	 */
 done_err:
	if(eflags & INTR_IF) InterruptEnable();
	if(err != NP) rxerr(err);
	return(0);
}

int Pda_cntrl(int *cnt, DA_CNTRL_ARG *ap)
{
	da_cntrl(*cnt, ap);
	return(0);
}

/*
 * Simple actions to call 'da_cntrl()' with one or two arguments.
*/
int
da_cntrl_1(long danum, long source, long src_num)
{
    DA_CNTRL_ARG args;

    args.da_cntrl_num= danum;
    args.da_cntrl_src= source;
    args.da_cntrl_src_num= src_num;
    da_cntrl(1, &args);
    return(0);
}

int Pda_cntrl_1(long *danum, long *source, long *src_num)
{
	da_cntrl_1(*danum, *source, *src_num);
	return(0);
}

int
da_cntrl_2(long danum0, long source0, long src_num0,
long danum1, long source1, long src_num1)
{
    DA_CNTRL_ARG args[2];

    args[0].da_cntrl_num= danum0;
    args[0].da_cntrl_src= source0;
    args[0].da_cntrl_src_num= src_num0;
    args[1].da_cntrl_num= danum1;
    args[1].da_cntrl_src= source1;
    args[1].da_cntrl_src_num= src_num1;
    da_cntrl(2, &args);
    return(0);
}

int Pda_cntrl_2(long *danum0, long *source0, long *src_num0,
				long *danum1, long *source1, long *src_num1)
{
	da_cntrl_2(*danum0, *source0, *src_num0, *danum1, *source1,  *src_num1);
	return(0);
}

int
da_mode(long danum, long mode)
{
    DA *dap;
    int menu;
    unsigned eflags;

    if((danum < 0) || (danum >= DA_MAXNUM)) {
		rxerr("da_mode(): Bad d/a number");
		return(0);
    }
    dap= &da[danum];
    menu= dap->m_da_menu;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
    if(menu && (dap->m_da_mode != NULLI)) dap->da_mode=	dap->m_da_mode;
    else if(mode != NULLI) dap->da_mode= mode;
	if(eflags & INTR_IF) InterruptEnable();

    return(0);
}

int Pda_mode(long *danum, long *mode)
{
	da_mode(*danum, *mode);
	return(0);
}

/*
 * Send output to  d/a's.  Note: this is *not* an action and cannot be called
 * from the state set because the arguments are not longs.
 */
int da_set(int cnt, DA_SET_ARG *ap)
{
	DA *dap;
	int menu;
	DIO_DA_ARG args[DA_MAXNUM];
	DIO_DA_ARG *diop;
	int i, danum, val;
	char *err= NP;
	unsigned eflags;

	if(cnt > DA_MAXNUM) {
		rxerr("da_set(): cannot have count > DA_MAXNUM");
		return(0);
	}

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	for(i= cnt, diop= &args[0]; i > 0; i--, ap++, diop++) {
		danum= ap->da_set_num;
		if(danum < 0 || danum >= DA_MAXNUM) {
			err= "da_set(): Bad d/a number";
			goto done_err;
		}
		dap= &da[danum];
		menu= dap->m_da_menu;

		if(menu && (dap->m_da_val != NULLI)) val= dap->m_da_val << TEN_TO_IC;
		else if(ap->da_set_val != NULLI) val= ap->da_set_val << TEN_TO_IC;
		else {

			/*
			 * Nothing to do.
			 */
		skip:
    
			diop--;
			cnt--;
			continue;
		}
		if(dap->da_mode & DA_OFFSET_STBY) val += dap->da_offset;
		if(val == dap->da_curval) goto skip;    /* don't output unnecessarily */
		diop->dio_da_id= dap->da_id;
		diop->dio_da_value= val;
		diop->dio_da_flag= 0;
		diop->dio_da_curvalp= &dap->da_curval;
	}
	if(eflags & INTR_IF) InterruptEnable();   /* 1 */
	if(cnt) dio_da(cnt, &args[0]);   /* update all d/a's at (nearly) same time */
	return(0);

	/*
	 * Cannot call rxerr when interrupts are disabled.
	 */
 done_err:
	if(eflags & INTR_IF) InterruptEnable();  /* 2 */
	if(err != NP) rxerr(err);
	return(0);
}

int Pda_set(int *cnt, DA_SET_ARG *ap)
{
	da_set(*cnt, ap);
	return(0);
}

/*
 * Simple actions to call 'da_set()' with one or two arguments.
 */
int
da_set_1(long danum, long val)
{
    DA_SET_ARG args;

    args.da_set_num= danum;
    args.da_set_val= val;
    da_set(1, &args);
    return(0);
}

int Pda_set_1(long *danum, long *val)
{
	da_set_1(*danum, *val);
	return(0);
}

int
da_set_2(long danum0, long val0, long danum1, long val1)
{
    DA_SET_ARG args[2];

    args[0].da_set_num= danum0;
    args[0].da_set_val= val0;
    args[1].da_set_num= danum1;
    args[1].da_set_val= val1;
    da_set(2, &args);
    return(0);
}

int Pda_set_2(long *danum0, long *val0, long *danum1, long *val1)
{
	da_set_2(*danum0, *val0, *danum1, *val1);
	return(0);
}

int
da_offset(long danum, long val)
{
    DA *dap;
    int menu;
    unsigned eflags;

    if(danum < 0 || danum >= DA_MAXNUM) {
		rxerr("da_offset(): Bad d/a number");
		return(0);
    }
    dap= &da[danum];
    menu= dap->m_da_menu;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
    if(menu && (dap->m_da_offset != NULLI)) dap->da_offset=
		dap->m_da_offset << TEN_TO_IC;
    else if(val != NULLI) dap->da_offset= val << TEN_TO_IC;
	if(eflags & INTR_IF) InterruptEnable();

    return(0);
}

int Pda_offset(long *danum, long *val)
{
	da_offset(*danum, *val);
	return(0);
}

/*
 * Specify da pair for the d/a cursors.
 */
int
da_cursor(long da_x, long da_y, long cursor)
{
    char *err= NP;
    unsigned eflags;

    if(da_x != NULLI) if(da_x < 0 || da_x >= DA_MAXNUM) {
		rxerr("da_cursor(): bad d/a number for xda");
		return(0);
    }
    if(da_y != NULLI) if(da_y < 0 || da_y >= DA_MAXNUM) {
		rxerr("da_cursor(): bad d/a number for yda");
		return(0);
    }
    
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
    switch(cursor) {
    case CU_DA_ONE:
		if(m_da_cur_xda != NULLI) da_cur_xda_p= &da[m_da_cur_xda];
		else if(da_x != NULLI) da_cur_xda_p= &da[da_x];
		if(m_da_cur_yda != NULLI) da_cur_yda_p= &da[m_da_cur_yda];
		else if(da_y != NULLI) da_cur_yda_p= &da[da_y];
		break;
    case CU_DA_TWO:
		if(m_oda_cur_xda != NULLI) oda_cur_xda_p= &da[m_oda_cur_xda];
		else if(da_x != NULLI) oda_cur_xda_p= &da[da_x];
		if(m_oda_cur_yda != NULLI) oda_cur_yda_p= &da[m_oda_cur_yda];
		else if(da_y != NULLI) oda_cur_yda_p= &da[da_y];
		break;
    default:
		err= "da_cursor(): bad cursor specified";
		break;
    }
	if(eflags & INTR_IF) InterruptEnable();
    if(err != NP) rxerr(err);
    return(0);
}

int Pda_cursor(long *da_x, long *da_y, long *cursor)
{
	da_cursor(*da_x, *da_y, *cursor);
	return(0);
}

/*
 * Init and clear d/a converters.
 */
int
da_init(void)
{
    DA *dap;
    int i;

    da_first.da_back= NP;
    da_first.da_for= &da_last;
    da_last.da_back= &da_first;
    da_last.da_for= NP;
    for(i= 0, dap= &da[0]; dap < &da[DA_MAXNUM]; dap++, i++) {
		dap->m_da_menu= 0;
		dap->da_source= 0;
		dap->m_da_source= NULLI;
		dap->da_src_num= 0;
		dap->m_da_src_num= NULLI;
		dap->da_mode= 0;
		dap->m_da_mode= NULLI;
		dap->m_da_val= NULLI;
		dap->da_offset= 0;
		dap->m_da_offset= NULLI;
		dap->da_curval= MININT;	    /* init to something outrageous until first value is output to d/a */
		dap->da_validp= 0;
		dap->da_posp= NP;
		dap->da_for= NP;
		dap->da_back= NP;
		if(dap->da_clear) da_set_1(i, 0);
    }
    return(0);
}
