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
 * Actions supporting memory array sources.
 */
#include <stdio.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "int.h"

static int ma_null= 0;		/* null memory array location */

/*
 * Control memory array sources.
 */
int
ma_cntrl(int manum, int *bap, int count, int rate, int repeat)
{
	MA *map;
	int menu;
	char *err= NP;
	int *tmp_bap, tmp_count, tmp_rate, tmp_repeat;
	unsigned eflags;

	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(manum < 0 || manum >= MA_MAXNUM) {
		err= "ma_cntrl(): Bad memory array number";
		goto done_err;
	}
	map= &ma[manum];

	menu= map->m_ma_menu;
	if(menu && (map->m_ma_bap != (void *)NULLI)) tmp_bap= map->m_ma_bap;
	else if(bap != (void *)NULLI) tmp_bap= bap;
	else tmp_bap= map->ma_bap;

	if(menu && (map->m_ma_count != NULLI)) tmp_count= map->m_ma_count;
	else if(count != NULLI) tmp_count= count;
	else tmp_count= map->ma_count;

	if(menu && (map->m_ma_rate != NULLI)) tmp_rate= map->m_ma_rate;
	else if(rate != NULLI) tmp_rate= rate;
	else tmp_rate= map->ma_rate;

	if(menu && (map->m_ma_repeat != NULLI)) tmp_repeat= map->m_ma_repeat;
	else if(repeat != NULLI) tmp_repeat= repeat;
	else tmp_repeat= map->ma_repeat;

	if((tmp_bap == &ma_null) && (tmp_count != 1)) {
		err= "ma_cntrl(): bap invalid";
		goto done_err;
	}
	if(tmp_count < 1) {
		err= "ma_cntrl(): count must be >= 1";
		goto done_err;
	}
	if(tmp_rate < 1) {
		err= "ma_cntrl(): rate must be >= 1";
		goto done_err;
	}
	if(tmp_repeat < -1) {
		err= "ma_cntrl(): repeat must be >= -1";
		goto done_err;
	}
	map->ma_bap= tmp_bap;
	map->ma_count= tmp_count;
	map->ma_rate= tmp_rate;
	map->ma_repeat= tmp_repeat;
	if(eflags & INTR_IF) InterruptEnable();  /* 1 */

	ma_reset(manum);  

	return(0);

	/*
	 * Cannot call rxerr when interrupts are disabled.
	 */
 done_err:
	if(eflags & INTR_IF) InterruptEnable();  /* 2 */
	if(err != NP) rxerr(err);
	return(0);
}

int Pma_cntrl(int *manum, int *bap, int *count, int *rate, int *repeat)
{
	ma_cntrl(*manum, bap, *count, *rate, *repeat);
	return(0);
}

/*
 * Reset memory array source to initial parameters.
 */
int
ma_reset(int manum)
{
	MA *map;
	unsigned eflags;

	if(manum < 0 || manum >= MA_MAXNUM) {
		rxerr("ma_reset(): Bad memory array number");
		return(0);
	}
  if((eflags= pswget()) & INTR_IF) InterruptDisable();
	map= &ma[manum];
	ma_unlink(map);  
	map->ma_cur_bap= map->ma_bap;
	map->ma_cur_val= *map->ma_bap;
	map->ma_cur_cnt= map->ma_count;
	map->ma_cur_time= map->ma_rate;
	map->ma_cur_repeat= map->ma_repeat;
	map->ma_repcnt= 0;
	if(eflags & INTR_IF) InterruptEnable();

	return(0);
}

int Pma_reset(int *manum)
{
	ma_reset(*manum);
	return(0);
}

/*
 * Turn on memory array source.
 */
int
ma_start(int manum)
{
	MA *map;
	unsigned eflags;

	if(manum < 0 || manum >= MA_MAXNUM) {
		rxerr("ma_start(): Bad memory array number");
		return(0);
	}
	map= &ma[manum];

	if(map->ma_cur_bap == &ma_null) {
		rxerr("ma_start(): Start called before ma_cntrl() or ma_reset()");
		return(0);
	}

	/*
	 * Link into active list at end.
	 */
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
	if(map->ma_back == NP) {
		map->ma_back= ma_last.ma_back;
		map->ma_back->ma_for= map;
		map->ma_for= &ma_last;
		ma_last.ma_back= map;
	}
	map->ma_status |= MA_RUN;
	if(eflags & INTR_IF) InterruptEnable();
	return(0);
}

int Pma_start(int *manum)
{
	ma_start(*manum);
	return(0);
}

/*
 * Turn off memory array source.
 */
int
ma_stop(int manum)
{
  MA *map;

  if(manum < 0 || manum >= MA_MAXNUM) {
    rxerr("ma_stop(): Bad memory array number");
    return(0);
  }
  map= &ma[manum];
  ma_unlink(map);
  return(0);
}

int Pma_stop(int *manum)
{
	ma_stop(*manum);
	return(0);
}

/*
 * Unlink array from active list.
 */
void
ma_unlink(MA *map)
{
    unsigned eflags;

    /*
     * Unlink from active list.
     */
	if((eflags= pswget()) & INTR_IF) InterruptDisable();
    map->ma_valid= 0;
    map->ma_cur_bap= &ma_null;
    map->ma_status= 0;
    if(map->ma_back == NP) goto done;
    else {
        map->ma_back->ma_for= map->ma_for;
        map->ma_for->ma_back= map->ma_back;
        map->ma_back= NP;
    }

done:
	if(eflags & INTR_IF) InterruptEnable();
    return;
}

/*
 * Init and clear memory arrays
 */
int
ma_init(void)
{
    MA *map;
    int i;

    ma_first.ma_back= NP;
    ma_first.ma_for= &ma_last;
    ma_last.ma_back= &ma_first;
    ma_last.ma_for= NP;
    for(i= 0, map= &ma[0]; map < &ma[MA_MAXNUM]; map++, i++) {
		map->ma_status= 0;
		map->ma_repcnt= 0;
		map->m_ma_menu= 0;
		map->ma_bap= &ma_null;
		map->m_ma_bap= (int *)NULLI;
		map->ma_count= 1;
		map->m_ma_count= NULLI;
		map->ma_rate= 1;
		map->m_ma_rate= NULLI;
		map->ma_repeat= 0;
		map->m_ma_repeat= NULLI;
		ma_reset(i);
		map->ma_for= NP;
		map->ma_back= NP;
    }
    return(0);
}
