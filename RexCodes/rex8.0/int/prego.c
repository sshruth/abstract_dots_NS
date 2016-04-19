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
 * Initializations done before clock is begun.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <hw/inout.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "sac.h"
#include "ramp.h"
#include "int.h"

#undef DEBUG

#ifdef PCI_AtoD
extern int niFirstInt;
#endif

int
prego(void)
{
	RL *rlp;
	RL_CHAN *rcp;
	int chan;
	
	/*
	 * The wrap index is set so that enough is left in buffer to contain
	 * the max amount that might be sampled and stored at one time.
	 */
	r_b->r_cendx= r_b->r_wrapx= RAWSIZE - (SA_MAXSIG+1); /* extra 1
							        for safety */
	r_b->r_bufendx= RAWSIZE;
	fillcheck= r_b->r_cendx - AHDRSZ_IN_DATUMS;
	r_b->r_sam_swtch= 1;

	/*
	 * Misc other Initializations.
	 */
	if(sig_init()) return(-1);
	w_flags = 0;
	eyeflag = 0;
	sp_lock = ds_lock = winrate = 0;

	/*
	 * Running line.
	 */
	rlp = &i_b->rl;
	for(rcp = &i_b->rl.rl_chan[0], chan = 0; rcp < &i_b->rl.rl_chan[RL_MAXCHAN]; rcp++, chan++) {
		if(rcp->rl_signal < 0) continue;
	    if(rcp->rl_signal >= SA_MAXSIG) {
			char st[100];

			sprintf(st, "prego(): 'signal_num' out of range for run line chan %d", chan);
			rxerr(st);
			continue;
	    }
	    rcp->rl_addr = sa_gvptr[rcp->rl_signal];
	    rcp->rl_sig_gvname = &samp.sa_gvname[rcp->rl_signal][0];
	    rcp->rl_sig_title = &samp.sa_title[rcp->rl_signal][0];
	}
	
	/*
	 * Eye window display
	 */
	for(chan = 0; chan < WDI_NDATACURS; chan++) {
		switch(i_b->cursTyp.datCurs[chan].type) {
		case CHAN_CENTER:
			curPtrs[chan].x = &winCntrx;
			curPtrs[chan].y = &winCntry;
			break;
		case CHAN_EYE:
			curPtrs[chan].x = &eyeh;
			curPtrs[chan].y = &eyev;
			break;
		case CHAN_OEYE:
			curPtrs[chan].x = &oeyeh;
			curPtrs[chan].y = &oeyev;
			break;
		case CHAN_OTHER:
			curPtrs[chan].x = &otherh;
			curPtrs[chan].y = &otherv;
			break;
		case CHAN_ADD:
			curPtrs[chan].x = &addh;
			curPtrs[chan].y = &addv;
			break;
		case CHAN_JOY:
			curPtrs[chan].x = &joyh;
			curPtrs[chan].y = &joyv;
			break;
		case CHAN_DA0:
			curPtrs[chan].x = &da[0].da_curval;
			curPtrs[chan].y = &da[1].da_curval;
			break;
		case CHAN_DA1:
			curPtrs[chan].x = &da[2].da_curval;
			curPtrs[chan].y = &da[3].da_curval;
			break;
		case CHAN_RAMP0:
			curPtrs[chan].x = &ramp[0].ra_x.ra_pos;
			curPtrs[chan].y = &ramp[0].ra_y.ra_pos;
			break;
		case CHAN_RAMP1:
			curPtrs[chan].x = &ramp[1].ra_x.ra_pos;
			curPtrs[chan].y = &ramp[1].ra_y.ra_pos;
			break;
		case CHAN_VERG:
			curPtrs[chan].x = &vergh;
			curPtrs[chan].y = &vergv;
			break;
		case CHAN_CYCLOP:
			curPtrs[chan].x = &cycloph;
			curPtrs[chan].y = &cyclopv;
			break;
		case CHAN_GAZE:
			curPtrs[chan].x = &gazeh;
			curPtrs[chan].y = &gazev;
			break;
		case CHAN_OGAZE:
			curPtrs[chan].x = &ogazeh;
			curPtrs[chan].y = &ogazev;
			break;
		case CHAN_VGAZE:
			curPtrs[chan].x = &vgazeh;
			curPtrs[chan].y = &vgazev;
			break;
		case CHAN_CGAZE:
			curPtrs[chan].x = &cgazeh;
			curPtrs[chan].y = &cgazev;
			break;
		default:
			curPtrs[chan].x = (int *)NULL;
			curPtrs[chan].y = (int *)NULL;
		}
	}

	flag_msg = scrb_msg = comm_msg = 0;
	st_count = sacstate = rurate = int_request = 0;
	Ctcount = Cstate = 0;

#ifdef PLEX_ECODES

	/*
	 * Init 8255 that communicates with Plexon box.
	 * Mode 0, Ports A, B, C-LO output, C-HI input.
	 */
	out8(PL_DIOBASE+3, 0x88);
	i_b->pl_hilx= i_b->pl_hidx= i_b->pl_lolx= i_b->pl_lodx= 0;
#endif	
	
#ifdef MEXWEX_UNITS
	mex1_sync= 1;
	mex2_sync= 1;
#endif	
#ifdef TC_UNITS

	/*
	 * Init 9513 timer/counter chips.
	 */
	if(tc_units) tc_init();
#endif	
#ifdef SMI_TRACK
	if(smi_on) {
	  {
	    int i;
	    for(i=0; i < SMI_CHIP_NUM; i++) {
			out8((SMI_DIO_BASE+3) + (i * 4), 0x9b);  /* set all modes
							to input */
	    }
	  }
	  {
	    int i;
	    for(i=0; i < SMI_MAXCOUNTS; i++) smi_counts[i]= 0;
	  }
	  
	  smi_icnt= smi_new= smi_old= 0;
	  smi_debtog= smi_debtog2= smi_deblval= smi_debhval= smi_late= smi_overrun= 0;
	}
#endif    
	i_b->int_msg_pid = 0;
	inside_clock = 0;
#ifdef PCI_AtoD
        niFirstInt= 0;
#endif   
	return(0);
}
	
