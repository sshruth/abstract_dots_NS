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
 *	state.h:  Header for state set processor.
 */

/*
 * These defines cannot be changed without also changing corresponding
 * values in spot/sp.h.
 */
#define MAXESC	    10	    /* max number of escapes */
#define MAXSTATE    1000    /* max number of states across all chains */
#define MAXCHAIN    50	    /* max number of chains */
#define REBEL_FLAG1 127
#define REBEL_FLAG2 128

#define ON		1
#define OFF		-1
#define YES		1		/* arguments for score function */
#define NO		0
#define OPEN_W		0	/* arguments for 'awind()' and mwind() actions -
						 * manipulates analog and mex windows */
#define CLOSE_W		1
#define CANCEL_W	2
#define OPEN_LFP	3	/* these arguments control the collection of */
#define CLOSE_LFP	4	/* local field potentials by MEX */
#define	CANCEL_LFP	5

/*
 *	These are the flags to the behavior controller to know whether
 * to escape on various conditions.
 */
#define TIME	    0
#define BITON	    (TIME + 1)
#define BITOFF	    (TIME + 2)
#define FUNC	    (TIME + 3)
#define EQUAL	    (TIME + 4)
#define LESSOR	    (TIME + 5)
#define GREATER	    (TIME + 6)
#define QUERY	    (TIME + 7)

/*
 *	Running line value flags.
 */
#define RL_NOCHANGE	0
#define RL_ABS		1
#define RL_ADD		2
#define RL_SUB		3

typedef struct state STATE;
typedef struct awake_struct AWAKE;
struct state {
	char statename[30];
	int code;
	int *ptrCode;
	int rlflag;
	int rlval;
	struct _action {
		int (*action)();
		long act_code;
		long act_2code;
		long act_3code;
		long act_4code;
		long act_5code;
		long act_6code;
		long act_7code;
		long act_8code;
		long act_9code;
		long act_10code;
	} device;
	long preset;
	int random;
	struct _escape {
		struct _escape *nextesc;
		AWAKE *awakestate;
		struct state *stateptr;
		int sflags;
		void *funct;
		int smask;
	} escape[MAXESC];
	int stateIndex;			/* added by JWM 10/3/2000 */
	int rebelFlag1;			// ---------------------------------------
	int rebelFlag2;			// added by JWM 12/3/2003 to support rebel
	int stateXpos;			//
	int stateYpos;			// ---------------------------------------
};

/*
 *	ACTION contains a pointer to an action to be performed upon
 * entering a new state. Act_code is a single code which
 * is automatically passed to the routine pointed to by an
 * action.  It may or may not be used as chosen by the
 * routine.  E.g., it may be the device code for the dr11
 * multiplexer.
 */
typedef struct _action ACTION;


/*
 *	ESCAPE contains a function to use to check whether a
 * particular condition is met to cause a state transition, and
 * the state to change to if the function returns the flag
 * NEWSTATE.
 * If the funct == 1, the the transition will occur
 * when time becomes zero.
 */
typedef struct _escape ESCAPE;
extern ESCAPE *firstesc;

/*
 *	The following is the struct which has the basic information to
 * allow transitions to the next state.  The code is placed
 * in the event buffer upon
 * entry to the next state if the code is other than NULL.
 * The time is the length of time to stay in this state if time
 * is one of the escapes.
 */
struct awake_struct {
	long timer;
	signed char init_flag, now_flag;
	STATE *initial_state;
	STATE *current_state;
	STATE **abort_list;
	STATE *prestate;
	STATE *poststate;
	ESCAPE *lastesc;
};
extern AWAKE nowstate[];

/*
 * Function prototypes.
 */
int s_access(STATE *bsp[], char *vstr, char *astr);
int pr_snames(STATE *bsp[], char *vstr, int curstate, int st_page, int last_state);
int mksroot(void);
int pr_state(MENU *mp);
int s_print(STATE *sp, FILE *file);
void stateinit(void);
/* void setstate(STATE *pstate, AWAKE *pawake); */
int awake(long x);
int asleep(long x);
int abort_s(long x);
int reset_s(long x);
int sf_init(long x);

