/* rex2vax.h */

#define DATA_SIZE	1024

#define MAGIC_STR	"REX M"

#define NULLF ((FILE *) 0)
#define NULLC ((char *) 0)

/* SYSTEM FLAGS for sysflags */
#define F_NOFILE	01

/* function definitions */
char *nopath();

/* TYPEDEFS */
typedef struct bytes {
	char lobyte;
	char hibyte;
} * BYTEP;

typedef struct words {
	short int loword;
	short int hiword;
} * WORDP;

/*
 * The format message.
 * This will be in the header of any file converted to
 * have the VAX compatible ordering of words in longs.
 * NOTE:  this string must have a \n at the end.
 */
char *format_msg = "VAX format\n";