/*
 * Validate SIG structure, and initialize SAMP structure.
 * Called before clock is begun.
 */
int
sig_init(void)
{
    SAMP *sap;
    SIG *sgp;
    GVPTR *gvp;
    AD_GAIN *ag;
    AD_CALIB *cap;
    int i;
    short sframe, *sfp, *ssp;
    int snum, acq_rate_x, ad_bin_const;
    int store_rate_x, store_bin_const;
    int stored_samp= 0, stored_tot= 0, sig_tot= 0;
    
    /*
     * Loop thru SIG struct.
     */
    null_samp_arrays();		/* init samp struct arrays */
    null_sig_arrays();		/* null unused sig arrays */
    sap= &samp;
    for(snum= 0, sgp= &sig; sgp < &sig[SA_MAXSIG]; snum++, sgp++) {

		/* Note: these checks are done in two
		 * places: here and in menu.c.  Reason that a single check in
		 * menu.c is not sufficient is because these values can be
		 * initialized at compile time in cnf.c and would not be checked
		 * by code in menu.c
		 */
		if(sig_ck_sig_enable(snum, sgp->sig_enable)) return(-1);
		if(sig_ck_mem_src_var(snum, sgp->sig_mem_source)) return(-1);
		if(sig_ck_sig_title_var(snum, sgp->sig_title)) return(-1);
		strncpy(&sap->sa_title[snum][0], sgp->sig_title, P_LNAME);
		if(sgp->sig_enable == SIG_AD_SOURCE) {
	
			/*
			 * Source is a/d channel.
			 */
			sig_tot++;
			if(sig_ck_chan(snum, sgp->sig_ad_chan)) return(-1);
			if(sgp->sig_ad_mem_var[0] == '-') {
				sig_e(snum, "ad_mem_variable cannot be '-'");
				return(-1);
			}
			if(sig_ck_ad_mem_var(snum, sgp->sig_ad_mem_var)) return(-1);
			gvp= gv_look(gv_ad_mem, sgp->sig_ad_mem_var);
			sa_gvptr[snum]= gvp->gv_ptr;
			strncpy(&sap->sa_gvname[snum][0], gvp->gv_name, P_LNAME);
			if(sig_ck_ad_acq_rate(snum, sgp->sig_ad_rate)) return(-1);
			if(sgp->sig_ad_rate == NULLI) {
				sig_e(snum, "ad_acquire_rate cannot be '-'");
				return(-1);
			}
			sap->sa_ad_rate[snum]= sgp->sig_ad_rate;
			if(sig_ck_ad_calib(snum, sgp->sig_ad_calib)) return(-1);
			sap->sa_ad_calib[snum]= sgp->sig_ad_calib;
			for(cap= ad_calib; cap->ad_ca_num != -1; cap++)
				if(cap->ad_ca_num == sgp->sig_ad_calib) break;
			sap->sa_shift[snum]= cap->ad_ca_shift;
#if (!defined AD_CGLIST)
			if(sig_ck_ad_gain(-1, ad_ov_gain)) return(-1);
			sap->sa_ad_ov_gain= ad_ov_gain;
			for(ag= &ad_gain; ag->ad_gain_factor != -1; ag++)
				if(ad_ov_gain == ag->ad_gain_factor) break;
			ad_cglist[0]= ag->ad_gain_bits;
#else
			if(sig_ck_ad_gain(snum, sgp->sig_ad_gain)) return(-1);
			sap->sa_ad_ov_gain= NULLI;
			sap->sa_gain[snum]= sgp->sig_ad_gain;
			for(ag= &ad_gain; ag->ad_gain_factor != -1; ag++)
				if(sgp->sig_ad_gain == ag->ad_gain_factor) break;
			ad_cglist[sgp->sig_ad_chan]= ag->ad_gain_bits;
#endif
			sap->sa_ad_chan[snum]= sgp->sig_ad_chan;
			if(sgp->sig_mem_source[0] != '-') {
				sig_e(snum, "memory_source must be '-'");
				return(-1);
			}
			if(sig_ck_sig_store_rate(snum, sgp->sig_store_rate))
				return(-1);
			if(sgp->sig_store_rate != NULLI) {
				if(sgp->sig_store_rate > sgp->sig_ad_rate) {
					sig_e(snum, "signal_store_rate cannot be > ad_acquire_rate");
					return(-1);
				}
			}
			sap->sa_store_rate[snum]= sgp->sig_store_rate;
		} else if((sgp->sig_enable == SIG_MEM_SOURCE) ||
				  (sgp->sig_enable == SIG_COMP_SOURCE)) {
			/*
			 * Memory source.
			 */
			sig_tot++;
			if(sgp->sig_ad_chan != NULLI) {
				sig_e(snum, "sig_ad_chan must be '-'");
				return(-1);
			}
			if(sgp->sig_ad_mem_var[0] != '-') {
				sig_e(snum, "ad_mem_variable must be '-'");
				return(-1);
			}
			if(sgp->sig_ad_rate != NULLI) {
				sig_e(snum, "ad_acquire_rate must be '-'");
				return(-1);
			}
			if(sgp->sig_ad_calib != NULLI) {
				sig_e(snum, "ad_calib must be '-'");
				return(-1);
			}
#if (defined AD_CGLIST)
			if(sgp->sig_ad_gain != NULLI) {
				sig_e(snum, "ad_gain must be '-'");
				return(-1);
			}
#endif
			if(sgp->sig_mem_source[0] == '-') {
				sig_e(snum, "memory_source cannot be '-'");
				return(-1);
			}
			gvp= gv_look(gv_mem_src, sgp->sig_mem_source);

			sa_gvptr[snum]= gvp->gv_ptr;
			strncpy(&sap->sa_gvname[snum][0], gvp->gv_name, P_LNAME);
			if(sig_ck_sig_store_rate(snum, sgp->sig_store_rate))
				return(-1);
			if(sgp->sig_store_rate == NULLI) {
				sig_e(snum, "signal_store_rate cannot be '-'");
				return(-1);
			}
			sap->sa_store_rate[snum]= sgp->sig_store_rate;
		}
    }

    /*
     * Load rest of SAMP struct.
     */
    sap->sa_maxsig= SA_MAXSIG;
    sap->sa_signum= sig_tot;
    sap->sa_fr_array_sz= SA_FR_ARRAY_SZ;
    sap->sa_maxcal= SA_MAXCAL;
    sap->sa_lname= P_LNAME;
    sap->sa_maxrate= ad_max_rate;
    sap->sa_minrate= ad_min_rate;
    sap->sa_subfr_num= SA_SUBFRNUM;
    sa_1msec_ctd= 1;
    if(ad_max_rate <= 1000) {
		sap->sa_mfr_num= 1;	/* at any rate <= 1kHz
							   every subframe is on a
							   1 msec boundary */
		sa_int_P_ms= ad_max_rate / 1000;
		sap->sa_mfr_dur= sap->sa_mfr_num * sap->sa_subfr_num * sa_int_P_ms;
    } else {
		long period_usec, cum_usec;
		int mframe;

		if((1000000L % (long)ad_max_rate) != 0) {
			rxerr("sig_init(): period of ad_max_rate must be integral number of usec");
			return(-1);
		}
		period_usec= (1000000L / (long)ad_max_rate);
		if(1000L % period_usec) {
			rxerr("sig_init(): period of ad_max_rate must evenly divide 1msec");
			return(-1);
		}
		sa_int_P_ms= 1000L / period_usec;

		/*
		 * Compute period in usec of a *frame*.
		 */
		period_usec= period_usec * (long)sap->sa_subfr_num;
		for(cum_usec= period_usec, mframe= 1; ; cum_usec += period_usec) {
			if((cum_usec % 1000) == 0) break;
			if(++mframe > SA_MAX_MFRAME_CNT) {
				rxerr("sig_init(): ad_max_rate results in too large a master frame");
				return(-1);
			}
		}
		sap->sa_mfr_num= mframe;
		sap->sa_mfr_dur= (int) (cum_usec / 1000L);
    }
    sap->sa_ad_channels= AD_CHANNELS;
    sap->sa_ad_res= AD_RES;
    sap->sa_ad_rcomp= AD_RCOMP;
    sap->sa_datumsz= sizeof(DATUM);
    for(i= 0, cap= ad_calib; cap->ad_ca_num != -1; i++, cap++)
		strncpy(&sap->sa_calibst[i][0], cap->ad_ca_fullscale, P_LNAME);
    sap->sa_ad_rate_bo= (short int)((char *)&samp.sa_ad_rate -
									(char *)&samp.sa_var_data_begin);
    sap->sa_store_rate_bo= (short int)((char *)&samp.sa_store_rate -
									   (char *)&samp.sa_var_data_begin);
    sap->sa_ad_calib_bo= (short int)((char *)&samp.sa_ad_calib -
									 (char *)&samp.sa_var_data_begin);
    sap->sa_shift_bo= (short int)((char *)&samp.sa_shift -
								  (char *)&samp.sa_var_data_begin);
    sap->sa_gain_bo= (short int)((char *)&samp.sa_gain -
								 (char *)&samp.sa_var_data_begin);
    sap->sa_ad_delay_bo= (short int)((char *)&samp.sa_ad_delay -
									 (char *)&samp.sa_var_data_begin);
    sap->sa_ad_chan_bo= (short int)((char *)&samp.sa_ad_chan -
									(char *)&samp.sa_var_data_begin);
    sap->sa_frame_bo= (short int)((char *)&samp.sa_frame -
								  (char *)&samp.sa_var_data_begin);
    sap->sa_gvname_bo= (short int)((char *)&samp.sa_gvname -
								   (char *)&samp.sa_var_data_begin);
    sap->sa_title_bo= (short int)((char *)&samp.sa_title -
								  (char *)&samp.sa_var_data_begin);
    sap->sa_calibst_bo= (short int)((char *)&samp.sa_calibst -
									(char *)&samp.sa_var_data_begin);

    /*
     * Fill in SAMP struct.  Loop for each subframe.
     */
    sap= &samp;
    sfp= &sap->sa_frame;
    stored_tot= 0;
    for(sframe= 0; sframe < SA_SUBFRNUM; sframe++) {
    	ssp= sfp;	/* save location of #_of_stored_samples */
		stored_samp= 0;
		for(snum= 0, sgp= &sig; sgp < &sig[SA_MAXSIG]; snum++, sgp++) {
			if(sgp->sig_enable == SIG_OFF) continue;
			else if(sgp->sig_enable == SIG_AD_SOURCE) {
				if((acq_rate_x= get_rates_index(snum, sgp->sig_ad_rate)) == -1)
					return(-1);
				ad_bin_const= ad_acq_rates[acq_rate_x].ad_acq_bin_const;
				if((sframe & ad_bin_const) == 0) {

					/*
					 * This signal is acquired.
					 */
					*++sfp= (snum & SA_SIGMASK) | SA_ACQUIRE_ONLY;
					if(sgp->sig_store_rate != NULLI) {
						if((store_rate_x= get_rates_index(snum, sgp->sig_store_rate))
						   == -1) return(-1);
						store_bin_const= ad_acq_rates[store_rate_x].ad_acq_bin_const;
						if((sframe & store_bin_const) == 0) {

							/*
							 * This acquired signal is also stored.
							 */
							stored_samp++;
							*sfp &= ~SA_ACQUIRE_ONLY;
						}
					}
				}
			}
			else if((sgp->sig_enable == SIG_MEM_SOURCE) ||
					(sgp->sig_enable == SIG_COMP_SOURCE)) {

				/*
				 * This is a memory_source signal
				 */
				if((store_rate_x= get_rates_index(snum, sgp->sig_store_rate))
				   == -1) return(-1);
				store_bin_const= ad_acq_rates[store_rate_x].ad_acq_bin_const;
				if((sframe & store_bin_const) == 0) {

					/*
					 * This memory_source signal is stored.
					 */
					stored_samp++;
					*++sfp= (snum & SA_SIGMASK) | SA_MEMSIG;
				}
			}
		}
		*ssp= stored_samp;  
		stored_tot += stored_samp;
		*++sfp= SA_SUBFR_TERM;	/* null terminate this row */
		sfp++;			/* advance to next row */
    }
    *sfp= SA_ARRAY_TERM;/* null terminate the entire sa_frame array */
    sa_frame= sfp;	/* init frame pointer to end of array so that
					   when clock is begun the first interrupt will
					   cause an immediate new master frame boundary */

    sap->sa_fr_sa_cnt= stored_tot;
    sap->sa_mfr_sa_cnt= sap->sa_fr_sa_cnt * sap->sa_mfr_num;
    r_b->r_samp= samp;	/* place a copy in raw block for scribe to write
						   out */

#ifdef DEBUG
	{
		int fd;

		fd= open("/tmp/samp", O_WRONLY|O_CREAT|O_APPEND);
		write(fd, &samp, sizeof(samp));
		close(fd);
	}
#endif
    return(0);
}

