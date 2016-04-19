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
 * Photon Int interprocess communication.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/* Toolkit headers */
#include <Ph.h>
#include <Pt.h>
#include <Ap.h>

/* Local headers */
#include "ph_toolBar.h"
#include "abimport.h"
#include "proto.h"

#define NVMASK	(s_(G_NMEXEC))	/* allow everything (but myself) to re-interrupt
								 * noun,menu processing
								 */
extern sigset_t	alert_set;
extern WIN_DATCURS localCursTyp;
extern RL localRl;

#undef DEBUG


#pragma off (unreferenced)
#ifdef PHOTON_SIG_COMPAT
int alert(int sig, void *data)
#else
void alert(int sig)
#endif
{
#pragma on (unreferenced)
	PROCTBL_P p = &i_b->ptbl[myptx];
	unsign msg, restormask;
	int fileStat;
	int nindex, nuser;
	char vstr[P_ISLEN], astr[P_ISLEN];

	extern ME_ACCP me_accp;
	extern FILE *rtout;

#ifdef DEBUG
   	fprintf(stderr, "rex_alert() entered\n");
#endif

#ifdef PHOTON_SIG_COMPAT
	if(p->p_state & P_ALRTBSY_ST) return(Pt_CONTINUE);
#else
	if(p->p_state & P_ALRTBSY_ST) return;
#endif
	p->p_state |= P_ALRTBSY_ST;

newscan:
	protect(&p->p_sem);
	msg = p->p_msg & ~p->p_rmask;	/* mask with priority level */
	if(msg == 0) {
		/*
		 * Alert must return from here only.
		 */
		p->p_state &= ~(P_NOSIG_ST|P_ALRTBSY_ST);
		release_(&p->p_sem);
#ifdef PHOTON_SIG_COMPAT
		return(Pt_CONTINUE);
#else
		return;
#endif
	}
	else if(msg & s_(G_KILL)) {
		p->p_msg &= ~s_(G_KILL);
		release_(&p->p_sem);
		p->p_state |= P_EXIT_ST;

#ifdef CLRMIRRORS
		XYmout(0,0);	/* put mirrors at 0,0 */
#endif
#ifdef PCS_SOCKET
		pcsCloseVex();
#endif
		PtExit(0);
	}
	else if(msg & s_(G_STOP)) {		
		p->p_msg &= ~s_(G_STOP);
		release_(&p->p_sem);
		if(i_b->i_flags & I_GO) {
			rxerr("alert(): Cannot stop int if clock is running");
			r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_NEG);
			goto newscan;
		}
		i_b->ph_int_pi= -1;		/* no int process now */
		r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);

#ifdef PCS_SOCKET
 		pcsCloseVex();
#endif
	}
	else if(msg & s_(G_RUN)) {
#ifdef DEBUG
   	fprintf(stderr, "rex_alert(): G_RUN\n");
#endif
		p->p_msg &= ~s_(G_RUN);
		release_(&p->p_sem);
		if(i_b->ph_int_pi > 0) {
			rxerr("alert(): An int process is already running");
			r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_NEG);
			goto newscan;
		}
		i_b->ph_int_pi= myptx;		/* I am now the int process */

		/*
		 * If currently reading from a root file, open root.
		 */
		if(i_b->i_rtflag & RT_READ) {
			/* save the name of the root file in the proc table for rex session */
			strncpy(myptp->p_root, i_b->i_rtname, P_LROOTNAME);

			pthread_mutex_lock(&i_b->i_mutex);
			{
				fileStat = readRoot(myptp->p_root);

				/* copy the real time display variables from shared memory to local memory */
				localCursTyp = i_b->cursTyp;
				localRl = i_b->rl;
			}
			pthread_mutex_unlock(&i_b->i_mutex);

			if(fileStat) r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_NEG);
			else r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
		}
	}
	else if(msg & s_(G_RTEXEC)) {
#ifdef DEBUG
   	fprintf(stderr, "rex_alert(): G_RTEXEC\n");
#endif
		p->p_msg &= ~s_(G_RTEXEC);
		release_(&p->p_sem);

		/*
		 * Note that the int process calls rt_write(myptp->p_root) when
		 * RT_WSTATE is set also.
		 */
		if(i_b->i_rtflag & RT_READ) {
			/* save the name of the root file in the proc table for rex session */
			strncpy(myptp->p_root, i_b->i_rtname, P_LROOTNAME);

			pthread_mutex_lock(&i_b->i_mutex);
			{
				fileStat = readRoot(myptp->p_root);

				/* copy the real time display variables from shared memory to local memory */
				localCursTyp = i_b->cursTyp;
				localRl = i_b->rl;
			}
			pthread_mutex_unlock(&i_b->i_mutex);
		}
		else if(i_b->i_rtflag & (RT_WMENU|RT_WUMENU|RT_WSTATE)) {
			/* save the name of the root file in the proc table for rex session */
			strncpy(myptp->p_root, i_b->i_rtname, P_LROOTNAME);

			fileStat = rt_write(myptp->p_root);
		}

		if(fileStat) r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_NEG);
		else r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);
	}
