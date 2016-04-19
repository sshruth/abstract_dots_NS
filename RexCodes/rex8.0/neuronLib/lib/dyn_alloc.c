#include <stdio.h>
#include <malloc.h>
#include "neuron.h"

/* DYN_ALLOC.C
 * Functions:
 *
 *	dyn_alloc()	= malloc() with test and error message report
 *	dyn_free()	= free array
 *
 *	dyn_palloc()	= nested malloc() for pointer to array of arrays
 *	dyn_pfree()	= free nested parray
 *
 *	dyn_clr()	= zero elements of an array
 *	dyn_pclr()	= zero elements of a parray
 *
 */

/* DYN_ALLOC
 *	get variable space with error mesg
 */
char *dyn_alloc(int n, int sz, char *msg)
{
	char *p = (char *)NULL;

	p = (char *) malloc(n * sz);
	if (p == NULL) {
		rxerr(msg);
	}

	return(p);
}

void dyn_free(char *p)
{
	if(p != (char *)NULL) {
		free(p);
	}
}

/* DYN_PARRAY
 * allocate a 2-D array as an array of pointers to 1-D vectors
 */
char *dyn_palloc(int n1, int sz1, int n2, int sz2, char *msg)
{
	int i;
	char **p;
	char buf[100];

	if((p = (char **) dyn_alloc(n1 + 1, sz1, msg)) != (char **)NULL) {
		for(i = 0; i < n1; i++) {
			sprintf(buf, "dyn_palloc(): %s[%d]", msg, i);

			if((p[i] = dyn_alloc(n2, sz2, buf)) == (char *)NULL) {
				dyn_pfree(p);
				return((char *)NULL);
			}
		}
		p[i] = NULL;	/* last holds null for dyn_pfree() */
	}

	return((char *)p);
}

void dyn_pfree(char **p)
{
	int i;

	if(p != (char **)NULL) {
		for(i = 0; p[i] != NULL; i++) {
			free(p[i]);
		}
		free(p);
	}
}

/* DYN_CLR
 * 	clear elements of pointer array
 */
void dyn_clr(char **z, int n1, int sz1)
{
	register int i;
	register char * p;

	n1 *= sz1;	/* convert to bytes */
	p = (char *) ((int)z + (i * sz1));
	for (i = 0; i < n1; i++) p[i] = 0;
}


/* DYN_PCLR
 * 	clear elements of pointer array
 */
void dyn_pclr(char **z, int n1, int sz1, int n2, int sz2)
{
	register int i, j;
	register char *p;

	n2 *= sz2;	/* convert to bytes */
	for (i = 0; i < n1; i++) {
		p = (char *) ((int)z + (i * sz1));
		for (j = 0; j < n2; j++) p[j] = 0;
	}
}
