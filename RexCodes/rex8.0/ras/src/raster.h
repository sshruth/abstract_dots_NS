/*
 * NOTICE:  This code was developed by employees of the
 * U.S. Government.  This version is distributed,
 * without guarantees or warranties of any kind,
 * for noncommercial use only.  The U.S. Government
 * holds the copyright.
 */
#ifndef RASTER_H
#define RASTER_H

#define MAXLENGTH	10000 /* maximum length of a trial */
#define NOHOLE_WIDTH	MAX_Y 	/* no hole paper page height in tig units */
#define MAX_LENGTH	MAX_X	/* page width in tig units */
#define THICK		10
#define THIN		1
#define CLSFILE		0
#define CLSFLTR		0
#define R_ALIGN		1
#define R_LEFT		2
#define R_UNIT		1
#define R_MEANS		2
#define R_ROW		0
#define R_COL		1
#define MARGINS		100

/* defines for the cell plot variables flag */
#define PLOT_SET	1
#define PLOT_ECODE	2
#define PLOT_SIGMA	4
#define PLOT_RASTER	8
#define PLOT_DENSITY	16
#define PLOT_UNITS	32
#define PLOT_MARK	64
#define PLOT_SYMBOL	128
#define PLOT_MSIZE	256
#define PLOT_LABEL	512
#define PLOT_LFONT	1024
#define PLOT_LSIZE	2048
#define PLOT_LPOS	4096
#define MAX_PLOT_VARS	13

/* defines for the cell graph variables flag */
#define GRAPH_START	1
#define GRAPH_DUR	2
#define GRAPH_RATE	4
#define GRAPH_TIC	8
#define GRAPH_YAXIS	16
#define GRAPH_NUMBER	32
#define GRAPH_NFORMAT	64
#define GRAPH_NFONT	128
#define GRAPH_NSIZE	256
#define GRAPH_XLABEL	512
#define GRAPH_YLABEL	1024
#define GRAPH_LFONT	2048
#define GRAPH_LSIZE	4096
#define MAX_GRAPH_VARS	13

#define PL_TRIALS	1
#define PL_MEAN		2
#define PL_ERROR	4
#define PL_LAG		8
#define PL_ADAPT	16

/* make typdefs for tig structures to make defining easier */
typedef struct TIGvec TIGVEC;
typedef struct darray DARRAY;
typedef struct space TIGSPACE;
typedef struct cinfo CINFO;
typedef struct TIGIO TIGIO;

typedef unsigned short USHORT;

/* This structure holds the definition of a raster graph */
typedef struct Rgrph {
	int start;		/* start of data display relative to alignment mark (msec) */
	int interval;		/* amount of data to plot (msec) */
	int maxFreq;		/* maximum expected unit firing rate */
	int timeMark;		/* interval between X axis tick marks in msec */
	int yAxes;		/* flag controlling the number and locations of Y axes */
	int numberAxes;		/* flag controlling axis enumeration */
	int Nformat;		/* format string for the numbers on the axis */
	int Nfont;		/* font to use for the numbers */
	int Nsz;		/* size of the numbers */
	int Lfont;		/* font to use for the axis label */
	int Lsz;		/* size of the axis label */
	char XaxisLabel[128];	/* structure defining the X axis of the plot */
	char YaxisLabel[128];	/* structure defining the left Y axis of the plot */
} GRAPH;

/* this structure holds the definition of the graph label */
typedef struct Dlbl {
	char label[128];	/* text of label */
	double x;		/* relative X coordinate of label */
	double y;		/* relative Y coordinate of label */
	int sz;			/* size of the text */
	int font;		/* font for text */
} LABEL;

/* this structure holds the definition of the ecode marks */
typedef struct Dem {
	int rMark;		/* ecode to plot */
	int sz;			/* size of the symbol */
	char symbolString[32];	/* name of the symbol used to plot the ecode */
	char symbolChar;	/* the actual symbol used to plot the ecode */
} ECODEMARK;

/* this structure holds the information needed to plot a signal */
typedef struct Dsig{
	char sigName[32];	/* array to hold the name of the signal */
	int sigNum;		/* number of the signal */
	int color;		/* color to draw the signal */
} SIGNAL;

/* This structure holds the definition of a raster plot */
typedef struct {
	int setNum;		/* number of the selection set */
	int cCode;		/* ecode identifying the condition to plot */
	double sigma;		/* adaptive kernel start value for unit data */
	int trialFlag;		/* whether to plot individual trials */
	int meanFlag;		/* whether to plot mean data */
	SIGNAL sigInfo[10];	/* array of signals info for this plot */
	int nSignals;		/* number of signals to plot */
	int maxSignals;		/* maximum number of signals allocated so far */
	GRAPH graph;		/* graphing properties structure */
	ECODEMARK ecodeMark;	/* pointer to list of other ecodes to plot */
	int markFlag;		/* number of other ecodes to plot */
	LABEL plotLabel;	/* plot label */
	int labelFlag;		/* whether or not the plot has a label */
	int maxLines;		/* maximum number of raster lines */
} PLOT;

/* This structure holds the definition of a page cell */
typedef struct Rcell {
	int RCindex;		/* row-column index of cell in the 8 x 8 array of cells */
	int colN;		/* column in which this cell is located */
	int rowN;		/* row in which this cell is located */
	int left;		/* left coordinate of the cell in TIG units */
	int bottom;		/* bottom coordinate of the cell in Tig units */
	PLOT plotInfo;		/* pointer to plot info for this cell */
} CELL;

/* This structure holds the definition of a raster page */
typedef struct Rpage {
	USHORT plotVariables;	/* flag indicating which plot variables were changed */
	USHORT graphVariables;	/* flag indicating which graph variables were changed */
	int pgLength;		/* length of the page in  TIG units */
	int pgWidth;		/* width of the page in TIG units */
	int nRows;		/* number of rows of cells in this page */
	int nCols;		/* number of columns of cells in this page */
	int cellWidth;		/* width of the cells in this page in TIG units */
	int cellHeight;		/* height of the cells in this page in TIG units*/
	CELL cellsInfo[64];	/* array of cells info for this page */
	int nDefCells;		/* number of cells to display in this page */
	int maxCells;		/* number of cells malloc'ed in this page */
	int curCellIndx;	/* index of current cell */
	LABEL pageLabel;	/* label for the page */
	int labelFlag;		/* whether or not there is a label for this page */
} PAGE;

/* This structure is a basket to hold pointers to the various raster components */
typedef struct {
	PAGE *pages;		/* pointer to an array of page structures */
	int nPages;		/* number of pages needed */
	int curPageIndx;	/* index of the current page */
	int curDrawPage;	/* index of the current page being drawn */
	int newRasters;		/* flag indicating new rasters are being defined */
} RASTER;

#endif /* RASTER_H */
