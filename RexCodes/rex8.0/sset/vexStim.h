#define NUM_STIM_LOC 8
#define NUM_STIM_TYPE 30
#define NUM_FLOW_TYPE 5

#define WALSH_STIM 2001
#define RANDOM_STIM 2002
#define ANNULUS_STIM 2003
#define BAR_STIM 2004
#define FLOW_STIM 2005
#define USER_STIM 2006
#define TIFF_STIM 2007
#define OKN_STIM 2008
#define RAMP_STIM 2009
#define FLOW_RAMP_STIM 2010
#define SINE_STIM 2011
#define PNG_STIM 2012
#define SOUND_STIM 2013

#define FLOWON 1200
#define FLOWOFF	1201
#define OKNON 1202
#define OKNOFF 1203
#define RAMPON 1204
#define RAMP_BACK 1205
#define RAMPOFF 1206
#define FLOW_RAMPON 1207
#define SINEON 1208
#define SINEOFF	1209

struct stimLoc_t {
	int x;
	int y;
};

struct stimLoc_t loc_list[] = {
	{0, 50},
	{50, 50},
	{50, 0},
	{50, -50},
	{0, -50},
	{-50, -50},
	{-50, 0},
	{-50, 50},
};

struct stimType_t {
	int typeStim;
	int forgrndRed;
	int forgrndGreen;
	int forgrndBlue;
	int bakgrndRed;
	int bakgrndGreen;
	int bakgrndBlue;
};

struct stimType_t stm_list[] = {
	{ WALSH_STIM, 255, 255, 255, 0, 0, 0 },
	{ RANDOM_STIM, 255, 255, 255, 0, 0, 0 },
	{ ANNULUS_STIM, 255, 255, 255, 0, 0, 0 },
	{ BAR_STIM, 255, 255, 255, 0, 0, 0 },
	{ FLOW_STIM, 255, 255, 255, 0, 0, 0 },
	{ USER_STIM, 255, 255, 255, 0, 0, 0 },
	{ TIFF_STIM, 255, 255, 255, 0, 0, 0 },
	{ PNG_STIM, 255, 255, 255, 0, 0, 0 },
	{ OKN_STIM, 255, 255, 255, 0, 0, 0 },
	{ RAMP_STIM, 255, 255, 255, 0, 0, 0 },
	{ FLOW_RAMP_STIM, 255, 255, 255, 0, 0, 0 },
	{ SINE_STIM, 255, 255, 255, 0, 0, 0 },
	{ SOUND_STIM, 255, 255, 255, 0, 0, 0 },

	{ WALSH_STIM, 255, 0, 0, 0, 255, 0 },
	{ RANDOM_STIM, 255, 0, 0, 0, 255, 0 },
	{ ANNULUS_STIM, 255, 0, 0, 0, 255, 0 },
	{ BAR_STIM, 255, 0, 0, 0, 255, 0 },
	{ FLOW_STIM, 255, 0, 0, 0, 255, 0 },
	{ USER_STIM, 255, 0, 0, 0, 255, 0 },
	{ TIFF_STIM, 255, 0, 0, 0, 255, 0 },
	{ PNG_STIM, 255, 255, 255, 0, 0, 0 },
	{ OKN_STIM, 255, 0, 0, 0, 255, 0 },
	{ RAMP_STIM, 255, 0, 0, 0, 255, 0 },
	{ FLOW_RAMP_STIM, 255, 0, 0, 0, 255, 0 },
	{ SINE_STIM, 255, 255, 255, 0, 0, 0 },
	{ SOUND_STIM, 255, 255, 255, 0, 0, 0 },

	{ WALSH_STIM, 255, 255, 0, 0, 0, 255 },
	{ RANDOM_STIM, 255, 255, 0, 0, 0, 255 },
	{ ANNULUS_STIM, 255, 255, 0, 0, 0, 255 },
	{ BAR_STIM, 255, 255, 0, 0, 0, 255 },
	{ FLOW_STIM, 255, 255, 0, 0, 0, 255 },
	{ USER_STIM, 255, 255, 0, 0, 0, 255 },
	{ TIFF_STIM, 255, 255, 0, 0, 0, 255 },
	{ PNG_STIM, 255, 255, 255, 0, 0, 0 },
	{ OKN_STIM, 255, 255, 0, 0, 0, 255 },
	{ RAMP_STIM, 255, 255, 0, 0, 0, 255 },
	{ FLOW_RAMP_STIM, 255, 255, 0, 0, 0, 255 },
	{ SINE_STIM, 255, 255, 255, 0, 0, 0 },
	{ SOUND_STIM, 255, 255, 255, 0, 0, 0 }
};

struct flowPars_t {
	int xy;
	int z;
	int v;
	int r;
	int p;
	int y;
};

struct flowPars_t flow_list[] = {
	{ 45, 0, 10, 0, 0, 0 },
	{ 0, 90, 20, 0, 0, 0 },
	{ 0, 0, 0, 20, 0, 0 },
	{ 0, 0, 0, 0, 20, 0 },
	{ 0, 0, 0, 0, 0, 20 },
};