/*
 * Routines to check individual components of SIG struct.  Called
 * from sig_init() and menu.c
 */
int
sig_ck_sig_enable(int snum, int enable)
{
    switch(enable) {
    case SIG_OFF:
    case SIG_AD_SOURCE:
    case SIG_MEM_SOURCE:
	case SIG_COMP_SOURCE:
	return(0);
    default:	
	    sig_e(snum, "illegal sig_enable");
	    return(-1);
    }
}

int
sig_ck_chan(int snum, int chan)
{
    if(chan != NULLI) {
	if(chan < 0 || chan >= AD_CHANNELS) {
	    sig_e(snum, "illegal ad_chan");
	    return(-1);
	}
    }
    return(0);
}

int
sig_ck_mem_src_var(int snum, char *sp)
{
    if(sig_check_str(sp)) {
		sig_e(snum, "memory_source name too long");
		return(-1);
    }
    if(*sp != '-') {
		if(gv_look(gv_mem_src, sp) == NULL) {
			sig_e(snum, "bad memory_source name");
			return(-1);
		}
    }
    return(0);
}

int
sig_ck_ad_mem_var(int snum, char *sp)
{
    if(sig_check_str(sp)) {
		sig_e(snum, "ad_mem_variable name too long");
		return(-1);
    }
    if(*sp != '-') {
		if(gv_look(gv_ad_mem, sp) == NULL) {
			sig_e(snum, "bad ad_mem_variable name");
			return(-1);
		}
    }
    return(0);
}

