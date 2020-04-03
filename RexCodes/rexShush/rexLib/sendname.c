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
 * Send names of nouns and menus to comm.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "rexhdrs.h"

void sendname(PROCTBL_P pp, NOUN *np, NOUN *unp, MENU *mp, MENU *ump)
{
	char ntmp[P_LNAME];

	r_sendmsg(COMM, CM_SENDNAME);	/* let comm know */

	/*
	 * Comm expects nouns first.
	 * Note that a full P_LNAME chars is sent even though name may
	 * be shorter.  Comm takes chars received as valid only up to the
	 * '\0'.  System names are sent first, user names second.
	 */
	do {
		stufs(np->n_name, ntmp, &ntmp[P_LNAME]);
		if (write(pp->p_pipe[1], ntmp, P_LNAME) < P_LNAME) {
			rxerr("sendname(): Bad pipe, sending nouns");
			break;
		}
	} while(*(np++)->n_name != '\0');
	do {
		stufs(unp->n_name, ntmp, &ntmp[P_LNAME]);
		if (write(pp->p_pipe[1], ntmp, P_LNAME) < P_LNAME) {
			rxerr("sendname(): Bad pipe, sending user nouns");
			break;
		}
	} while(*(unp++)->n_name != '\0');

	/*
	 * Now send menus.
	 */
	do {
		stufs(mp->me_name, ntmp, &ntmp[P_LNAME]);
		if (write(pp->p_pipe[1], ntmp, P_LNAME) < P_LNAME) {
			rxerr("sendname(): Bad pipe, sending menus");
			break;
		}
	} while(*(mp++)->me_name != '\0');
	do {
		stufs(ump->me_name, ntmp, &ntmp[P_LNAME]);
		if (write(pp->p_pipe[1], ntmp, P_LNAME) < P_LNAME) {
			rxerr("sendname(): Bad pipe, sending user menus");
			break;
		}
	} while(*(ump++)->me_name != '\0');

	/*
	 * Close pipe to save file descriptors;  won't be needed again.
	 */
	close(pp->p_pipe[1]);
	close(pp->p_pipe[0]);
}
