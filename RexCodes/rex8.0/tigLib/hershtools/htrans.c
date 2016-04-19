/*
		 HTRANS

Translate a range of character definitions in occidental.hsy into
an editable file format.

compile: cc htrans -e1600
link:    li htrans -n

Greg Lee, 10/84

*/
#include <stdio.h>
#include <sys/file.h>

char cbuf[1024];
int cnum, firstc, lastc, cocnt, fd, i, maxi;
char dummy, letter, bndflag, fname[20];
FILE *fchr;

main()
{	/*int fd, i, maxi;
	char fname[20];*/


	printf("Htrans - Create file of plot points from Hershey database.\n");

	if ((fd = open("occidental.hsy",O_RDONLY,"r")) == NULL)
	{	printf("File occidental.hsy needed in current directory.");
		exit(1);
	}

	printf("Output .chr file name?  : "); scanf("%s", fname);
	strcat(fname, ".chr");
	if ((fchr = fopen(fname, "w")) == NULL)
	{	printf("Can't open %s for output.", fname);
		exit(1);
	}

	printf("first number? : "); scanf("%d", &firstc);
	scanf("%c", &dummy);
	printf("first letter? : "); scanf("%c", &letter);
	if (letter > ' ')
	{	scanf("%c", &dummy);
		printf("last letter?  : ");
		scanf("%c", &bndflag);
		lastc = firstc + bndflag - letter;
	}
	else { printf("last number?  : "); scanf("%d", &lastc); }

	cnum = 0; cocnt = 0;
	bndflag = 1;
	i = 2;

	maxi = read(fd, cbuf, 1024);

	for (; cnum <= lastc ; i++,i++)
	{	if (i >= maxi)
		{	i = 0;
			if (maxi < 1024) break;
			maxi = read(fd, cbuf, 1024);
			if (!maxi) break;
		}

		if (dspc(cbuf[i], cbuf[i+1]))
		 if (letter > ' ' && letter <= 127)
			fprintf(fchr, "'%c': ", letter++);
		 else fprintf(fchr,"%d: ", cnum);
	}

	fprintf(fchr, "\n");
	fclose(fchr);

}

dspc(b1, b2)
char b1, b2;
{	int i1, i2;

	i1 = b1 - 64; i2 = b2 - 64;

	if (i1 == -64 && i2 == -64)
	{	if (cnum >= firstc) fprintf(fchr," End Char\n");
		cnum++;
		cocnt = 0; bndflag = 1;
		if (cnum >= firstc && cnum <= lastc) return(cnum);
		return(0);
	}

	if (cnum < firstc) return(0);

	if (i1 == -64 && i2 == 0)
	{	fprintf(fchr," Pen Up\n    ");
		cocnt = 0;
	}
	else
	{	if (cocnt >8) {cocnt = 0; fprintf(fchr,"\n    ");}

		if (bndflag)
		{ fprintf(fchr,"Bounds(%d,%d)", i1,i2); bndflag = 0; }
		else fprintf(fchr,"(%d,%d)", i1,i2);
		cocnt++;
	}

	return(0);
}