int
sig_ck_ad_acq_rate(int snum, int rate)
{
    int i;
    
    /*
     * Make sure that interrupt rate is okay, and compute possible
     * rates.
     */
    if(rate == NULLI) return(0);
    if(sig_ck_ad_max_rate(snum, ad_max_rate)) return(-1);
    if(rate > 0) {
	for(i= 0; i < (SA_SPREAD_LOG2+1); i++)
	    if(rate == ad_acq_rates[i].ad_acq_rate) return(0);
    }
    sig_e(snum, "bad ad_acquire_rate");
    return(-1);
}

int
sig_ck_ad_max_rate(int snum, int rate)
{
    AD_SAMP *ap;

    /*
     * Check to see if legal sampling rate.
     */
    if(rate >= SA_AD_INT_RATE_MIN) {
	for(ap= &ad_samp; ap->ad_samp_max != 0; ap++) {
		if(rate == ap->ad_samp_max) {
		    sig_compute_ad_rates();  /* compute new rates */
		    return(0);
		}
	}
    }
    sig_e(snum, "bad ad_max_rate");
    return(-1);
}

/*
 * Compute valid a/d acquisition rates.
 */
void
sig_compute_ad_rates(void)
{
    int rate_num;	/* number of possible rates */
    int i;
    int next_rate, bin_const;

    rate_num= SA_SPREAD_LOG2+1;
    next_rate= ad_max_rate;
    bin_const= 0;
    for(i= 0;;) {
		ad_acq_rates[i].ad_acq_rate= next_rate;
		ad_acq_rates[i].ad_acq_bin_const= bin_const;
		if(++i >= rate_num) break;
		bin_const <<= 1;
		bin_const |= 01;
		next_rate >>= 1;
    }
    ad_min_rate= next_rate;
}
    
