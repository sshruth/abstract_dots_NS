/******************************************************************
**  RCSID: $Id: safe_alloc.c,v 1.2 2002/02/08 01:42:05 jig Exp $
** Program: dowl
**  Module: safe_alloc
**  Author: mazer
** Descrip: wrappers for memory allocation routines
**
** Prefix: SAFE
**
** Revision History (most recent last)
**
** Sun Mar 19 10:56:29 1995 mazer@asio
**  creation date
**
** Sun Mar 19 11:46:45 1995 mazer@asio
**  note: maloc(0) might return NULL, so it's technically not
**  an out-of-memory condition, so it's special cased..
**
** 12.12.95 jig
**   Cleaned up a little
**
*******************************************************************/

/* INCLUDES */
#include "safe_alloc.h"
#include <stdio.h>
#include <malloc.h>

/* PUBLIC ROUTINES */
void *SAFE_calloc(size_t nmemb, size_t size, char *file, int line)
{
  void *x;

  if (!(x = calloc(nmemb, size)) && size)
      fprintf(stderr, "calloc error in file:%s line:%d\n", file, line);
  return(x);
}

void *SAFE_malloc(size_t size, char *file, int line)
{
  void *x;

  if (!(x = malloc(size)) && size)
      fprintf(stderr, "malloc error in file:%s line:%d\n", file, line);
  return(x);
}

void *SAFE_realloc(void *ptr, size_t size, char *file, int line)
{
  void *x;

  if (!(x = realloc(ptr, size)) && size)
      fprintf(stderr, "realloc error in file:%s line:%d\n", file, line);
  return(x);
} 