#ifdef NIH_ACUC
	else if(msg & s_(G_SCORE_ALERT)) {
#ifdef DEBUG
		fprintf(stderr, "rex_alert(): G_SCORE_ALERT\n");
#endif
		p->p_msg &= ~s_(G_SCORE_ALERT);
		release_(&p->p_sem);

		/* pop up a dialog reminding the user that
		 * the monkey has not made a correct response
		 */
		ApCreateModule(ABM_scoreAlertDialog, ABW_base, NULL);
	}
#endif
	else if(msg & s_(G_NMEXEC)) {
#ifdef DEBUG
   	fprintf(stderr, "rex_alert(): G_NMEXEC\n");
#endif
		p->p_msg &= ~s_(G_NMEXEC);
		stufs(i_b->i_verbs, vstr, &vstr[P_ISLEN]);
		stufs(i_b->i_args, astr, &astr[P_ISLEN]);
		nindex= i_b->i_nindex;
		nuser= i_b->i_nuser;
		i_b->i_nindex= NULLI;

		/*
		 * Prepare to allow re-interrupts.
		 */
		restormask= (p->p_rmask ^ NVMASK) & NVMASK;
		p->p_rmask |= NVMASK;
		p->p_state &= ~(P_NOSIG_ST|P_ALRTBSY_ST|P_EARLYWAKE_ST);
		release_(&p->p_sem);
		if(sigprocmask(SIG_UNBLOCK, &alert_set, NULL) == -1)
		    rxerr("alert(): cannot sigprocmask");

		if(*vstr == '\0') {
		    ME_RECUR recur;

		    recur.me_rec_bp= NP;
		    if(nuser) recur.me_rec_menup= &umenus[nindex];
		    else recur.me_rec_menup= &menus[nindex];
		    recur.me_rec_astr= astr;
		    access_m(&me_accp, &recur);
		}

		/*
		 * Comm may have already been awakened by noun,menu.  If
		 * not, awaken it.
		 */
		if(!(p->p_state & P_EARLYWAKE_ST))
			r_sendmsg(&i_b->ptbl[i_b->ph_switch_pi], CM_AFFIRM);

		/*
		 * Disable re-interrupts and restore pri mask.
		 */
		if(sigprocmask(SIG_BLOCK, &alert_set, NULL) == -1)
		    rxerr("alert(): cannot sigprocmask");
		p->p_state |= (P_NOSIG_ST|P_ALRTBSY_ST);
		p->p_rmask &= ~restormask;
#ifdef DEBUG
   	fprintf(stderr, "rex_alert(): G_NMEXEC done\n");
#endif
	}
	else if(msg & s_(G_ABORT)) {
	}
	else {		/* bad msg */
		p->p_msg &= ~msg;
		release_(&p->p_sem);
		rxerr("alert(): Int received illegal message");
	}

	goto newscan;
}

/*
 * On cntlb reset interprocess communication to a default state.
 */
void
oncntrlb(int sig)
{
	int tmp;
	PROCTBL_P p;
	int old_value;
	char *old_name;

	tmp= sig;

	signal(S_CNTRLB, oncntrlb);
	p= myptp;
	p->p_state |= P_NOSIG_ST;		/* prevent IPC */
	sleep(1);				/* let comm run first */
	p->p_msg = p->p_vindex = p->p_rmask= 0;
	p->p_sem = HIGHBIT;
	if(i_b->i_flags & I_GO) {
		ad_stop();
		clk_stop();
	}
	p->p_state &= ~P_NOSIG_ST;
	old_name = doutput_filename;
	doutput_filename = "/tmp/doutput_int";
	old_value = doutput_tofile;
	doutput_tofile = 1;
	doutput(0);
	doutput_tofile = old_value;
	doutput_filename = old_name;
}

void death(int sig)
{
	/* stop the a/d converters */
	ad_ds_end();

	/* send message to scribe to close data file */
	to_scrb_(SC_ADERR);

	/* stop the msec clock */
	clk_stop();

	/* clean up process table */
	release_(&myptp->p_sem);
	myptp->p_state |= P_EXIT_ST;
	myptp->p_id = 0;
	if(i_b->ph_int_pi == myptx) i_b->ph_int_pi = -1;

#ifdef CLRMIRRORS
	XYmout(0, 0);
#endif
#ifdef PCS_SOCKET
	pcsCloseVex();
#endif

	/* send rex error message */
	if(sig == SIGFPE) rxerr("INT PROCESS DIED WITH SIGFPE ERROR (divide by 0?)");
	if(sig == SIGSEGV) rxerr("INT PROCESS DIED WITH SIGSEGV ERROR (null pointer?)");

	PtExit(-1);
}