int
sig_ck_ad_calib(int snum, int calib)
{
    AD_CALIB *ap;

    if(calib == NULLI) return(0);
    for(ap = ad_calib; ap->ad_ca_num != -1; ap++) {
		if(ap->ad_ca_num == calib) return(0);
	}
    sig_e(snum, "bad ad_calib");
    return(-1);
}

int
sig_ck_ad_gain(int snum, int gain)
{
    AD_GAIN *ag;

    /*
     * Check to see if legal gain.
     */
    if(gain == NULLI) return(0);
    if(gain != -1) {
	for(ag= &ad_gain; ag->ad_gain_factor != -1; ag++)
	    if(gain == ag->ad_gain_factor) return(0);
    }
    if(snum == -1) rxerr("sig_ck_ad_gain(): bad ad_overall_gain");
    else sig_e(snum, "bad ad_gain");
    return(-1);
}

int
sig_ck_sig_store_rate(int snum, int store_rate)
{
    int i;

    /*
     * Check to see if legal rate.
     */
    if(store_rate == NULLI) return(0);	    /* not stored */
    if(sig_ck_ad_max_rate(snum, ad_max_rate)) return(-1);
    if(store_rate > 0) {
	for(i= 0; i < (SA_SPREAD_LOG2+1); i++)
	    if(store_rate == ad_acq_rates[i].ad_acq_rate) return(0);
    }
    sig_e(snum, "bad signal_store_rate");
    return(-1);
}

