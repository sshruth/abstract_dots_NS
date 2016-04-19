#include "spd.h"
     
#define RASTER (01 << 0)
#define ANALOG (01 << 1)
#define LAG_COMP (01 << 2)
#define CORR_HISTO (01 << 3)
#define SHIFT_HISTO (01 << 4)
#define LAMBDA_HISTO (01 << 5)
#define ANA_OFFSET (01 << 6)
#define IN_ARGS 40

extern int color;
extern int row_count;
extern int file_count;
#define MAX_TRIALS 4000
#define MAX_SPIKES 1000

#define TL_SZ	1000
#define NM_SZ	256
extern char fnames[IN_ARGS][NM_SZ];

struct trig_list {
	int r_width;	/* plot width in msec */
	int r_latency;	/* latency for the trigger */
	int r_stim;	/* the first bar length under the data */
	int r_stim2;	/* the second bar length under the data */
	int r_stim_start; /* how far to offset the bar start, signed */
	int r_stim2_start;  /* offset for bar 2 */
	float ana_height;	/* calibration height for plotting */
	float zero_height;	/* proportion for zero in analog plot */
	int cell_flags; /* bit flags */
	int t_signal;	/* for the analog channel */
	int unit_code;  /* so that rasters for different units can appear */
	float r_sigma;	/* sigma for kernel - if zero adapt */
	char t_specnm[100]; /* specnam for analog spec file */
};
extern struct trig_list trig_list[1000];
extern int tlist_index;

struct cell {
	int file_index;
	int trig;
	int params;
	int t_count;
	int xpos;
	int ypos;
};

extern struct cell cells[MAXROW * MAXROW];


