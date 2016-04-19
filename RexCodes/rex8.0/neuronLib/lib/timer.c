#include <stdio.h>
#include <sys/time.h>
#include <ctype.h>
#include <sys/resource.h>

#ifdef sgi
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>
#endif

/* TIMER.C
 *	routines for program timing.  Prints a nice output line!
 *
 * usage:
 *	at start of program, call "timer_start();"
 *	at end of program, call "timer_print();"
 */

#define SECperMIN	(60)
#define SECperHR	(60 * SECperMIN)
#define SECperDAY	(24 * SECperHR)

static struct timeval tstart;	/* time of program start */
static struct timeval tend;
static struct timezone tzp;

timer_start()
{
	gettimeofday(&tstart, &tzp);
}

long int
pr_time_elt(f, s, lbl, fac, no_zero)
	FILE *f;
	long int s;
	char *lbl;
	int fac, no_zero;
{
	int x;

	x = s / fac;
	s %= fac;

	if (x > 0 || !no_zero) {
		fprintf(f, "%d%s", x, lbl);
	}
	return(s);
}

expand_time(f, msg, sec)
	FILE *f;
	char * msg;
	long int sec;
{
	fprintf(f, "%s", msg);
	sec = pr_time_elt(f, sec, "d : ", SECperDAY, 1);
	sec = pr_time_elt(f, sec, "h : ", SECperHR, 1);
	sec = pr_time_elt(f, sec, "m : ", SECperMIN, 1);
	sec = pr_time_elt(f, sec, "s", 1, 0);
}

timer_print(f)
	FILE *f;
{
	struct rusage ru;
	long int sec, esec;
	int d, h, m, s;
#ifdef sgi
	struct tms t_end;

	times(&t_end);
	ru.ru_utime.tv_sec = (t_end.tms_utime + (HZ / 2)) / HZ;
	ru.ru_stime.tv_sec = (t_end.tms_stime + (HZ / 2)) / HZ;
#else
	getrusage(RUSAGE_SELF, &ru);
#endif
	gettimeofday(&tend, &tzp);

	expand_time(f, "User: ", ru.ru_utime.tv_sec);
	expand_time(f, "  Sys: ", ru.ru_stime.tv_sec);
	fprintf(f, "  ");

	sec = ru.ru_utime.tv_sec + ru.ru_stime.tv_sec;
	expand_time(f, "Total: ", sec);

	esec = tend.tv_sec - tstart.tv_sec;
	expand_time(f, "  Elapsed: ", esec);
	if (esec < 1) esec = 1;
	
	fprintf(f, "  Share: %d%%", (int)(((100 * sec) + (esec / 2)) / esec));
#ifdef PARA
	if (num_procs > 1) fprintf(f, "  %d CPUs\n", num_procs);
	else fprintf(f, "\n");
#else
	fprintf(f, "\n");
#endif
}


