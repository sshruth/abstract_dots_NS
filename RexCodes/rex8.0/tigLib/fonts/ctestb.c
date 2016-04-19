#include <stdio.h>
#include "/usr/local/include/tig.h"

extern double atof();


main(argc,argv)
int argc;
char **argv;
{

	if( argc < 2 || argc > 3 ) {
		fprintf(stderr,"Usage:\nctestb string [num]\n");
		exit(0);
	}
	if( argc == 2 )
		pltstr(argv[1]);
	else	pltstr(argv[1],atof(argv[2]));
}
