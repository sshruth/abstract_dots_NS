/*
 * rex stuff
 */
#define INIT_MASK       020000
#define CANCEL_MASK     040000

typedef struct {
	short unsigned	e_seqnum;	/* event sequence number in E file */
	short int	e_code;		/* event code; see event.h */
	long	e_key;		/* a long time; or if this event references
				   an analog record in the A file, a long
				   address */
} EVENT;

struct trials {
	long begin,end;
};

struct count {
	double bcount,scount;
};

struct specify {
	int pnum;
	int btrig,boff,bwidth;
	int strig,soff,swidth;
} ;

long *nextinit();
int goodtrial();
long nexttrig();
