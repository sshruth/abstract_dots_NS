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
 *----------------------------------------------------------------------------*
 *			HARDWARE INTERRUPT HANDLERS
 *
 *    NOTE!!! There can be no floating point in a QNX interrupt handler!!!
 *
 *----------------------------------------------------------------------------*
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <hw/inout.h>
#include "rexhdrs.h"
#include "bufm.h"
#include "ecode.h"
#include "sac.h"
#include "dma.h"
#include "ramp.h"
#include "int.h"

/*
 *----------------------------------------------------------------------------*
 *    NOTE!!! There can be no floating point in a QNX interrupt handler!!!
 *----------------------------------------------------------------------------*
 */
/*
 * Following variables could be automatic on stack, however have been made
 * static to reduce size of stack allocation when int re-interrupts multiple
 * times.
 */
static int l_lowx, l_highx;		/* used in write to disk */
static long l_awtime;
extern int errprt;
int igoo1, igoo2;
#ifdef GOO
struct SREGS srega, sregb, sregc, sregd;
int igooa= 0, igoob= 0, igooc= 0;
unsigned igooa_int= 0, igoob_int=0, igooc_int= 0;
unsigned intcnt= 0;
#endif

#ifdef PCI_AtoD
int niFirstInt;
#endif

u_int ad_tmp1;
extern DMA dma_a;
short int *ad_bufp;

#if (defined AD_DUALDMA)
u_int ad_tmp2;
extern DMA dma_b;
extern int_a;
#endif

EVENT lev;			/* must temp be defined non-static until bug fix in QNX 4.1 */
DIO_DA_ARG da_args[DA_MAXNUM];	/* ditto */
short int sa_temp[AD_CHANNELS];	/* temp holding area for a/d samples */

#ifdef HARDWARE_SWITCH
int dina_diff= 0;
int old_dina=  0;
#endif

#ifdef MEXWEX_UNITS
#ifdef MEX1_TEST
int mex1_cur;
#endif
int mex1_index;			/* MEX1 IPC communication index */
#endif

#undef DEBUG

