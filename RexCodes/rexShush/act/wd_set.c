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
 * Actions to control eye windows.
 */
#include <stdio.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ramp.h"
#include "int.h"

/*
 * Display process needs to be notified if some of the bits
 * in d_wd_disp change.  The following mask determines which bits, if
 * changed, will cause notification.
 */
#define DISP_NOTIFY	(D_W_GRID|D_W_FRAME)

/*
 * Control windows.
 */
int
wd_cntrl(long wdnum, long flag)
{
	WD *wdp;
	int menu;
	int t_cntrl;
	char *err= NP;
	unsigned eflags;

	if(wdnum < 0 || wdnum >= WD_MAXNUM) {
		rxerr("wd_pos(): Bad window number");
		return(0);
	}
	wdp= &i_b->wd[wdnum];
	menu= wdp->m_wd_menu;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(menu && (wdp->m_wd_cntrl != NULLI)) t_cntrl= wdp->m_wd_cntrl;
	else if(flag != NULLI) t_cntrl= flag;
	else t_cntrl= wdp->wd_cntrl;

	switch(t_cntrl) {
	case WD_OFF:
		eyeflag &= ~(wdp->wd_xflag|wdp->wd_yflag);

		/*
		 * Unlink from active list.
		 */
		if(wdp->wd_back == NP) break;
		else {
			wdp->wd_back->wd_for= wdp->wd_for;
			wdp->wd_for->wd_back= wdp->wd_back;
			wdp->wd_back= NP;
		}
		break;
	case WD_ON:
		/*
		 * Link window into active list at end.
		 */
		if(wdp->wd_back == NP) {
			wdp->wd_back= i_b->wd_last.wd_back;
			wdp->wd_back->wd_for= wdp;
			wdp->wd_for= &i_b->wd_last;
			i_b->wd_last.wd_back= wdp;
		}
		break;
	default:
		err= "wd_cntrl(): bad control flag argument";
		goto done;
	}
	wdp->wd_cntrl= t_cntrl;

done:
	if(eflags & INTR_IF) InterruptEnable();

	/*
     * Cannot call rxerr when interrupts are disabled.
     */
	if(err != NP) rxerr(err);
	return(0);
}
int Pwd_cntrl(long *wdnum, long *flag)
{
	wd_cntrl(*wdnum, *flag);
}


/*
 * Specify position source for window position.
 */
