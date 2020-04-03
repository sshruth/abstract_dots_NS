#

/*
 *	S T A N D A R D   H E A D E R   F I L E :	A L L O C
 *
 *	For routines:	alloc(III)/free(III), malloc(III)/mfree(III)
 *			_alloc(III)/_free(III)
 *			realloc(IIIS), calloc(IIIS), falloc(IIIS)
 */

struct freeblk {		/* Structure system to link all of mem together */
	int blksize;	/* This holds the mem size which follows */
	struct freeblk *nxtblkp;/* Address of this pointer is returned */
};

struct a_ctrl {			/* Control header structure */
	char *a_mptr;		/* malloc() ptr to beginning of memory */
	char *a_mtopp;		/* Top of memory which is allowed */
	struct freeblk a_frlst[1];/* freelist header */
};

#define	BLKSIZE	(sizeof (int))	/* sizeof( freeblk.blksize ) */
#define	EVEN(x)	(((x)+1)&~01)	/* Make (x) even */

extern char etext, edata, ebss, end;
extern struct a_ctrl A_hdr;	/* Header used by alloc()/free() */

extern char *_gmalloc(), *_alloc();

