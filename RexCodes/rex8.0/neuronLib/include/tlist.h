struct tbl {
	int para;
	int *trialcdes;
	int *tl;
	char *comment;
};

extern int *st_ptr;
extern int stack[];
extern int N;
extern int para_num;
extern int tbeg, tend;
extern struct tbl states[];
extern FILE *outfd;
