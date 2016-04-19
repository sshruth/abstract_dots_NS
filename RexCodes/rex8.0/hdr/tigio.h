/*
 * tig i/o header
 */
	/* I/O control */

struct TIGIO {			/* I/O buffers for disk and pipes */
	char _ppflag;		/* Operation flags for pipeio, and diskio */
	FILE * _ppiop;		/* Output buffer pointer */
};

extern struct TIGIO diskio, pipeio;	/* Disk and Pipe I/O structures */

void output(char *, char *);
