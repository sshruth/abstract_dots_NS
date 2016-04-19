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
 * Dump raw buf data to A file.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "rexhdrs.h"
#include "bufm.h"
#include "ecode.h"
#include "scribe.h"

extern CHANNEL a_chan, e_chan;

static char samphdr_err[]= "dumpa(): Error on write of sample hdr";
static char af_err[]= "dumpa(): Err on write to Afile, possibly out of space";

#undef DEBUG

/*
 * This routine is not reentrant.  It is called only from alert(), which
 * ensures that it is not reentered.
 */
/*int skip= 0;*/
void
dumpa(void)
{
	u_int length;
	WSTACK *wsp;
	ANALOGHDR *ap;
	ANALOGHDR samphdr, savhdr;
	EVENT ev= {0};
	int rawindex;
	u_int dump1, dump2, alength, ndump;
	int retval;

	/*
	  if(skip++ < 10) {
	  dprintf("skip %d\n", skip);
	  return;
	  }
	  skip= 0;
	*/
    while(r_b->r_wsdx != r_b->r_wslx) {
#ifdef DEBUG
		dprintf("dump: dx %x lx %x\n", r_b->r_wsdx, r_b->r_wslx);
#endif
		/*
		 * Is request pending to dump a new sample header?
		 */
		if(i_b->i_flags & I_NEWSAMPHDR) {

			/*
			 * Load a remote event and set up analog header.
			 */
			ev.e_key = a_chan.fkey;
			ev.e_code = SAMPHDRCD;
			samphdr.amagic= MAGIC;
			samphdr.aseqnum = i_b->seqnum_a++;
			samphdr.acode = SAMPHDRCD;
			samphdr.atime= i_b->i_time;
			samphdr.acount= samphdr.acontinue= 0;
			samphdr.alength = sizeof(SAMP);

			if((length= write(analog_fd, &samphdr, sizeof(samphdr))) < sizeof(samphdr)) {
				dumperr(E_ABADW);
				rxerr(samphdr_err);
				return;
			}
			if((length += write(analog_fd, &r_b->r_samp, sizeof(SAMP))) < sizeof(SAMP)) {
				dumperr(E_ABADW);
				rxerr(samphdr_err);
				return;
			}
			ldevent(&ev);
			a_chan.fkey += length;	/* set new key */
			i_b->i_flags &= ~I_NEWSAMPHDR;
		}

		/*
		 *	A WSTACK entry holds all info describing this dump.
		 * To minimize the number write system calls needed, the analog
		 * header is temporaily placed inline with the data to be dumped.
		 * The displaced data is stored temporaily in the savhdr struct.
		 * After dumping is complete, the data is restored to the buffer.
		 *	The beginning point of the write is backed up enough to 
		 * hold the AHDR.  If the low index is close to the start of the
		 * circular buffer, this backup may extend behind the beginning of
		 * the buffer.  This has been accounted for in the declaration
		 * of the buffer-  there is a blank space the size of an AHDR
		 * before the beginning of the circular buffer.
		 */
		wsp= &r_b->wstack[r_b->r_wsdx];
#ifdef DEBUG
		dprintf("flag %o code %d cont %d\nacount %D time %D lowx %d highx %d\n",
				wsp->ws_flag, wsp->ws_code, wsp->ws_cont, wsp->ws_acount, wsp->ws_time,
				wsp->ws_lowx, wsp->ws_highx);
#endif
		if(wsp->ws_flag & WS_OPEN) {

			/*
			 * This entry specifies a window opening;  deceremnt overflow
			 * count and continue (see comments in write to disk section
			 * of int/int.c).
			 */
			ndump= wsp->ws_cont;
			goto next_entry;
		}
		rawindex= wsp->ws_lowx - ((sizeof(ANALOGHDR))/(sizeof(DATUM)));

		/*
		 * Get length of dump.  If dump extends past end of circular buffer
		 * split length into amount before end of buffer and amount remaining
		 * after beginning of buffer.
		 */
		if(wsp->ws_lowx <= wsp->ws_highx) {
			dump1= (wsp->ws_highx - wsp->ws_lowx);
			dump2= 0;
		} else {
			dump1= (r_b->r_cendx - wsp->ws_lowx);
			dump2= (wsp->ws_highx);
		}
		ndump= dump1 + dump2;			/* number of DATUMS dumped */
		dump1 <<= (sizeof(DATUM)/2);		/* get size in bytes */
		dump2 <<= (sizeof(DATUM)/2);
		dump1 += (sizeof(ANALOGHDR));		/* add AHDR length */
		length= dump1 + dump2;			/* write size */
		alength= length - (sizeof(ANALOGHDR));	/* alength in AHDR */
		ap= (ANALOGHDR *)&r_b->raw[rawindex];
		savhdr= *ap;
		ap->amagic= MAGIC;
		ap->aseqnum= i_b->seqnum_a++;
		ap->acode= wsp->ws_code;
		ap->atime= wsp->ws_time;
		ap->acount= wsp->ws_acount;
		ap->acontinue= wsp->ws_cont;
		ap->alength= alength;

#ifdef DEBUG
		dprintf("write1: ap %X, dump1 %u alength %u\n", ap, dump1, alength);
#endif
		if(write(analog_fd, ap, dump1) != dump1) {
#ifdef DEBUG
			dprintf("Err 1\n");
#endif
			dumperr(E_ABADW);
			rxerr(af_err);
			return;
		}
		if(dump2) {
#ifdef DEBUG
			dprintf("write2: dump2 %u\n", dump2);
#endif
			if(write(analog_fd, &r_b->raw[0], dump2) != dump2)  {
#ifdef DEBUG
				dprintf("Err 2\n");
#endif
				dumperr(E_ABADW);
				rxerr(af_err);
				return;
			}
		}

		*ap= savhdr;		/* restore data */
		ev.e_code= wsp->ws_code;
		ev.e_key= a_chan.fkey;
		a_chan.fkey += length;
		ldevent(&ev);		/* load event for this write */

    next_entry:
#ifdef DEBUG
		dprintf("next_entry: ndump %u, fillcnt %d\n", ndump, r_b->r_fillcnt);
#endif
		r_b->r_fillcnt -= ndump;	/* reduce ovcnt by amount dumped */

		/*
		 * wsdx is advanced in following way because it must always be valid.
		 * Incrementing first and then checking for wrap around creates
		 * a critical section when wsdx == NUMWST.
		 */
		if(r_b->r_wsdx == (NUMWST-1)) r_b->r_wsdx= 0;
		else r_b->r_wsdx++;
    }
}