int
sig_ck_sig_title_var(int snum, char *sp)
{
    if(sig_check_str(sp)) {
	sig_e(snum, "signal_title name too long");
	return(-1);
    }
    return(0);
}

/*
 * Check to see if string fits and is null terminated.
 */
int
sig_check_str(char *sp)
{
    int i;
	
    for(i= 0;;) {
	if(*sp == NULL) return(0);
	if(i++ == P_LNAME) {
	    *sp= NULL;	    /* force a null term */
	    return(-1);
	}
	sp++;
    }
}

/*
 * Lookup string name in GVPTR array.  Must be an exact match.
 */
GVPTR *
gv_look(GVPTR *gvp, char *sp)
{
    for(; gvp->gv_ptr != 0; gvp++) {
		if(strncmp(gvp->gv_name, sp, P_LNAME) == 0) return(gvp);
    }
    return(NULL);
}

int
get_rates_index(int snum, int rate)
{
    int i;
    
    for(i= 0; i < (SA_SPREAD_LOG2+1); i++) {
		if(rate == ad_acq_rates[i].ad_acq_rate) {
			return(i);
		}
    }
    sig_e(snum, "get_rates_index: bad ad or store rate");
    return(-1);
}

void
sig_e(int snum, char *sp)
{
#define T_LEN 200
    char tmp[T_LEN];
    char *op, *op_end;

    op= tmp, op_end= &tmp[T_LEN];
    if(snum >= 0) {
		op= stufs("Config error, signal ", op, op_end);
		op= itoa_RL(snum, 'd', op, op_end);
		op= stufs(": ", op, op_end);
    } else {
		op= stufs("Config error, ", op, op_end);
    }
    op= stufs(sp, op, op_end);
    rxerr(&tmp);
}

