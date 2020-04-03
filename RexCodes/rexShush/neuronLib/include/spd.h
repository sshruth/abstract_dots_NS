
#ifndef min_
#define min_(a,b) ((a) < (b) ? (a) : (b))
#endif

extern int rows, cols, lht;
extern int x_length, y_length;
extern int box_flag;

#define TOP		8000
#define R_EDGE		10660
#define MAX_HEIGHT	8000
#define MAX_LENGTH	10660
#define THT		200
#define LHT		150	/* minimum label height */
#define DLHT		 25	/* change in label height */
#define MXLHT		250	/* biggest label height */
#define X_MARG		150
#define Y_MARG		100
#define X_LENGTH	((MAX_LENGTH / MAXCOL) - LHT)
#define Y_LENGTH	((MAX_HEIGHT / MAXROW) - Y_MARG)
#define MAXROW		8
#define THICK		10
#define THIN		1

#define RAS_BOT	1000
#define RAS_NLINE	75
#define SD_BOT	5000
#define SD_NLINE	1000
#define HS_BOT	500
#define EDGE	250
#define INTER	500
#define	TIKHT	8
#define SUM_HT	3200
#define FREQ	5
#define MAXLINES	2000

#define PLOT_WIDTH 1000
#define CELL_WIDTH 1200
#define L_EDGE 400
#define CELL_HT 900