/*
 * On any type of file error (overflows, out of space on device, etc)
 * or on a master reset (cntl b) try to protect data already written
 * by closing files and turning off keeping.
 */
void
dumperr(int error)
{
	int sav_iflags;

	/*
	 * Protect against reentering (e.g. if dumpe() is called from
	 * here to flush Efile dumperr() might be called again by
	 * dumpe()).
	 */
	if(derror) {
		derror |= error;
		return;
	}
	derror |= error;
	r_b->r_sam_swtch= MININT;	/* turn off data saving */
	sav_iflags= i_b->i_flags;
	i_b->i_flags &= ~(I_EOUT|I_AOUT);

	/*
	 * If an analog window is currently open, force its closrure.  Must
	 * be done after clearing I_AOUT.  Likewise, clearing write stack must
	 * be done after insuring a current window is closed.
	 */
	i_b->i_flags |= I_FORCECLOSE;
	r_b->r_wsdx= r_b->r_wslx;		/* clear write stack */

	/*
	 * Try to flush Efile if possible.
	 */
	if( (sav_iflags & I_EOUT)
	&& ((derror & (E_EVERR|E_EBADW|E_ABADW)) == 0) ) dumpe(1);
	i_b->evdx= -1;		/* stop event overflow checking */

	/* if data files were open, close them */
	if((i_b->i_flags & I_FILEOPEN)) n_file("c", "");
}
