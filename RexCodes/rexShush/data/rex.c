/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * are in the public domain.  This version, REX 4.0, is a port of the
 * original version to the Intel 80x86 architecture.  This version is
 * copyright (C) 1992 by the National Institutes of Health, Laboratory
 * of Sensorimotor Research, Bldg 10 Rm 10C101, 9000 Rockville Pike,
 * Bethesda, MD, 20892, (301) 496-9375.  All rights reserved.
 *-----------------------------------------------------------------------*
 */

/*
 *	Rex is the first REX process executed.  It establishes and
 * initializes the shared data areas and execs comm.
 * This process doesnt communicate with other REX processes;  it
 * simply waits until comm goes away and then it terminates.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <process.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "rexhdrs.h"
#include "ecode.h"

char switchStr[]= "procSwitch";
char ref_err_msg[]= "\
Shared memory areas are still in use.  A REX process may have crashed\n\
but still exist in memory.  Run 'ps' to verify.\n";
PROCTBL ptbl_init = {
	{ 0 },     /* p_id */
	{ 0 },     /* p_state */
	{ 0 },     /* p_sem */
	{ 0 },     /* p_msg */
	{ 0 },     /* p_rmask */
	{ 0 },     /* p_pnounp */
	{ 0 },     /* p_menup */
	{ 0 },     /* p_vindex */
	{ 0 },     /* p_x */
	{ 0 },     /* p_y */
	{ 0 },     /* p_w */
	{ 0 },     /* p_h */
	{ 0, 0 },  /* p_pipe */
	{ 0 },     /* p_name */
	{ 0 },     /* p_root */
	{ 0 },     /* p_version */
};

int status= 0;
INT_BLOCK_P i_b = 0;
RAW_BLOCK_P r_b = 0;

main(int argc, char *argv[])
{
	pid_t my_pid;
	u_int int_size, raw_size;
	int i_fd, r_fd;
	int ref_err;
	int i;
	struct stat sbuf;
	pthread_mutexattr_t i_attr;

	my_pid= getpid();

	/*
	 * Allocate shared segments.
	 */
	int_size= sizeof(INT_BLOCK);
	raw_size= sizeof(RAW_BLOCK);

	/*
	 * Open shared memory areas.  First, unlink any previous area
	 * that might still exist.
	 */
	shm_unlink("/INT_BLOCK");
	shm_unlink("/RAW_BLOCK");
	if((i_fd= shm_open("/INT_BLOCK", O_RDWR|O_CREAT, 0777)) == -1) {
	    fprintf(stderr, "Cannot open shared memory area: /INT_BLOCK\n");
	    goto err_out;
	}

	if((r_fd= shm_open("/RAW_BLOCK", O_RDWR|O_CREAT, 0777)) == -1) {
	    fprintf(stderr, "Cannot open shared memory area: /RAW_BLOCK\n");
	    goto err_out;
	}

	if(ftruncate(i_fd, int_size)) {
	    perror("Cannot ftruncate INT_BLOCK shared memory area");
		goto err_out;
	}

	if(ftruncate(r_fd, raw_size)) {
	    perror("Cannot ftruncate RAW_BLOCK shared memory area");
		goto err_out;
	}

	if((i_b = mmap(0, int_size, PROT_READ|PROT_WRITE,
				  MAP_SHARED, i_fd, 0)) == (void *)-1) {
	    fprintf(stderr, "Cannot mmap shared memory area: /INT_BLOCK\n");
		goto err_out;
	}

	if((r_b = mmap(0, raw_size, PROT_READ|PROT_WRITE,
				  MAP_SHARED, r_fd, 0)) == (void *)-1) {
	    fprintf(stderr, "Cannot mmap shared memory area: /RAW_BLOCK\n");
		goto err_out;
	}

	/*
	 * Initializations of shared memory areas.
	 */
	for(i = 0; i < P_NPROC; i++) {
		i_b->ptbl[i] = ptbl_init;
	}
	strncpy(i_b->i_rxver, RHEADER, P_LVERSION);
	pthread_mutexattr_init(&i_attr);
	pthread_mutex_init(&i_b->i_mutex, &i_attr);
	i_b->scrb_pi = -1;
	i_b->ph_int_pi = -1;
	i_b->ph_rline_pi = -1;
	i_b->ph_ewind_pi = -1;
	i_b->ph_ras_pi = -1;
	i_b->d_rwakecd= ENABLECD;
	i_b->d_wd_disp= D_W_EYE_X;
	i_b->d_emag= 1;
	i_b->d_omag= 3;
	i_b->evdx= -1;
	i_b->evdump= EDUMPINC;
	i_b->i_nindex= NULLI;

	/*
 	 * Exec comm.
	 */
	switch(fork()) {

		/*
		 * Exec comm.
		 */
	case 0:
		if(execl("procSwitch", "procSwitch", "-x1r", "-y1r", 0) == -1) {
			perror("Cant exec procSwitch process");
			exit(1);
		}
	case -1:
		perror("Cant fork to exec procSwitch");
		exit(1);
	}

	/*
	 * Ignore signals;  wait for comm to die.
	 */
	signal(S_CNTRLC, SIG_IGN);
	signal(S_CNTRLB, SIG_IGN);
	signal(S_ALERT, SIG_IGN);

	wait(&status);

	/*
	 * Free shared memory areas.
	 */
 err_out:
	/*
	 * See if reference count is 3 - otherwise a REX process
	 * might still be alive and holding an open file descriptor
	 * on the shared memory area.
	 */
	ref_err= 0;
	if(fstat(i_fd, &sbuf) == -1) {
	    fprintf(stderr, "Cannot fstat shared memory area: /INT_BLOCK\n");
	}
	fprintf(stderr, "rex.c i_fd, sbuf.st_nlink = %d\n", sbuf.st_nlink);
	if(sbuf.st_nlink != 3) ref_err= 1;

	if(fstat(r_fd, &sbuf) == -1) {
	    fprintf(stderr, "Cannot fstat shared memory area: /RAW_BLOCK\n");
	}
	fprintf(stderr, "rex.c r_fd, sbuf.st_nlink = %d\n", sbuf.st_nlink);
	if(sbuf.st_nlink != 3) ref_err= 1;

	if(ref_err == 1) fprintf(stderr, ref_err_msg);
	shm_unlink("/INT_BLOCK");
	shm_unlink("/RAW_BLOCK");
	exit(0);
}
