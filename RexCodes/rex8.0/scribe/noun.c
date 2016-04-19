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
 * Scribe noun processing.
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "scribe.h"

static char *badopm= "noun(): Data files already open";
static char *noflm= "noun(): No data files currently open";
static char *noignore= "noun(): Cannot ignore Edata while Adata being kept";
static char *noclock= "noun(): Cannot keep if clock is not running";

/*
 * Noun "file".
 */
void
n_file(char *vstr, char *astr)
{
	switch(*vstr) {

	case 'o':	/* open files */
		if (i_b->i_flags & I_FILEOPEN) rxerr(badopm);
		else if(r_b->r_sam_swtch == MININT) {
			rxerr("Sampling disabled- end clock and begin to clear");
			r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_ABORT);
		}
		else filecr(astr);
		break;

	case 'c':	/* close files */
		if ((i_b->i_flags & I_FILEOPEN) == 0) {
			rxerr(noflm);
			break;
		}
		if(i_b->i_flags & (I_EOUT|I_AOUT)) {
			n_bdata("i", "");
		}
		close(analog_fd);
		close(event_fd);
		i_b->i_flags &= ~(I_FILEOPEN|I_NEWSAMPHDR);
		analog_fd = -1;
		event_fd = -1;
		break;

	default:
		badverb();
		break;
	}
}

/*
 * Noun "adata".
 */
#pragma off (unreferenced)
void
n_adata(char *vstr, char *astr)
{
#pragma on (unreferenced)

	switch(*vstr) {

	case 'k':	/* keep the A-file */

		if( ! (i_b->i_flags & I_FILEOPEN)) rxerr(noflm);
		else if( ! (i_b->i_flags & I_GO)) rxerr(noclock);
		else if( ! (i_b->i_flags & I_EOUT))
			rxerr("Efile writing must be enabled");
		else i_b->i_flags |= I_AOUT;
		break;

	case 'i':	/* ignore (stop keeping) Afile data */
		i_b->i_flags &= ~I_AOUT;

		/*
		 * Wait until the wstack is flushed.
		 * First wait until a current window, if any, is
		 * closed.  Then wait till wstack is empty.
		 */
		while(i_b->i_flags & I_WINDOPEN) sleep(1);
		while(r_b->r_wslx != r_b->r_wsdx) sleep(1);
		break;
	default:
		badverb();
		break;
	}
}

/*
 * Noun "n_edata".
 */
#pragma off (unreferenced)
void
n_edata(char *vstr, char *astr)
{
#pragma on (unreferenced)
	EVENT ev;

	switch(*vstr) {

	case 'k':	/* keep E-file */

		if( ! (i_b->i_flags & I_FILEOPEN)) rxerr(noflm);
		else if(i_b->i_flags & I_EOUT) break;
		else if( ! (i_b->i_flags & I_GO)) rxerr(noclock);
		else {
			i_b->evdx= i_b->evlx;
			i_b->i_flags |= I_EOUT;
		}
		break;

	case 'i':	/* ignore E-file */
		if(i_b->i_flags & I_AOUT) rxerr(noignore);
		else if((i_b->i_flags & I_EOUT) == 0) break;
		else {
			i_b->i_flags &= ~I_EOUT;
			ev.e_code= ESTOPCD;
			ev.e_key= i_b->i_time;
			ldevent(&ev);

			/*
			 * Must flush out event buffer.
			 */
			dumpe(1);
			i_b->evdx= -1;		/* no overflow checking */
sleep(1);
		}
		break;

	default:
		badverb();
		break;
	}
}

/*
 * Noun "both";  verbs apply to both Afile and Efile together.
 */
void
n_bdata(char *vstr, char *astr)
{
	char *tmp;

	tmp= astr;

	/*
	 * Order is important;  Efile must be started first and ended
	 * last.
	 */
	switch(*vstr) {

	case 'k':	/* set both keep flags */
		n_edata("k", "");
		n_adata("k", "");
		break;

	case 'i':	/* ignore both */
		n_adata("i", "");	/* stop A-file first */
		n_edata("i", "");	/* stop E-file second */
		break;

	default:
		badverb();
		break;
	}
}