/*
 * Clear and set default null values into SAMP struct arrays.
 */
void
null_samp_arrays(void)
{
    int snum;
    int i;

    for(i= 0; i < SA_FR_ARRAY_SZ; i++) samp.sa_frame[i]= 0;
    for(snum= 0; snum < SA_MAXSIG; snum++) {
		samp.sa_ad_rate[snum]= NULLI;
		samp.sa_store_rate[snum]= NULLI;
		samp.sa_ad_calib[snum]= NULLI;
		samp.sa_shift[snum]= 0;
		samp.sa_gain[snum]= NULLI;
		samp.sa_ad_delay[snum]= NULLI;
		samp.sa_ad_chan[snum]= NULLI;
		sa_gvptr[snum]= &null_int;
		for(i= 0; i < P_LNAME; i++) {
			samp.sa_gvname[snum][i]= '\0';
			samp.sa_title[snum][i]= '\0';
		}
    }
    for(snum= 0; snum < SA_MAXCAL; snum++) {
		for(i= 0; i < P_LNAME; i++) {
			samp.sa_calibst[snum][i]= '\0';
		}
    }
}

/*
 * Clear and set default null values into unused SIG arrays.
 */
void
null_sig_arrays(void)
{
    int snum;
    SIG *sgp;

    for(snum= 0, sgp= &sig; sgp < &sig[SA_MAXSIG]; snum++, sgp++) {
		if(sgp->sig_enable == SIG_OFF) {
			sgp->sig_ad_chan= NULLI;
			strncpy(sgp->sig_ad_mem_var, "-", P_LNAME);
			sgp->sig_ad_rate= NULLI;
			sgp->sig_ad_calib= NULLI;
			sgp->sig_ad_gain= NULLI;
			sgp->sig_ad_delay= NULLI;
			strncpy(sgp->sig_mem_source, "-", P_LNAME);
			strncpy(sgp->sig_title, "-", P_LNAME);
			sgp->sig_store_rate= NULLI;
		}
    }
}

/*
 * Init and setup 9513 timer/counter chips.
 */
#ifdef TC_UNITS
void
tc_init(void)
{
    TC *tcp;
    int mode, inuse;
    int i;

#define DEL in8(0x350),in8(0x350)

    for(tcp= &tc[0]; tcp < &tc[TC_NUM]; tcp++) {
		out8(tcp->tc_port+1, 0xff);	/* reset counter command */
		DEL;
		out8(tcp->tc_port+1, 0x5f);	/* load command */
		DEL;
		out8(tcp->tc_port+1, 0x66);	/* gate off FOUT */
		DEL;
		for(i= 0; i < 5; i++) tcp->tc_lcnt[i]= 0;
		for(i= 0, inuse= 0; i < tcp->tc_num_inuse; i++) {
			out8(tcp->tc_port+1, 0x01 + i); /* set pointer to mode reg */
			DEL;

			/*
			 * Count on falling edge, source n, reload from load,
			 * count repetitively, binary, count up, output off,
			 * no gating (mode D).
			 */
			mode= ( ((i+1) << 8)    /* count source n */
					| (0x1000)	    /* count on falling edge */
					| (0x20)	    /* repetitively */
					| (0x8) );	    /* count up */
			out8(tcp->tc_port, mode);		    /* low byte */
			DEL;
			out8(tcp->tc_port, ((mode >> 8) & 0xff));	    /* high byte */
			DEL;
			inuse |= (01 << i);
		}
		tcp->tc_inuse_bits= inuse;
		out8(tcp->tc_port+1, (0x60 | inuse));	/* load and arm! */
		DEL;
    }
}
#endif	    