static char wd_pos_snum_err[]= "wd_src_pos(): bad source number specifier";
int
wd_src_pos(long wdnum, long x_src, long x_src_num, long y_src, long y_src_num)
{
	WD *wdp;
	DA *dap;
	RAMP *rp;
	MA *map;
	int menu;
	char *err= NP;
	int t_x_src, t_x_src_num, t_y_src, t_y_src_num;
	unsigned eflags;

	if(wdnum < 0 || wdnum >= WD_MAXNUM) {
		rxerr("wd_src_pos(): Bad window number");
		return(0);
	}
	wdp= &i_b->wd[wdnum];
	menu= wdp->m_wd_menu;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();

	if(menu && (wdp->m_wd_xpos_source != NULLI))
		t_x_src= wdp->m_wd_xpos_source;
	else if(x_src != NULLI) t_x_src= x_src;
	else t_x_src= wdp->wd_xpos_source;

	if(menu && (wdp->m_wd_xpos_src_num != NULLI))
		t_x_src_num= wdp->m_wd_xpos_src_num;
	else if(x_src_num != NULLI) t_x_src_num= x_src_num;
	else t_x_src_num= wdp->wd_xpos_src_num;

	if(menu && (wdp->m_wd_ypos_source != NULLI))
		t_y_src= wdp->m_wd_ypos_source;
	else if(y_src != NULLI) t_y_src= y_src;
	else t_y_src= wdp->wd_ypos_source;

	if(menu && (wdp->m_wd_ypos_src_num != NULLI))
		t_y_src_num= wdp->m_wd_ypos_src_num;
	else if(y_src_num != NULLI) t_y_src_num= y_src_num;
	else t_y_src_num= wdp->wd_ypos_src_num;

	switch(t_x_src) {

	case WD_DIRPOS:
		wdp->wd_xposp= &wdp->wd_xdirpos;
		break;

	case WD_RAMP_X:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		rp= &ramp[t_x_src_num];
		wdp->wd_xposp= &rp->ra_x.ra_pos;
		break;

	case WD_RAMP_Y:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		rp= &ramp[t_x_src_num];
		wdp->wd_xposp= &rp->ra_y.ra_pos;
		break;

	case WD_JOY_X:
		wdp->wd_xposp= &joyh;
		break;

	case WD_JOY_Y:
		wdp->wd_xposp= &joyv;
		break;

	case WD_DA:
		if(t_x_src_num < 0 || t_x_src_num >= DA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		dap= &da[t_x_src_num];
		wdp->wd_xposp= &dap->da_curval;
		break;

	case WD_SIGNAL:
		if(t_x_src_num < 0 || t_x_src_num >= SA_MAXSIG) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		wdp->wd_xposp= sa_gvptr[t_x_src_num];
		break;

	case WD_MEM:
		if(t_x_src_num < 0 || t_x_src_num >= MA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		map= &ma[t_x_src_num];
		wdp->wd_xposp= &map->ma_cur_val;
		break;

	default:
		err= "wd_src_pos(): Bad x source specifier";
		goto done_err;
	}

	switch(t_y_src) {

	case WD_DIRPOS:
		wdp->wd_yposp= &wdp->wd_ydirpos;
		break;

	case WD_RAMP_X:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		rp= &ramp[t_y_src_num];
		wdp->wd_yposp= &rp->ra_x.ra_pos;
		break;

	case WD_RAMP_Y:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		rp= &ramp[t_y_src_num];
		wdp->wd_yposp= &rp->ra_y.ra_pos;
		break;

	case WD_JOY_X:
		wdp->wd_yposp= &joyh;
		break;

	case WD_JOY_Y:
		wdp->wd_yposp= &joyv;
		break;

	case WD_DA:
		if(t_x_src_num < 0 || t_x_src_num >= DA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		dap= &da[t_y_src_num];
		wdp->wd_yposp= &dap->da_curval;
		break;

	case WD_SIGNAL:
		if(t_x_src_num < 0 || t_x_src_num >= SA_MAXSIG) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		wdp->wd_yposp= sa_gvptr[t_y_src_num];
		break;

	case WD_MEM:
		if(t_x_src_num < 0 || t_x_src_num >= MA_MAXNUM) {
			err= wd_pos_snum_err;
			goto done_err;
		}
		map= &ma[t_y_src_num];
		wdp->wd_yposp= &map->ma_cur_val;
		break;

	default:
		err= "wd_src_pos(): Bad y source specifier";
		goto done_err;
	}

	wdp->wd_xpos_source= t_x_src;
	wdp->wd_xpos_src_num= t_x_src_num;
	wdp->wd_ypos_source= t_y_src;
	wdp->wd_ypos_src_num= t_y_src_num;

	/*
     * Cannot call rxerr when interrupts are disabled.
     */
done_err:
	if(eflags & INTR_IF) InterruptEnable();
	if(err != NP) rxerr(err);
	return(0);
}
int Pwd_src_pos(long *wdnum, long *x_src, long *x_src_num, long *y_src, long *y_src_num)
{
	wd_src_pos(*wdnum, *x_src, *x_src_num, *y_src, *y_src_num);
}

/*
 * Specify source to be checked by window.
 */
static char wd_check_snum_err[]= "wd_src_check(): bad source number specifier";
int
wd_src_check(long wdnum, long x_src, long x_src_num, long y_src, long y_src_num)
{
	WD *wdp;
	DA *dap;
	RAMP *rp;
	MA *map;
	int menu;
	char *err= NP;
	int t_x_src, t_x_src_num, t_y_src, t_y_src_num;
	unsigned eflags;

	if(wdnum < 0 || wdnum >= WD_MAXNUM) {
		rxerr("wd_src_check(): Bad window number");
		return(0);
	}
	wdp= &i_b->wd[wdnum];
	menu= wdp->m_wd_menu;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(menu && (wdp->m_wd_xcheck_source != NULLI))
		t_x_src= wdp->m_wd_xcheck_source;
	else if(x_src != NULLI) t_x_src= x_src;
	else t_x_src= wdp->wd_xcheck_source;
	if(menu && (wdp->m_wd_xcheck_src_num != NULLI))
		t_x_src_num= wdp->m_wd_xcheck_src_num;
	else if(x_src_num != NULLI) t_x_src_num= x_src_num;
	else t_x_src_num= wdp->wd_xcheck_src_num;
	if(menu && (wdp->m_wd_ycheck_source != NULLI))
		t_y_src= wdp->m_wd_ycheck_source;
	else if(y_src != NULLI) t_y_src= y_src;
	else t_y_src= wdp->wd_ycheck_source;
	if(menu && (wdp->m_wd_ycheck_src_num != NULLI))
		t_y_src_num= wdp->m_wd_ycheck_src_num;
	else if(y_src_num != NULLI) t_y_src_num= y_src_num;
	else t_y_src_num= wdp->wd_ycheck_src_num;

	switch(t_x_src) {

	case WD_DIRPOS:
		wdp->wd_xcheckp= &wdp->wd_xdirpos;
		break;

	case WD_RAMP_X:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		rp= &ramp[t_x_src_num];
		wdp->wd_xcheckp= &rp->ra_x.ra_pos;
		break;

	case WD_RAMP_Y:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		rp= &ramp[t_x_src_num];
		wdp->wd_xcheckp= &rp->ra_y.ra_pos;
		break;

	case WD_JOY_X:
		wdp->wd_xcheckp= &joyh;
		break;

	case WD_JOY_Y:
		wdp->wd_xcheckp= &joyv;
		break;

	case WD_DA:
		if(t_x_src_num < 0 || t_x_src_num >= DA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		dap= &da[t_x_src_num];
		wdp->wd_xcheckp= &dap->da_curval;
		break;

	case WD_SIGNAL:
		if(t_x_src_num < 0 || t_x_src_num >= SA_MAXSIG) {
			err= wd_check_snum_err;
			goto done_err;
		}
		wdp->wd_xcheckp= sa_gvptr[t_x_src_num];
		break;

	case WD_MEM:
		if(t_x_src_num < 0 || t_x_src_num >= MA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		map= &ma[t_x_src_num];
		wdp->wd_xcheckp= &map->ma_cur_val;
		break;

	default:
		err= "wd_src_check(): Bad x source specifier";
		goto done_err;
	}

	switch(t_y_src) {

	case WD_DIRPOS:
		wdp->wd_ycheckp= &wdp->wd_ydirpos;
		break;

	case WD_RAMP_X:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		rp= &ramp[t_y_src_num];
		wdp->wd_ycheckp= &rp->ra_x.ra_pos;
		break;

	case WD_RAMP_Y:
		if(t_x_src_num < 0 || t_x_src_num >= RA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		rp= &ramp[t_y_src_num];
		wdp->wd_ycheckp= &rp->ra_y.ra_pos;
		break;

	case WD_JOY_X:
		wdp->wd_ycheckp= &joyh;
		break;

	case WD_JOY_Y:
		wdp->wd_ycheckp= &joyv;
		break;

	case WD_DA:
		if(t_x_src_num < 0 || t_x_src_num >= DA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		dap= &da[t_y_src_num];
		wdp->wd_ycheckp= &dap->da_curval;
		break;

	case WD_SIGNAL:
		if(t_x_src_num < 0 || t_x_src_num >= SA_MAXSIG) {
			err= wd_check_snum_err;
			goto done_err;
		}
		wdp->wd_ycheckp= sa_gvptr[t_y_src_num];
		break;

	case WD_MEM:
		if(t_x_src_num < 0 || t_x_src_num >= MA_MAXNUM) {
			err= wd_check_snum_err;
			goto done_err;
		}
		map= &ma[t_y_src_num];
		wdp->wd_ycheckp= &map->ma_cur_val;
		break;

	default:
		err= "wd_src_check(): Bad y source specifier";
		goto done_err;
	}

	wdp->wd_xcheck_source= t_x_src;
	wdp->wd_xcheck_src_num= t_x_src_num;
	wdp->wd_ycheck_source= t_y_src;
	wdp->wd_ycheck_src_num= t_y_src_num;

	/*
     * Cannot call rxerr when interrupts are disabled.
     */
done_err:
	if(eflags & INTR_IF) InterruptEnable();
	if(err != NP) rxerr(err);
	return(0);
}
int Pwd_src_check(long *wdnum, long *x_src, long *x_src_num, long *y_src, long *y_src_num)
{
	wd_src_check(*wdnum, *x_src, *x_src_num, *y_src, *y_src_num);
}

/*
 * Set size of window.
 */
int
wd_siz(long wdnum, long xsiz, long ysiz)
{
	WD *wdp;
	int menu;
	unsigned eflags;

	if(wdnum < 0 || wdnum >= WD_MAXNUM) {
		rxerr("wd_siz(): Bad window number");
		return(0);
	}
	wdp= &i_b->wd[wdnum];
	menu= wdp->m_wd_menu;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(menu && (wdp->m_wd_xsiz != NULLI)) {
		wdp->wd_xsiz= wdp->m_wd_xsiz << TEN_TO_IC;
	}
	else if(xsiz != NULLI) wdp->wd_xsiz= xsiz << TEN_TO_IC;
	if(menu && (wdp->m_wd_ysiz != NULLI)) wdp->wd_ysiz=
	wdp->m_wd_ysiz << TEN_TO_IC;
	else if(ysiz != NULLI) wdp->wd_ysiz= ysiz << TEN_TO_IC;

	if(eflags & INTR_IF) InterruptEnable();
	return(0);
}

int Pwd_siz(long *wdnum, long *xsiz, long *ysiz)
{
	wd_siz(*wdnum, *xsiz, *ysiz);
	return(0);
}

/*
 * Set position of window.
 */
int
wd_pos(long wdnum, long xpos, long ypos)
{
	WD *wdp;
	int menu;
	unsigned eflags;

	if(wdnum < 0 || wdnum >= WD_MAXNUM) {
		rxerr("wd_pos(): Bad window number");
		return(0);
	}
	wdp= &i_b->wd[wdnum];
	menu= wdp->m_wd_menu;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();

	if(menu && (wdp->m_wd_xdirpos != NULLI)) {
		wdp->wd_xdirpos= wdp->m_wd_xdirpos << TEN_TO_IC;
	}
	else if(xpos != NULLI) {
		wdp->wd_xdirpos= xpos << TEN_TO_IC;
	}

	if(menu && (wdp->m_wd_ydirpos != NULLI)) {
		wdp->wd_ydirpos= wdp->m_wd_ydirpos << TEN_TO_IC;
	}
	else if(ypos != NULLI) {
		wdp->wd_ydirpos= ypos << TEN_TO_IC;
	}

	if(eflags & INTR_IF) InterruptEnable();
	return(0);
}

int Pwd_pos(long *wdnum, long *xpos, long *ypos)
{
	wd_pos(*wdnum, *xpos, *ypos);
	return(0);
}

/*
 * Set test mode on windows.
 */
int
wd_test(long flag)
{
	if(flag) i_b->i_flags |= I_WINOFF;
	else i_b->i_flags &= ~I_WINOFF;
	r_sendmsg(COMM, CM_STATUS);
	return(0);
}
int Pwd_test(long *flag)
{
	wd_test(*flag);
}

/*
 * Init window struct.
 */
int nullpos= 0;
int
wd_init(void)
{
	WD *wp;
	int i;

	i_b->wd_first.wd_back= NP;
	i_b->wd_first.wd_for= &i_b->wd_last;
	i_b->wd_last.wd_back= &i_b->wd_first;
	i_b->wd_last.wd_for= NP;

	for(i = 0; i < WD_MAXNUM; i++) {
		wp = &i_b->wd[i];
		wp->m_wd_menu= 0;
		wp->wd_cntrl= 0;
		wp->m_wd_cntrl= NULLI;
		wp->wd_xpos_source= 0;
		wp->m_wd_xpos_source= NULLI;
		wp->wd_xpos_src_num= 0;
		wp->m_wd_xpos_src_num= NULLI;
		wp->wd_ypos_source= 0;
		wp->m_wd_ypos_source= NULLI;
		wp->wd_ypos_src_num= 0;
		wp->m_wd_ypos_src_num= NULLI;
		wp->wd_xcheck_source= 0;
		wp->m_wd_xcheck_source= NULLI;
		wp->wd_xcheck_src_num= 0;
		wp->m_wd_xcheck_src_num= NULLI;
		wp->wd_ycheck_source= 0;
		wp->m_wd_ycheck_source= NULLI;
		wp->wd_ycheck_src_num= 0;
		wp->m_wd_ycheck_src_num= NULLI;
		wp->wd_xdirpos= 0;
		wp->m_wd_xdirpos= NULLI;
		wp->wd_ydirpos= 0;
		wp->m_wd_ydirpos= NULLI;
		wp->wd_xsiz= 0;
		wp->m_wd_xsiz= NULLI;
		wp->wd_ysiz= 0;
		wp->m_wd_ysiz= NULLI;
		wp->wd_xposp= &nullpos;
		wp->wd_yposp= &nullpos;
		wp->wd_xcurpos= 0;
		wp->wd_ycurpos= 0;
		wp->wd_xcheckp= &nullpos;
		wp->wd_ycheckp= &nullpos;
		wp->wd_xflag= (01 << (i << 1));
		wp->wd_yflag= (02 << (i << 1));
		wp->wd_for= NP;
		wp->wd_back= NP;
	}
	return(0);
}

/*
 * Specify which cursors to display
 */
int wd_disp(long disp)
{
	/* this function is used only in non-photon versions of rex */
	return(0);
}

/*
 * Specify window numbers for two window display cursors.
 */
int
wd_cursor(long wdnum, long cursor)
{
	/* this function is used only in non-photon versions of rex */
	return(0);
}

/*
 * Specify cursor used for center of screen.
 */
int
wd_center(long cursor)
{
	/* this function is used only in non-photon versions of rex */
	return(0);
}
