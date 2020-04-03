/******************************************************************
**  RCSID: $Id: safe_alloc.h,v 1.2 2002/02/08 01:42:05 jig Exp $
** Program: dowl
**  Module: safe_alloc
**  Author: mazer
** Descrip: interface for safe_alloc
**
** Prefix: SAFE
**
** Revision History (most recent last)
**
** Sun Mar 19 10:56:29 1995 mazer@asio
**  creation date -- this header file should ALWAYS be included!
**
*******************************************************************/

#ifndef _SAFE_ALLOC_H_
#define _SAFE_ALLOC_H_

#include <stdlib.h>

/* PUBLIC ROUTINE PROTOTYPES */
void *SAFE_calloc(size_t, size_t, char *, int);
void *SAFE_malloc(size_t, char *, int);
void *SAFE_realloc(void *, size_t, char *, int);

/* PUBLIC MACROS */
#define SAFE_CALLOC(a,b)  	SAFE_calloc((size_t) (a), (size_t) (b), __FILE__, __LINE__)
#define SAFE_MALLOC(a) 	  	SAFE_malloc((size_t) (a), __FILE__, __LINE__)
#define SAFE_REALLOC(a,b) 	SAFE_realloc((void *) (a), (size_t) (b), __FILE__, __LINE__)
#define SAFE_FREE(p) 		do {if ((p)) {free((p)); (p) = NULL;}} while(0)

#define SAFE_STALLOC(st)	((struct st *)SAFE_MALLOC(sizeof (struct st)))
#define SAFE_ZALLOC(s,num) ((s*)SAFE_MALLOC((sizeof (s)) * (long)(num)))

#endif /* _SAFE_ALLOC_H_ */