const struct sigevent *ad_int_hand(void *arg, int id)
{
	/*
	 *----------------------------------------------------------------------------*
	 *    NOTE!!! There can be no floating point in a QNX interrupt handler!!!
	 *----------------------------------------------------------------------------*
	 */

	struct sigevent *sigEvent = (struct sigevent *)arg;
	int clx_4;			/* current load index */
	DATUM *ladr_3;		/* buffer load address */
	WSTACK *wsp_4;
	int back_3;
	int mframenum;
	int forw_3;
	PROCTBL_P pt_3;		/* pointer to ptbl */
	RAMP *rp;
	WD *wdp;
	int *r4_posp;
	RA_PARAM *r2_rp;
	int *r3_errp;
	/*	EVENT lev;*/		/* local event for ldevent_s in int */
	/*	DA_DIO_ARG args[DA_MAXNUM];*/
	DA *dap;
	MA *map;
	int da_cnt;
	pid_t proxy= 0;
	int r2_cnt;		/* amount to advance algorithm */
	int r2_rpoint;		/* holds rpoint */
	int r3_vel;		/* holds velocity */
	int r3_range;		/* holds range */
	int r3_lim;		/* holds upper or lower limit */
	int *r3_adjust;		/* pointer to sacstack for advancing */
	int *r4_sp;		/* points to sacstatck */
	int r3_tmp;
	int sa_count;		/* count of samples stored this int */
	int sa_rcomp;		/* radix compensation */
	short *sh_ptr;		/* pointer to sa_shift array */
	int sa_sig;		/* current channel */
	int adval;
	short *tmp_sa_frame;
	DIO_DA_ARG *diop;
#if (defined AD_SINGLEDMA)
	int i;
#endif

#ifdef MEXWEX_UNITS
	int mex1_tmp, mex1_ecode, mex1_i;
	int mex2_tmp, mex2_ecode, mex2_i, mex2_units, mex2_new_seq;
	MEX2 *mex2_p;
#endif

#ifdef TC_UNITS
	TC *tcp;
	unsigned short tc_ccnt, tc_ucnt;
	int tc_ctr, tc_i, tc_ecode, tc_unum;
#endif	
#ifdef SMI_TRACK
	int smi_i, smi_tmpl, smi_tmph, smi_check;
	SMI *smi_p;
#endif	
#ifdef PCI_AtoD
	int AIStatus1, i;
#endif

#ifdef BUGTEST
	intcnt++;
	if(igooa == 0) {
		segread(&srega);
		if(srega.ss != 0xf8) {
			igooa= 1;
			igooa_int= intcnt;
		}
	}
	segread(&sregd);
	if(sregd.ss == 0xf8) {
		if(igooa) {
			dprintf("\n aint %u: cs %x ds %x ss %x es %x ",
					igooa_int, srega.cs, srega.ds, srega.ss, srega.ds);
			igooa= 0;
		}
		if(igoob) {
			dprintf("\n bint %u: cs %x ds %x ss %x es %x ",
					igoob_int, sregb.cs, sregb.ds, sregb.ss, sregb.ds);
			igoob= 0;
		}
		if(igooc) {
			dprintf("\n cint %u: cs %x ds %x ss %x es %x ",
					igooc_int, sregc.cs, sregc.ds, sregc.ss, sregc.ds);
			igooc= 0;
		}
	}
#endif

#ifdef PCI_AtoD
   
	if( ! ((AIStatus1= NI_ReadDaqDir(ni_adp, AI_STATUS_1_RO_DIR)) & Interrupt_A_St)) {
		return(NULL);	/* wasnt our interrupt- pass it thru */
	}
	if(niFirstInt == 0) { /* ignore first interrupt, since NI card interrupts on START */
		NI_StrobeDaqDir(ni_adp, INT_A_ACK_WO_DIR, 0xffe0); /* ack all interrupts */
		niFirstInt= 1;
		return(NULL);
	}
#endif

	/*
	 *----------------------------------------------------------------------------*
	 * Keep master system time.  Currently, the master system time is incremented
	 * by the a/d converter interrupt.  This guarantees that the times in the
	 * analog record headers accurately correspond to the analog data.
	 *----------------------------------------------------------------------------*
	 */
	inside_int= 1;		    /* flag is true when inside int routine */
	
	if(--sa_1msec_ctd <= 0) {	    /* wait til 1msec interval comes
									   around */
		sa_1msec_ctd= sa_int_P_ms;  /* reinitialize counter */
		i_b->i_time++;		    /* inc master system time by 1msec */
#ifdef MEX62_UNITS
		if(i_b->i_time == mexTime.rexClock) out8(MEX_CODE_PORT, ((mexTime.code << 4) & 0xf0));
#endif
#ifdef EXT_CTR
		dio_on(EXT_CTR_COUNT);
#endif
	}
#ifdef SCOPE
	dio_on(SCOPE_L1);
#endif

	/*
	 *----------------------------------------------------------------------------*
	 * Restart a/d converters.  This is done as soon as possible so that no
	 * data is lost.  For converters that don't support dual dma, data is copied
	 * from dma buffer to a temp location so that the new incoming data won't
	 * overwrite it.
	 *----------------------------------------------------------------------------*
	 */
#if (defined AD_SINGLEDMA)
	ad_bufp= (short int *)dma_a.dma_lbufp;
	for(i=0; i < AD_CHANNELS; i++) sa_temp[i]= ad_bufp[i]; /* copy data to safe
															  place */
#endif

#ifdef DT2821
	if((ad_tmp1= in16(DT0_ADCSR_RW)) & DT0_ERR) {
		dprintf("A/D csr: %x\n", ad_tmp1);
		ad_tmp2= in16(DT0_SCSR_RW);
		dprintf("A/D scsr: %x\n", ad_tmp2);
		to_scrb_(SC_ADERR);
	}
	out16(DT0_SCSR_RW, DT0_ERRINT|DT0_CLRDMADNE|DT0_DUALDMA|DT0_CLKDMA);
#endif
#ifdef NIATMIO
	if((ad_tmp1= in16(NI0_STAT_RO)) & (NI0_OVERRUN | NI0_OVERFLOW)) {
		dprintf("A/D csr: %x\n", ad_tmp1);
		to_scrb_(SC_ADERR);
	}
	out16(NI0_TCINT_WO, 0x0);	    /* clear TC register */
#endif
#ifdef NIATMIOX
	if((ad_tmp1= in16(NI1_STAT1_RO)) & (NI1_OVERRUN | NI1_OVERFLOW)) {
		dprintf("A/D csr: %x\n", ad_tmp1);
		to_scrb_(SC_ADERR);
	}
	if(ad_tmp1 & NI1_DMATCA) out8(NI1_DMATCACL_WOST_8, 0x0);
	else if(ad_tmp1 & NI1_DMATCB) in8(NI1_DMATCBCL_ROST_8);
#endif
#ifdef AC5508HR
	if((ad_tmp1= in8(AC0_ADCSR1_RW)) & (AC0_CLOERR | AC0_DERR)) {
		dprintf("A/D csr: %x\n", ad_tmp1);
		to_scrb_(SC_ADERR);
	}
	out8(AC0_ADCSR1_RW, 0x0);	    /* clear DMAIP */
	in8(AC0_ADCSR1_RW);		    /* short delay */
	out8(AC0_ADCSR1_RW, AC0_DMAEN); /* reenable dma */
#endif
#ifdef AC5525MF
	if((ad_tmp1= in8(AC1_ADCSR1_RW)) & (AC1_ADOERR)) {
		dprintf("A/D csr: %x\n", ad_tmp1);
		to_scrb_(SC_ADERR);
	}
	out8(AC1_IAK_ST, 0x0);	    /* clear DMAIP */
#endif
#ifdef ANLSDAS16
	if((ad_tmp1= in16(AN0_STATUS_RO)) & (AN0_OVERFLOW)) {
		dprintf("A/D csr: %x\n", ad_tmp1);
		to_scrb_(SC_ADERR);
	}
	Wsetport_WO(AN0_ICTRL_WO_P, an0_ictrl, 0x0, AN0_ADTCIEN);
	in16(AN0_STATUS_RO);    /* short delay */
	Wsetport_WO(AN0_ICTRL_WO_P, an0_ictrl, AN0_ADTCIEN, 0x0);
#endif    
#ifdef ANDAS12
	if((ad_tmp1= in16(AN1_STATUS_RO)) & (AN1_ADERR)) {
		dprintf("A/D csr: %x\n", ad_tmp1);
		to_scrb_(SC_ADERR);
		Wsetport_RW(AN1_BDCTRL_RW, 0x0, AN1_CLADERR);
		in16(AN1_STATUS_RO);    /* short delay */
		Wsetport_RW(AN1_BDCTRL_RW, AN1_CLADERR, 0x0);
		if((ad_tmp1 & AN1_TCDONE) == 0) goto int_done;
	} else if((ad_tmp1 & AN1_TCDONE) == 0) {
		if(in16(AN1_BDCTRL_RW) & AN1_MINTENA) {
			dprintf("A/D interrupt with TCDONE clear, st: %x\n", ad_tmp1);
			to_scrb_(SC_ADERR);
			goto int_done;
		}
	}
	Wsetport_RW(AN1_BDCTRL_RW, 0x0, AN1_CLTCDONE);
	in16(AN1_STATUS_RO);    /* short delay */
	Wsetport_RW(AN1_BDCTRL_RW, AN1_CLTCDONE, 0x0);
#endif    
#ifdef PCI_AtoD
	if(AIStatus1 & (AI_Overrun_St | AI_Overflow_St | AI_SC_TC_Error_St)) {
		NI_WriteDaqWin(ni_adp, INT_CTL_WO, 0x0, Interrupt_A_Enable, 0x0); /* clear Interrupt_A_Enable */
		dprintf("A/D error, csr %x\n", AIStatus1);
		to_scrb_(SC_ADERR);
		goto int_done;
	}
	NI_StrobeDaqDir(ni_adp, INT_A_ACK_WO_DIR, 0xffe0); /* ack all interrupts */
	for(i= 0; i < 8; i++) {
		if(NI_ReadDaqDir(ni_adp, AI_STATUS_1_RO_DIR) & AI_FIFO_Empty_St) {
			NI_WriteDaqWin(ni_adp, INT_CTL_WO, 0x0, Interrupt_A_Enable, 0x0); /* clear Interrupt_A_Enable */
			dprintf("Not enough samples in Fifo\n");
			to_scrb_(SC_ADERR);
			goto int_done;
		}
		sa_temp[i]= NI_ReadDaqDir(ni_adp, ADC_FIFO_DATA_RO_DIR);
	}
	ad_bufp= &sa_temp;
#endif

	/*
	 *----------------------------------------------------------------------------*
	 * Input data from SMI Eye Tracker.
	 *----------------------------------------------------------------------------*
	 */
#ifdef SMI_TRACK
	if(smi_on) {
		for(smi_i= 0;; smi_i++) {
			smi_new= in8(SMI_TOGGLE_BASE) & SMI_TOGGLE_MASK;
			if(smi_new == (in8(SMI_TOGGLE_BASE) & SMI_TOGGLE_MASK)) break;
			if(smi_i == 3) {
				smi_debtog++;
				goto smi_err;
			}
		}
		if(smi_new == smi_old) goto smi_done;
		for(smi_p= &smi[0]; smi_p < &smi[SMI_NUM_VAL]; smi_p++) {
			smi_tmpl= in8(SMI_DIO_BASE+smi_p->smi_offl);
			if(smi_tmpl != in8(SMI_DIO_BASE+smi_p->smi_offl)) {
				smi_deblval++;
				goto smi_err;
			}
			smi_tmph= in8(SMI_DIO_BASE+smi_p->smi_offh);
			if(smi_tmph != in8(SMI_DIO_BASE+smi_p->smi_offh)) {
				smi_debhval++;
				goto smi_err;
			}
			*(smi_p->smi_var)= (((smi_tmph << 8) | (smi_tmpl & 0xff)) - smi_offset)
			    * smi_gain;

		}
		for(smi_i= 0;; smi_i++) {
			smi_check= in8(SMI_TOGGLE_BASE) & SMI_TOGGLE_MASK;
			if(smi_check == (in8(SMI_TOGGLE_BASE) & SMI_TOGGLE_MASK)) break;
			if(smi_i == 3) {
				smi_debtog2++;
				goto smi_err;
			}
		}
		if(smi_new != smi_check) {	/* shouldnt change */
			smi_overrun++;
			goto smi_err;
		}

		/*
		 * Have valid data from SMI.
		 */
		if(smi_icnt < SMI_MAXCOUNTS) smi_counts[smi_icnt]++;
		else smi_late++;

	smi_err:
		smi_icnt= -1;
	smi_done:
		smi_old= smi_new;
		smi_icnt++;
	}
#endif

	/*
	 *----------------------------------------------------------------------------*
	 * Check for MEX units.
	 * MEX sends new unit info as often as every msec.  However, the REX clock and
	 * the MEX clocks aren't syncronized.  Therefore REX checks twice each
	 * interrupt to see if there is any new unit data, and reports an error if
	 * an index number was skipped.
	 *----------------------------------------------------------------------------*
	 */
#ifdef MEXWEX_UNITS
	if(mexwex_units == 1) {
		for(mex1_i= 0; mex1_i < 4; mex1_i++) {

			/*
			 * Optimize for speed here by calling 'in8()' directly.
			 * Read twice until both are the same to debounce input.
			 */
			mex1_tmp= in8(MEX1_LOW_DIR);
			if(mex1_tmp == in8(MEX1_LOW_DIR)) break;
#ifdef GOO
			mex1_tmp= dio_in(MEX1_LOW);
			if(mex1_tmp == dio_in(MEX1_LOW)) break;
#endif
			if(mex1_i == 3) rxerr("int(): Cannot debounce MEX1 input");
			mex1_tmp= mex1_index;
		}

		if((mex1_tmp & 0xf) != mex1_index) {
			if(mex1_sync) {

				/*
				 * First time after clock begun.
				 */
				mex1_index= mex1_tmp & 0xf;
				mex1_sync= 0;
#ifdef MEX1_TEST
				mex1_cur= mex1_tmp >> 4;
#endif
			} else {
				if(++mex1_index > 0xf) mex1_index= 0;
				if((mex1_tmp & 0xf) != mex1_index) {
					rxerr("int(): MEX1 unit missed");
					mex1_sync= 1;
				} else {

					/*
					 * New units.
					 */
					mex1_tmp >>= 4;		    /* shift units down */
#ifdef MEX1_TEST
					if(mex1_cur != mex1_tmp) {
						if(++mex1_cur > 0xf) mex1_cur= 1;
						if(mex1_cur != mex1_tmp) {
							rxerr("int(): Out of sequence");
						}
					}
#endif
					mex1_ecode= MEX1_BASE_CODE;
					for(mex1_i= 0; mex1_i < 4; mex1_i++, mex1_ecode++) {
						if(!(mex1_tmp)) break;	    /* done */
						if(mex1_tmp & 01) {
							lev.e_code= mex1_ecode;
							lev.e_key = i_b->i_time;
							int_ldevent_(&lev);
							rl_setspike(mex1_i+1);	    /* spike 0 is latch */
						}
						mex1_tmp >>= 1;
					}
				}
			}
		}
	}
	else if(mexwex_units == 2) {
		/*
		 *----------------------------------------------------------------------------*
		 * Check for MEX2 units.
		 * MEX sends new unit info as often as every msec.  However, the REX clock and
		 * the MEX clocks aren't syncronized.  Therefore REX checks twice each
		 * msec (if the a/d rate is set to 2KHz) to see if there is any new unit data,
		 * and reports an error if a sequence number was skipped.
		 *----------------------------------------------------------------------------*
		 */
		for(mex2_p= &mex2[0]; mex2_p < &mex2[MEX2_BYTE_NUM]; mex2_p++) {
			for(mex2_i= 0;; mex2_i++) {

				/*
				 * Optimize for speed here by calling 'in8()' directly.
				 * Read twice until both are the same to debounce input.
				 */
				mex2_tmp= in8(mex2_p->mex2_port);
				if(mex2_tmp == in8(mex2_p->mex2_port)) break;
				if(mex2_i == 3) {
					rxerr("int(): Cannot debounce MEX2 input");
					mex2_tmp= mex2_p->mex2_seq;	    /* dont load any units */
					break;
				}
			}
			if((mex2_tmp & MEX2_SEQ_MASK) != mex2_p->mex2_seq) {

				/*
				 * Description of MEX -> REX unit communications algorithm:
				 * This algorithm was designed so that two 8255 chips could
				 * be cabled together with the MEX PC writing the output ports and the
				 * REX PC reading the input ports.  The channel is one byte wide.
				 * The MEX PC outputs a new value to a port whenever
				 * a new unit event occurs, and this value is held in the
				 * port until the next unit event.  The low order bits in the
				 * port contain a communication sequence number.  This number
				 * is incremented in a cycle.  The high order
				 * bits contain the unit event information.  (Number of bits in
				 * sequence number is determined by MEX_SEQ_MASK).
				 *
				 * Each time MEX has info to send, it increments the
				 * sequence number and places the new byte in the port.  REX
				 * reads the port each interrupt and checks to see if the
				 * sequence number has incremented.  If it has, REX assumes
				 * that there is new unit info present.  REX also checks the sequence
				 * number to make sure that no values are skipped.  REX
				 * debounces the port in software.  Ringing on the port
				 * lines does not interfere with this algorithm, and no external
				 * glue logic is required between the 8255 chips.
				 *
				 * Get here when current sequence number does not equal number
				 * present on port.
				 */
				mex2_new_seq= (mex2_tmp & MEX2_SEQ_MASK);
				if(mex2_sync || (mex2_new_seq == 0)) {

					/*
					 * If mex_sync is true, REX wants to resyncronize
					 * communication (e.g. after the clock is begun).  If
					 * mex_new_seq is zero, the MEX PC has initiated
					 * resyncronization.  MEX only initiates resyncronization
					 * when it is first started, and no unit data is sent.
					 * When REX initiates the value present on the port
					 * may be stale or new.  Higher probability it is stale,
					 * however.  It would only be new if it occured in last
					 * msec.  Therefore assume it is stale and ignore any
					 * unit info present.
					 */
					if(mex2_new_seq == 0) mex2_new_seq++;
					mex2_p->mex2_seq= mex2_new_seq;
#ifdef MEX2_TEST
					mex2p->mex2_cur= mex2_tmp >> MEX2_SEQ_P_BYTE;
#endif
				} else {

					/*
					 * New unit data present.  Check for missing sequence number.
					 */
					if(++mex2_p->mex2_seq > MEX2_SEQ_MASK) mex2_p->mex2_seq= 1;
					if(mex2_new_seq != mex2_p->mex2_seq) {
						rxerr("int(): MEX2 unit missed, comm is out of sequence");
						mex2_sync= 1;
					} else {

						/*
						 * New units.
						 */
						mex2_units= (mex2_tmp >> MEX2_SEQ_P_BYTE);
#ifdef MEX2_TEST
						if(mex2p->mex2_cur != mex2_units) {
							if(++mex2p->mex2_cur > (0xff >> MEX2_SEQ_P_BYTE))
								mex2p->mex2_cur= 1;
							if(mex2p->mex2_cur != mex2_units) {
								rxerr("int(): Out of sequence");
							}
						}
#endif
						mex2_ecode= mex2_p->mex2_ecode_base;
						for(mex2_i= 0; mex2_i < MEX2_UNITS_P_BYTE; mex2_i++, mex2_ecode++) {
							if(!(mex2_units)) break;	    /* done */
							if(mex2_units & 01) {
								lev.e_code= mex2_ecode;
								lev.e_key = i_b->i_time;
								int_ldevent_(&lev);
#ifdef LATCH_UNITS
								rl_setspike(mex2_i+1);	    /* bit 0 is latched unit */
#else
								rl_setspike(mex2_i + mex2_p->mex2_unit_base);
#endif
							}
							mex2_units >>= 1;
						}
					}
				}
			}
			mex2_sync= 0;
		}
	}	// end of mexwex_units == 2
#endif	// MEXWEX_UNITS

	/*
	 *----------------------------------------------------------------------------*
	 * Manage analog buffers;  check for overflow.
	 *----------------------------------------------------------------------------*
	 */
	/*
     * Dont store when sampling has been disabled due to an error
     * condition.
     */
	if(r_b->r_sam_swtch == MININT) goto sa_done;
	clx_4= r_b->r_rawlx;	/* current load index */
	if((sa_count= *sa_frame++) == SA_ARRAY_TERM) { /* number of samples to be
													  stored this interrupt */
		sa_frame= &samp.sa_frame;	/* wrap, at beginning of a new
									   frame */
		if(sa_1msec_ctd == sa_int_P_ms) {
			sa_mfrbeglx= clx_4;	/* beginning of new master
								   frame */
			sa_mfrbegtime= i_b->i_time;
		}
		sa_count= *sa_frame++;
	}
	if(sa_count) {			/* have something to store */

		/*
		 * REX analog circular buffers are constructed so that a block
		 * of samples (e.g. from a DMA a/d converter) can be loaded
		 * without checking for wrap-around or overflow after
		 * each load.  Therefore a device could DMA directly into the
		 * circular buffer.  The buffers wrap whenever the amount to be
		 * loaded exceeds a target wrap index.  The length of the buffer
		 * is consequently always changing.
		 */
		if((r_b->r_rawlx += sa_count) >= r_b->r_wrapx) {

			/*
			 * Buffer wrap-  set new current end index.
			 */
			r_b->r_cendx= r_b->r_rawlx;
			r_b->r_rawlx= 0;

			/*
			 * Init new 'fillcheck'- size of buffer.
			 */
			fillcheck= r_b->r_cendx - AHDRSZ_IN_DATUMS;
			if(r_b->r_cendx > r_b->r_bufendx) {

				/*
				 * Should never happen- buffer grew larger than
				 * allocated size.
				 */
				dprintf("Analog buffer grew larger than declared size.\n");
				to_scrb_(SC_ANERR);
				r_b->r_sam_swtch= MININT;
				w_flags |= W_ERR;
				goto sa_done;
			}
		}
		if((r_b->r_fillcnt += sa_count) >= fillcheck) {
			/*
			 * When a window is opened r_fillcnt is initialized to the size
			 * of the w_pre amount.  Then it is incremented when
			 * data is added to the circular buffer.  Scribe decrements
			 * r_fillcnt when data is written to disk.  As long as r_fillcnt
			 * remains lower than fillcheck the buffer has not overflowed.
			 * If we get here when a window is open, we have overflow.
			 * 
			 * When a window is not open, we ignore r_fillcnt when we
			 * get here.  It is only valid after it has been initialized
			 * at window opening time.
			 */
#ifdef DEBUG
			dprintf("\nfillcnt %d fillcheck %d\n", r_b->r_fillcnt, fillcheck);
#endif
			r_b->r_fillcnt= MININT;
			if(r_b->r_wslx != r_b->r_wsdx || w_flags & W_ISOPEN) {

				/*
				 * r_fillcnt exceeded fillcheck when a window is open,
				 * or scribe is still busy dumping previous windows.
				 * Buffer overflow.
				 */
				dprintf("Analog buffer overflow.\n");
				to_scrb_(SC_ANERR);
				r_b->r_sam_swtch= MININT;
				w_flags |= W_ERR;
				goto sa_done;
			}
		}
		ladr_3= &r_b->raw;
		ladr_3 += clx_4;
		windcnt += sa_count;
	}

	/*
	 *----------------------------------------------------------------------------*
	 * Pick up samples from dma buffers.  Process is table driven from arrays
	 * in SAMP struct.
	 *----------------------------------------------------------------------------*
	 */
#if (defined AD_SINGLEDMA)
	ad_bufp= &sa_temp;
#endif
#if (defined AD_DUALDMA)
	if(int_a == 0) {
		int_a= 1;
		ad_bufp= (short int *)dma_a.dma_lbufp;
	} else {
		int_a= 0;
		ad_bufp= (short int *)dma_b.dma_lbufp;
	}
#endif
#ifdef DEBUG
	{
		int i;
		ad_bufp[0]= longlo_(i_b->i_time);
		for(i=1; i < AD_CHANNELS; i++) ad_bufp[i]= i;
	}
#endif
	sa_rcomp= samp.sa_ad_rcomp;
	sh_ptr= &samp.sa_shift;
	tmp_sa_frame = sa_frame; /* temporary copy of frame pointer for first loop */
	while((sa_sig= *tmp_sa_frame++) != SA_SUBFR_TERM) {
		if(sa_sig < 0) {
			/*
			 * Frame pointer out of sync- should only be negative at
			 * end of array.  This condition should have already been
			 * checked for above.
			 */
			dprintf("Frame pointer out of sync.\n");
			to_scrb_(SC_ANERR);
			r_b->r_sam_swtch= MININT;
			w_flags |= W_ERR;
			goto sa_done;
		}

		/* check signal type. if it is NOT a memory signal,
		 * it IS an A/D signal so process it 
		 */
		if(!(sa_sig & SA_MEMSIG)) {
			sa_sig &= SA_SIGMASK;
			adval= ad_bufp[samp.sa_ad_chan[sa_sig]];
			if(samp.sa_shift[sa_sig] >= 0) {
				*sa_gvptr[sa_sig] = (adval - sa_rcomp) >>
				    samp.sa_shift[sa_sig];
			}
			else {
				*sa_gvptr[sa_sig] = (adval - sa_rcomp) <<
				    -samp.sa_shift[sa_sig];
			}
		}
	} /* end of first signal frame while loop */

	/* compute any derived signals */
	vergh = eyeh - oeyeh;
	vergv = eyev - oeyev;
	cycloph = (eyeh + oeyeh) / 2;
	cyclopv = (eyev + oeyev) / 2;
	gazeh = eyeh - otherh;
	gazev = eyev - otherv;
	ogazeh = oeyeh - otherh;
	ogazev = oeyev - otherv;
	vgazeh = gazeh - ogazeh;
	vgazev = gazev - ogazev;
	cgazeh = (gazeh + ogazeh) / 2;
	cgazev = (gazev + ogazev) / 2;

	/* this while loop stores A/D and memory signals into the A-file */
	while((sa_sig= *sa_frame++) != SA_SUBFR_TERM) {
		if(sa_sig & SA_MEMSIG) {
			/*
			 * Store an incore variable to A-file.  Must be an int.
			 */
			sa_sig &= SA_SIGMASK;
			*ladr_3++ = *(DATUM *)sa_gvptr[sa_sig];

		} else if((sa_sig & SA_ACQUIRE_ONLY) == 0) {
			/*
			 * Store an A/D sample to A-file
			 */
			sa_sig &= SA_SIGMASK;
			adval= ad_bufp[samp.sa_ad_chan[sa_sig]];
			*ladr_3++ = adval;
		}
	}

 sa_done:

	/*
     * Apply eye offsets.
     */
	eyeh += eho, eyev += evo, oeyeh += oeho, oeyev += oevo;

	/*
	 *----------------------------------------------------------------------------*
	 * Rest of interrupt routine is executed only once per msec.
	 *----------------------------------------------------------------------------*
	 */
	if(sa_1msec_ctd != sa_int_P_ms) goto int_done;

	/*
	 *----------------------------------------------------------------------------*
	 * Units on 9513 Counter/timer chips.
	 *----------------------------------------------------------------------------*
	 */
#ifdef TC_UNITS
#define DEL in8(0x350)
	if(tc_units) {
		for(tc_unum= 0, tcp= &tc[0]; tcp < &tc[TC_NUM]; tcp++) {

			/*
			 * Loop for each chip.
			 */
			out8(tcp->tc_port+1, (0xa0 | tcp->tc_inuse_bits));	/* save counters */
			DEL, DEL;
			out8(tcp->tc_port+1, 0x19);				/* hold cycle */
			DEL, DEL;
			tc_ecode= tcp->tc_ecode_base;
			for(tc_ctr= 0; tc_ctr < tcp->tc_num_inuse;
			    tc_ctr++, tc_ecode++, tc_unum++) {

				tc_ccnt= (in8(tcp->tc_port) & 0x00ff);    /* count low byte */
				DEL;
				tc_ccnt |= (in8(tcp->tc_port) << 8);	     /* count high byte */
				DEL;
				if(tc_ccnt == tcp->tc_lcnt[tc_ctr]) continue;    /* no new units */

				/*
				 * New units.
				 */
				tc_ucnt= tc_ccnt - tcp->tc_lcnt[tc_ctr];
				tcp->tc_lcnt[tc_ctr]= tc_ccnt;
				if(tc_ucnt > 4) {
					unsigned int goo;
					goo= tc_ucnt;
#ifdef GOO
					dprintf("Too many units: %d, unum %d\n", goo, tc_unum);
#endif
					tc_ucnt= 1;
				}
				for(tc_i= 0; tc_i < tc_ucnt; tc_i++) {
					lev.e_code= tc_ecode;
					lev.e_key= i_b->i_time;
					int_ldevent_(&lev);
				}
#ifdef LATCH_UNITS
				rl_setspike(tc_unum+1);	    /* bit 0 is latched unit */
#else
				rl_setspike(tc_unum);
#endif
			}
		}
	}
#endif

	/*
	 *----------------------------------------------------------------------------*
	 * Digital input processing.
	 *----------------------------------------------------------------------------*
	 */
	{
		int hi, lo;
		lo= dio_in(DIN_LOW);
#ifdef DIN_8
		hi= 0;
#else	   
		hi= dio_in(DIN_HI);
#endif
		dina= ((hi << 8) & 0xff00) | (lo & 0xff);
#ifdef HARDWARE_SWITCH
		dina_diff= old_dina ^ dina;
		old_dina= dina;
		if(dina_diff & PSTOP) {
			if(dina & PSTOP) {
				/*
				 * Switch was turned on.
				 */
				i_b->i_flags |= I_PSTOP;
				softswitch |= PSTOP;
			} else {

				/*
				 * Switch turned off.  Overrides software PSTOP.
				 */
				i_b->i_flags &= ~I_PSTOP;
				softswitch &= ~PSTOP;
			}
			to_comm_(CM_STATUS);
		}
		if(dina_diff & RLSTOP) {
			if(dina & RLSTOP) {

				/*
				 * Switch was turned on.
				 */
				i_b->d_flags |= D_RLSTOP;
				softswitch |= RLSTOP;
			} else {

				/*
				 * Switch turned off.  Overrides software RLSTOP.
				 */
				i_b->d_flags &= ~D_RLSTOP;
				softswitch &= ~RLSTOP;
			}
			to_comm_(CM_STATUS);
		}
#endif
	}

	/*
	 *----------------------------------------------------------------------------*
	 * Collect units data.
	 *----------------------------------------------------------------------------*
	 */
#ifdef BUGTEST
	if(igoob == 0) {
		segread(&sregb);
		if(sregb.ss != 0xf8) {
			igoob= 1;
			igoob_int= intcnt;
		}
	}
#endif
#ifdef FAKE_UNITS		/* generate fake units for testing */
	if(igoo2++ >= 3) {
		igoo2= 0;
		if(igoo1++ >= 30) igoo1= 1;
		lev.e_code = UNIT1CD+igoo1;
		lev.e_key = i_b->i_time;
		int_ldevent_(&lev);
		rl_setspike(igoo1);
	}
#endif
#ifdef LATCH_UNITS		/* grab units if conditionally compiled */
	if(uwind && (dina & UNIT0)) {
		dio_on(LATCH_RESET);
		dio_in(DIN_LOW);	/* extra delay for reset pulse */
		dio_off(LATCH_RESET);
		lev.e_code = UNIT1CD;
		lev.e_key = i_b->i_time;
		int_ldevent_(&lev);
		rl_setspike(0);
	}
#endif

	/*
	 *----------------------------------------------------------------------------*
	 * Hardware abort detection.  This permits a way to abort
	 * when a condition arises that causes the interrupt routine to
	 * run continuously, such as an infinite loop.
	 * Typing cntl B wont work in these cases, since the interrupt
	 * routine is running continuously and never returns.
	 *----------------------------------------------------------------------------*
	 */
#ifdef HARD_ABORT
	if(dina & ABORT) {
		ad_ds_end();
		dprintf("HARD ABORT bit set; a/d turned off\n");
		to_scrb_(SC_ADERR);
	}
#endif

	/*
	 *---------------------------------------------------------------------------*
	 *			   RAMP OUTPUT
	 *---------------------------------------------------------------------------*
	 */
	for(rp= ra_first.ra_for; rp->ra_for != NP; rp= rp->ra_for) {
		if(rp->ra_state > 0)  {
			switch(rp->ra_state)  {

			case RA_DELST:		/* start with 40ms delay for mir to settle */
				rp->ra_state= RA_DELWAIT;
				rp->ra_wtime= 40 / CLOCK_RATE;
				rp->ra_x.ra_valid= 1;
				rp->ra_y.ra_valid= 1;
				break;

			case RA_DELWAIT:
				if(--rp->ra_wtime > 0) break;
				goto ra_go;		/* begin ramp */

			case RA_NODEL:		/* start ramp with no settling delay */
				rp->ra_x.ra_valid= 1;
				rp->ra_y.ra_valid= 1;
				/* continue to RA_GO */

			case RA_GO:			/* begin ramp */
			ra_go:
				rp->ra_state= RA_25WAIT;
				rp->ra_timeleft= rp->ra_x.ra_dtime;
				if(rp->ra_type & RA_NO25MS) rp->ra_wtime= 0;
				else rp->ra_wtime= 25 / CLOCK_RATE;	/* accel delay */
				rp->ra_phistate= 1;
				rp->ra_phicount= 1;
				/* continue to RA_25WAIT */

			case RA_25WAIT:
				if(--rp->ra_wtime <= 0) {
					rp->ra_state= RA_RUN;
					rp->ra_rampflag |= RA_STARTED;
				} else break;
				/* continue to RA_RUN */

			case RA_RUN:				/* output ramp */
				/*
				 *	Update only every ra_urate interrupts.
				 */
				if(rp->ra_phiflag & RA_PHIGO) {
					if( ! (--rp->ra_phicount)) {
						switch(rp->ra_phistate) {

						case 0:		/* was on */
							rp->ra_phistate++;
							rp->ra_phicount= rp->ra_pofftime;
							dio_off(rp->ra_device);
							break;
						case 1:
							rp->ra_phistate= 0;
							rp->ra_phicount= rp->ra_pontime;
							dio_on(rp->ra_device);
							lev.e_code= PHIONCD;
							lev.e_key= i_b->i_time;
							int_ldevent_(&lev);
							break;
						}
					}
				}
				if((rurate -= CLOCK_RATE) > 0) goto da_out;
				rurate= rp->ra_urate;
				r2_rp= &rp->ra_x;
				r3_errp= &rp->ra_x.ra_err;
				r4_posp= &rp->ra_x.ra_pos;
				rp->ra_xflag= 1;
				for(;;)  {

					switch(r2_rp->ra_mode)  {

					case RA_NOMOVE:
						goto rgnext;

					case RA_EQINT:
						*r4_posp += r2_rp->ra_quot;
						break;

					case RA_LESS1:
						*r3_errp += r2_rp->ra_d;
						if(*r3_errp >= r2_rp->ra_dtime)  {
							*r3_errp -= r2_rp->ra_dtime;
							*r4_posp += r2_rp->ra_inc;
							break;
						} else goto rgnext;

					case RA_GREAT1:
						*r3_errp += r2_rp->ra_rem;
						if(*r3_errp >= r2_rp->ra_dtime)  {
							*r3_errp -= r2_rp->ra_dtime;
							*r4_posp += r2_rp->ra_quot + r2_rp->ra_inc;
						} else {
							*r4_posp += r2_rp->ra_quot;
						}
						break;
					}

					/*
					 * Do y now.
					 */
				rgnext:
					if(rp->ra_xflag == 0) break;
					rp->ra_xflag= 0;
					r2_rp= &rp->ra_y;
					r3_errp= &rp->ra_y.ra_err;
					r4_posp= &rp->ra_y.ra_pos;
				}

				if(rp->ra_timeleft-- <= 0) {
					ra_unlink(rp);
					lev.e_code= RDONECD;
					lev.e_key= i_b->i_time;
					int_ldevent_(&lev);
				}

			} /* end switch */
		}
	}

	/*
	 *---------------------------------------------------------------------------*
	 *		     MEMORY ARRAY OUTPUT
	 *---------------------------------------------------------------------------*
	 */
	for(map= ma_first.ma_for; map->ma_for != NP; map= map->ma_for) {

		/*
		 * Catch case of initial repeat count == 0.
		 */
		if(map->ma_cur_repeat == 0) goto ma_done;
		if(--map->ma_cur_time <= 0) {
			if(map->ma_cur_cnt <= 0) {
				if(--map->ma_cur_repeat == 0) {
					/*
					 * Done- repeat count exhausted.
					 */

				ma_done:
					ma_unlink(map);
					continue;

				} else {
					/*
					 * Reset and go again.
					 */
					map->ma_cur_bap= map->ma_bap;
					map->ma_cur_cnt= map->ma_count;
					map->ma_repcnt++;
					if(map->ma_cur_repeat < 0) map->ma_cur_repeat= -1;
				}
			}

			/*
			 * Output next point.
			 */
			map->ma_cur_val= *map->ma_cur_bap++;
			map->ma_valid= 1;
			map->ma_cur_cnt--;
			map->ma_cur_time= map->ma_rate;
		}
	}

	/*
	 *----------------------------------------------------------------------------*
	 *			D/A OUTPUT
	 *----------------------------------------------------------------------------*
	 */
 da_out:
	for(diop= &da_args[0], dap= da_first.da_for, da_cnt= 0;
	    dap->da_for != NP; dap= dap->da_for) {
		if(*(int *)dap->da_validp) {
			if(dap->da_mode & (DA_OFFSET_CONN))
				diop->dio_da_value= *(int *)dap->da_posp + dap->da_offset;
			else diop->dio_da_value= *(int *)dap->da_posp;
			/*
			 * Don't update if value hasn't changed.  Not only saves
			 * cylces, but may prevent unnecessary glitches on output
			 * of d/a.
			 */
			if(diop->dio_da_value == dap->da_curval) continue;
			diop->dio_da_id= dap->da_id;
			diop->dio_da_flag= 0;
			diop->dio_da_curvalp= &dap->da_curval;
			da_cnt++, diop++;
		}
	}

	/*
     * Update all d/a's at same time.
     */
	if(da_cnt) dio_da(da_cnt, &da_args[0]);

	/*
	 *----------------------------------------------------------------------------*
	 *			SACCADE DETECTION
	 * Algorithm discriminates by position initially to save run time.  A possible
	 * saccade triggers a subsequent 5 point filter with integer coef.
	 * NOTE:  Code assumes execution rate of 1msec.  Durations, times
	 * will be off otherwise.
	 *----------------------------------------------------------------------------*
	 */
	if(sdctl)  {
		switch (sacstate)  {

		case S_SEARCH:

			/*
			 * Look for points that exceed position ranges.
			 * Note that eyeh and eyev are normalized at this point to
			 * 40 steps/deg, 2's comp.
			 */
			if(eyeh > limh.ulim)  {
				newval= aval= &eyeh;
				bval= &limh.ulim;
				sacflags= SF_ULIM;
				goto hstart;
			}
			if(eyeh < limh.blim)  {
				aval= &limh.blim;
				newval= bval= &eyeh;
				sacflags= 0;
			hstart:
				sacstack[0]= eyeh;
				sacstate++;

				/*
				 * Save eye position at start of possible saccade.
				 */
				h_sacinit = eyeh;
				v_sacinit = eyev;
				sacflags |= SF_HOR;
				break;
			}
			if(eyev > limv.ulim)  {
				newval= aval= &eyev;
				bval= &limv.ulim;
				sacflags= SF_ULIM;
				goto vstart;
			}
			if(eyev < limv.blim)  {
				aval= &limv.blim;
				newval= bval= &eyev;
				sacflags= 0;
			vstart:
				sacstack[0]= eyev;

				/*
				 * Save eye position at start of possible saccade.
				 */
				h_sacinit = eyeh;
				v_sacinit = eyev;
				sacflags &= ~ SF_GOOD;	/* turn off accepted flag */
				sacstate++;
				break;
			}
			break;

		case S_P1:
		case S_P2:
		case S_P3:

			/*
			 * Accumulate four more points that exceed range.  If
			 * four consecutive points aren't seen, abort and go
			 * back to S_SEARCH.
			 */
			if(*aval > *bval)  {
				sacstack[sacstate]= *newval;
				sacstate++;
			} else
				sacstate= S_SEARCH;
			break;

		case S_EVAL:

			/*
			 * Determine if initial velocity exceeds a threshold.  If
			 * so, then assume saccade start.
			 */
			if(*aval <= *bval)  {
				sacstate= S_SEARCH;
				break;
			}

			/*
			 * Filter for velocity computation: 5 points,
			 * weighted -1, -1, 0, +1, +1.  Integer coeff.
			 * for fast computation.
			 */
			r4_sp= &sacstack[4];
			r3_vel= *r4_sp= *newval;
			r3_vel += *--r4_sp;
			--r4_sp;
			r3_vel -= *--r4_sp;
			r3_vel -= *--r4_sp;
			if(abs(r3_vel) <= velstart)  {

				/*
				 * No saccade;  set new range limits around new
				 * range point p0.  If any of p1-p4 already exceed
				 * this new range, advance algorithm to proper
				 * state from S_P1 to S_EVAL.
				 * Note that if eye value is at or close to max
				 * or min input range of A/D new limits may exceed or
				 * fall below max, min values.
				 */
				if(sacflags & SF_HOR) limp= &limh;
				else limp= &limv;
				limp->rpoint= r2_rpoint= *r4_sp;
				r3_range= limp->range;
				limp->ulim = r2_rpoint + r3_range;
				limp->blim = r2_rpoint - r3_range;

				/*
				  r4_tmp= r2_rpoint + r3_range;
				  if(r4_tmp > ADC_MAX2C)  *limp++ = ADC_MAX2C;
				  else *limp++ = r4_tmp;
				  r4_tmp= r2_rpoint - r3_range;
				  if(r4_tmp < ADC_MIN2C)  *limp++ = ADC_MIN2C;
				  else *limp++ = r4_tmp;
				*/

				r4_sp= &sacstack[5];
				if(sacflags & SF_ULIM)  {
					r3_lim= (sacflags & SF_HOR ? limh.ulim :
							 limv.ulim);
					for(r2_cnt= 0; *--r4_sp > r3_lim; r2_cnt++);
				} else {
					r3_lim= (sacflags & SF_HOR ? limh.blim :
							 limv.blim);
					for(r2_cnt= 0; *--r4_sp < r3_lim; r2_cnt++);
				}
				if(r2_cnt)  {

					/*
					 * Must shift down entries in sacstack to
					 * get new p0 when advancing algorithm.
					 */
					r3_adjust= &sacstack;
					sacstate= r2_cnt;	/* advance to r2_cnt */
					while(r2_cnt--) *r3_adjust++ = *++r4_sp;
				} else
					sacstate= S_SEARCH;
				break;
			}

			sacstate++;
			sacflags |= SF_ONSET;		/* saccade is going */
			sacfillp= &sacstack[5];		/* pointer for future fills */
			sacontime= i_b->i_time - 4;		/* take p0 */
			duration= 4;
			peakvel= 0;

			if(sacflags & SF_HOR) sd_mark_sig("eyeh", SAC_MARK_START);
			else sd_mark_sig("eyev", SAC_MARK_START);
			break;

		case S_ACCEPT:

			/*
			 * Wait for velocity to fall below velend.  Determine if
			 * saccade meets acceptance criteria.
			 */
			if(++duration < STSIZE)  {
				r4_sp= sacfillp++;
				r3_vel= *r4_sp= *newval;
				r3_vel += *--r4_sp;
				--r4_sp;
				r3_vel -= *--r4_sp;
				r3_vel -= *--r4_sp;
				if(r3_vel < 0) r3_vel= -r3_vel;
				if(r3_vel > peakvel) peakvel= r3_vel;
				if(r3_vel >= velend) break;

				if( (duration >= durmin)
				    && (duration <= durmax)
				    && (peakvel >= velmin)
				    && (peakvel <= velmax) )  {

					lev.e_code= (sacflags & SF_HOR ? SACONHCD :
								 SACONVCD);
					lev.e_key= sacontime;
					int_ldevent_(&lev);

					/*
					 * Mark good saccade.
					 */
					if(sacflags & SF_HOR)
						sd_mark_sig("eyeh", SAC_MARK_GOOD);
					else sd_mark_sig("eyev", SAC_MARK_GOOD);
					sacstate= S_DELAY;

					/*
					 * Keep parameters of good saccade.
					 */
					h_sacend = eyeh;
					v_sacend = eyev;
					h_sacsize = h_sacend - h_sacinit;
					v_sacsize = v_sacend - v_sacinit;
					timeout= delay_time;
					sacflags |= SF_GOOD;
					sacflags &= ~SF_ONSET;
					break;
				}
			}
			sacflags &= ~(SF_ONSET | SF_GOOD);
			sacstate= S_SEARCH;
			break;

		case S_DELAY:

			if(timeout--) break;
			sacstate= S_SEARCH;
		}
	}

	/*
	 *---------------------------------------------------------------------------*
	 *			  SIGNAL WINDOW
	 * REX maintains an internal calibration of 40 steps/deg, 2's comp, quad 0
	 * +,+ (monkey view) for all eye related a/d and d/a signals.
	 *---------------------------------------------------------------------------*
	 */
	if((winrate--) < 0) {
		winrate= i_b->t_wrate;
		/*
		 * While in this loop protect against any reinterrupts.
		 * The default is to assume that the signal is within the window if
		 * window is off.  Bits are *set* in 'eyeflag' when the signal
		 * is outside the window.
		 */
		eyeflag= 0;
		for(wdp= i_b->wd_first.wd_for; wdp->wd_for != NP; wdp= wdp->wd_for) {
			wdp->wd_xcurpos= *wdp->wd_xposp;
			r3_tmp= *wdp->wd_xcheckp - *wdp->wd_xposp;
			if(r3_tmp < 0) r3_tmp= -r3_tmp;
			if(r3_tmp >= wdp->wd_xsiz) eyeflag |= wdp->wd_xflag;
			wdp->wd_ycurpos= *wdp->wd_yposp;
			r3_tmp= *wdp->wd_ycheckp - *wdp->wd_yposp;
			if(r3_tmp < 0) r3_tmp= -r3_tmp;
			if(r3_tmp >= wdp->wd_ysiz) eyeflag |= wdp->wd_yflag;
		}
		if(i_b->i_flags & I_WINOFF) eyeflag= 0;
	}

	/*
	 *----------------------------------------------------------------------------*
	 *			WRITE TO DISK
	 * Must run at high pri;  critical sections with other parts of int.  Order of
	 * testing bits in w_flags takes into account that the user is directly setting
	 * these bits to manipulate windows and may make mistakes (e.g. two window
	 * opens without intervening close, a cancel AND a close, etc).
	 *
	 * This section is really hard to follow because it was coded for the pdp11
	 * without subroutine calls, and to require just a single test per interrupt
	 * if the window is not open.
	 *----------------------------------------------------------------------------*
	 */

	/*
     * Only one bit test is done per interrupt if window is not open.
     */
	if(w_flags & (W_OPEN|W_ISOPEN|W_NULLOPEN)) {	/* INDENT LEVEL A */
		/*
		 * ---->
		 * Get here when a window is either open, or a request to open
		 * has been issued. First check for reopen request, then continue
		 * processing.
		 * ---->
		 */
		if( (w_flags & W_OPEN)
		    &&  (w_flags & (W_ISOPEN|W_NULLOPEN)) ) {		/* INDENT LEVEL B */

			/*
			 * Request to open window while it is already open.  Proabably
			 * an open done during the w_post time.
			 */
			dprintf("Window open request made while window already open\n");
			dprintf("or during w_post time.  Open ignored.\n");
			to_scrb_(SC_ANERR);
			w_flags &= ~W_OPEN;
			lev.e_code = WOPENERRCD;
			lev.e_key = i_b->i_time;
			int_ldevent_(&lev);
		}
		if(w_flags & (W_ISOPEN|W_NULLOPEN)) {		/* INDENT LEVEL B */

			/*
			 * ---->
			 * Get here if a window is currently open.  Check first for
			 * error conditions, then see if time to write a record or
			 * process a close request.
			 * ---->
			 */
			if( (w_flags & (W_CANCEL|W_ERR))
			    ||  (i_b->i_flags & I_FORCECLOSE) )  {		/* INDENT LEVEL C */

				/*
				 * Scribe sets I_FORCECLOSE on errors, int.c sets
				 * W_ERR in buffer management section above.
				 */
				if(w_flags & W_CANCEL) {
					lev.e_code = WCANCELCD;
					lev.e_key = i_b->i_time;
					int_ldevent_(&lev);
				} else {

				err_wind_exit:
					lev.e_code = WERRCD;
					lev.e_key = i_b->i_time;
					int_ldevent_(&lev);
				}
				w_flags= 0;	/* close window on cancel or error */
				i_b->i_flags &= ~(I_WINDOPEN|I_FORCECLOSE);
				goto int_done;

			}
			if(w_flags & W_NULLOPEN) {			/* INDENT LEVEL C */
				if(w_flags & W_CLOSE) {
					w_flags= 0;
					lev.e_code = WCLOSECD;
					lev.e_key = i_b->i_time;
					int_ldevent_(&lev);
					i_b->i_flags &= ~(I_WINDOPEN|I_FORCECLOSE);
				}
				goto int_done;

			}
			if(windcnt >= hiwat) {				/* INDENT LEVEL C */

#ifdef DEBUG
				dprintf("Write: windcnt %d, hiwat %d, rawlx %d, time %D\n",
						windcnt, hiwat, r_b->r_rawlx, i_b->i_time);
				dprintf("  fillcnt %d\n", r_b->r_fillcnt);
#endif
				/*
				 * ---->
				 * Have something to write.  Window will be written out
				 * either as one A-file record, or an initial A-file
				 * record plus continuation records.
				 * ---->
				 */
				l_awtime= awtime;   /* save current values while computing
								       new ones */
				l_lowx= lowx;
				if(windcnt >= postcnt) {
#ifdef DEBUG
					dprintf("post_expire: windcnt %d postcnt %d time %D\n",
							windcnt, postcnt, i_b->i_time);
#endif
					/*
					 * When windcnt >= postcnt, w_post time has expired.
					 * Write out whatever is remaining in this window.
					 */
					back_3= (windcnt - postcnt);
					w_flags= 0;
				} else {

					/*
					 * Either first write for this window, or a
					 * continuation record write.
					 * Reduce amount to write so that it is modulo 512
					 * bytes.  Compute new lowx and awtime for *next*
					 * write (not this one!).  Account for analog header.
					 */
					back_3= (windcnt % DAT_P_BLOCK) + AHDRSZ_IN_DATUMS;
					awtime= 0;	/* no time in cont records */
					if(w_flags & W_WAITCLOSE) {
#ifdef DEBUG
						dprintf("wait_close: postcnt %d hiwat %d windcnt %d back_3\n",
								postcnt, hiwat, windcnt, back_3);
#endif
						postcnt -= (windcnt - back_3);
						if(postcnt < hiwat) hiwat= postcnt;
#ifdef DEBUG
						dprintf("  postcnt %d hiwat %d windcnt %d back_3\n",
								postcnt, hiwat, windcnt, back_3);
#endif
					}
				}
				backlx_(l_highx, r_b->r_rawlx, back_3);
				lowx= lasthighx= l_highx;
				windcnt= back_3;

				/*
				 * Load the analog write stack.  Scribe dumps from lowx to
				 * highx - 1 inclusively.
				 */
				wsp_4 = &r_b->wstack[r_b->r_wslx];
				wsp_4->ws_flag= WS_WRITE;
				wsp_4->ws_code= ADATACD;
				wsp_4->ws_cont= (c_count ? 1 : 0);
				wsp_4->ws_acount= c_count++;
				wsp_4->ws_time= l_awtime;
				wsp_4->ws_lowx= l_lowx;
				wsp_4->ws_highx= l_highx;

				if(++r_b->r_wslx == NUMWST)
					r_b->r_wslx = 0;
				if (r_b->r_wslx == r_b->r_wsdx) {

					/*
					 * Write stack overflow.
					 */
					dprintf("Analog write stack overflow- 1.\n");
					to_scrb_(SC_ANERR);
					goto err_wind_exit;
				}
#ifdef DEBUG
				dprintf("ws: flag %o code %d cont %d, lx %d, dx %d\n  acount %D time %D lowx %d highx %d\n",
						wsp_4->ws_flag, wsp_4->ws_code, wsp_4->ws_cont, r_b->r_wslx,
						r_b->r_wsdx, wsp_4->ws_acount,
						wsp_4->ws_time,
						wsp_4->ws_lowx, wsp_4->ws_highx);
#endif
				to_scrb_(SC_ADUMP);
				if(w_flags == 0) i_b->i_flags &= ~I_WINDOPEN;
				goto int_done;

			}   /* INDENT LEVEL C */

			/*
			 * ---->
			 *  If window has be closed, initialize postcnt and wait for
			 * w_post time to expire.
			 * ---->
			 */
			if( (w_flags & W_CLOSE)
			    && ( ! (w_flags & W_WAITCLOSE)) ) {		/* INDENT LEVEL C */
				/*
				 * Process window close request.
				 */
				w_flags &= ~W_CLOSE;
				w_flags |= W_WAITCLOSE;
				lev.e_code = WCLOSECD;
				lev.e_key = i_b->i_time;
				int_ldevent_(&lev);

				/*
				 * Compute count of samples to save during w_post time.
				 * End data window on a master frame boundary.
				 */
				mframenum= ((w_post + (samp.sa_mfr_dur - 1)) / samp.sa_mfr_dur);
				forw_3= (mframenum * samp.sa_mfr_sa_cnt) + 
				    (samp.sa_mfr_sa_cnt - len_(r_b->r_rawlx, sa_mfrbeglx));
				postcnt= windcnt + forw_3;
				if(postcnt < hiwat) hiwat= postcnt;
#ifdef DEBUG
				dprintf("close_compute: mframenum %d w_post %d forw_3 %d postcnt %d\n",
						mframenum, w_post, forw_3, postcnt);
				dprintf("  windcnt %d hiwat %d time %D\n", windcnt, hiwat, i_b->i_time);
#endif
				goto int_done;

			}   /* INDENT LEVEL C */
		}	    /* INDENT LEVEL B */

		if(w_flags & W_OPEN) {				/* INDENT LEVEL B */

			/*
			 * ---->
			 *  Request to open window.
			 *    - open window if A file keeping enabled
			 *    - backup storage by w_pre time.
			 * ---->
			 */
			lev.e_code = WOPENCD;
			lev.e_key = i_b->i_time;
			int_ldevent_(&lev);
			if( ! (i_b->i_flags & I_AOUT)) {

				/*
				 * Dont open window to save data unless A file keeping enabled.
				 */
				w_flags= W_NULLOPEN;
				goto int_done;
			}

			/*
			 *  Open data window.  Data is not queued to scribe for storage on
			 * disk unless analog data keeping is enabled by the I_AOUT flag being
			 * set.  If the I_AOUT flag is reset during the time a window is open,
			 * that window will continue to be stored until it closes.  If
			 * the I_AOUT flag is set while a window is open, that window will
			 * NOT be stored -- storage will start with the NEXT open window.
			 */

			/*
			 * Compute location and time in analog buffer for beginning of window.
			 * First, compute how many master frames compose the w_pre time.  Must
			 * work with master frame granularity because the w_pre is in msec.
			 * A frame might be less than 1msec if sampling rate is > 1Khz.  Note
			 * even with a w_pre of 0msec, we always save the data from the
			 * last master frame boundary (sa_mfrbeglx) to the current load
			 * index (r_rawlx).  This is because it is difficult to deal
			 * with time computations within a master frame given the possiblity
			 * that a frame duration is less than 1msec.
			 */
			mframenum= ((w_pre + (samp.sa_mfr_dur - 1)) / samp.sa_mfr_dur);
#ifdef DEBUG
			dprintf("\nOpen: mfnum %d, rawlx %d, mfrbeglx %d, time %D, mfrtime %D\n",
					mframenum, r_b->r_rawlx, sa_mfrbeglx, i_b->i_time, sa_mfrbegtime);
			dprintf("  sfr_num %d mfr_num %d mfr_dur %d fr_cnt %d mfr_cnt %d pre %d\n",
					samp.sa_subfr_num, samp.sa_mfr_num, samp.sa_mfr_dur, samp.sa_fr_sa_cnt,
					samp.sa_mfr_sa_cnt, w_pre);
#endif
			/*
			 * Compute lowx- index into circular buffer for beginning of
			 * window.  'back_3' equals number of samples to back up from
			 * r_rawlx.
			 */
			back_3= (mframenum * samp.sa_mfr_sa_cnt) +
			    len_(r_b->r_rawlx, sa_mfrbeglx);
#ifdef DEBUG
			dprintf("  back_3 %d ", back_3);
#endif
			if(back_3 >= fillcheck) {

				/*
				 * Whoa!  Can't have a w_pre larger than the buffer!
				 */
				dprintf("w_pre time exceeds length of analog buffer.\n");
				to_scrb_(SC_ANERR);
				goto err_wind_exit;
			}
			backlx_(lowx, r_b->r_rawlx, back_3);
			awtime= sa_mfrbegtime - (mframenum * samp.sa_mfr_dur);
#ifdef DEBUG
			dprintf("awtime %D ", awtime);
#endif
			if(r_b->r_wslx != r_b->r_wsdx) {

				/*
				 * Scribe is still dumping previous windows;  do not alter
				 * r_fillcnt;  compute length from previous lasthighx to new
				 * low index and let scribe subtract it from r_fillcnt when it
				 * gets to this window.  In this way r_fillcnt remains accurate
				 * when scribe is behind the current window.  Note
				 * that length can be negative if w_pre extends past last
				 * high index.  Use ws_cont for the length.
				 */
				wsp_4= &r_b->wstack[r_b->r_wslx];
				wsp_4->ws_flag= WS_OPEN;
				wsp_4->ws_code= ADATACD;
				wsp_4->ws_cont= len_(r_b->r_rawlx, lasthighx) - back_3;
				if(++r_b->r_wslx == NUMWST) r_b->r_wslx= 0;
				if(r_b->r_wslx == r_b->r_wsdx) {
					dprintf("Analog write stack overflow- 2.\n");
					to_scrb_(SC_ANERR);
					goto err_wind_exit;
				}
#ifdef DEBUG
				dprintf("\ndump prev: ws_cont: %d, wslx %d wsdx %d\n",
						wsp_4->ws_cont, r_b->r_wslx, r_b->r_wsdx);
#endif
			} else {
				r_b->r_fillcnt= back_3;
			}
			lasthighx= lowx;
			windcnt= back_3;
			hiwat= adumpinc;
			c_count= 0;
			postcnt= MAXINT;
			w_flags= W_ISOPEN;
#ifdef DEBUG
			dprintf("fillcnt %d\n", r_b->r_fillcnt);
#endif
			i_b->i_flags &= ~I_FORCECLOSE;
			i_b->i_flags |= (I_WINDOPEN);

		}	    /* INDENT LEVEL B */
	}	    /* INDENT LEVEL A */

 int_done:

	/*
	 *----------------------------------------------------------------------------*
	 *		MESSAGE PROCESSING, REMOTE EVENT LOADING
	 * Message sending/ communication to REX upper level, loading of remote events.
	 * A proxy is sent to the 'data' process;  it will wake up and deliver
	 * the requested signal.
	 * Note- currently the signal is returned from the msec clock routine.
	 *----------------------------------------------------------------------------*
	 */
	if(flag_msg) {		/* any messages to send? */
		/*
		 * Only one signal can be returned each interrupt.  Scribe messages
		 * have priority.
		 */
		if(scrb_msg) {
			if(i_b->scrb_pi <= 0) {	/* no scribe process */
				scrb_msg= 0;
			}
			else {
				pt_3= &i_b->ptbl[i_b->scrb_pi];
				if(pt_3->p_sem) {
					if( ! (pt_3->p_state & P_NOSIG_ST) ) {
						if(i_b->int_msg_pid) goto next1;
						i_b->int_msg_pid= pt_3->p_id;
						proxy= i_b->data_proxy;
					}
					pt_3->p_msg |= scrb_msg;
					scrb_msg= 0;
				}
			}
		}
	next1:
		if(comm_msg) {
			pt_3= &i_b->ptbl[0];
			if(pt_3->p_sem) {	/* sem must be free */
				if( ! (pt_3->p_state & P_NOSIG_ST) ) {
					if(i_b->int_msg_pid) goto next2;
					i_b->int_msg_pid= pt_3->p_id;
					proxy= i_b->data_proxy;
				}
				pt_3->p_msg |= comm_msg;
				comm_msg= 0;
			}
		}
	next2:
		flag_msg = (scrb_msg | comm_msg);
	}				/* end sort */

	/*
	 *----------------------------------------------------------------------------*
	 *			RETURN FROM INTERRUPT
	 *----------------------------------------------------------------------------*
	 */
#ifdef PLEX_ECODES

	/*
	 * Check to see if ecodes are waiting to be sent to Plexon box.
	 * Give priority to codes in high priority buffer.
	 */
	if(i_b->pl_hidx != i_b->pl_hilx) {

		if(pl_box_ready_) {
		    pl_send_code_(i_b->pl_hipri[i_b->pl_hidx]);
		    if(++i_b->pl_hidx == PL_MAXCODES) i_b->pl_hidx= 0;
		}
	}
	else if(i_b->pl_lodx != i_b->pl_lolx) {

		if(pl_box_ready_) {
		    pl_send_code_(i_b->pl_lopri[i_b->pl_lodx]);
		    if(++i_b->pl_lodx == PL_MAXCODES) i_b->pl_lodx= 0;
		}
	}
#endif
#ifdef EXT_CTR
	dio_off(EXT_CTR_COUNT);
#endif
#ifdef SCOPE
	dio_off(SCOPE_L1);
#endif
	inside_int= 0;
#ifdef GOO
	if(igooc == 0) {
		segread(&sregc);
		if(sregc.ss != 0xf8) {
			igooc= 1;
			igooc_int= intcnt;
		}
	}
#endif
	return(NULL);
}
