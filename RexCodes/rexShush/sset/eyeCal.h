#define BI_STIM_16_POS	40
#define	UNI_STIM_16_POS	20
#define MAX_POS			40

typedef struct position {
	int fpx;
	int fpy;
	int ex;
	int ey;
	int oex;
	int oey;
} calPos_t;

typedef struct location {
	int x;
	int y;
	int c;
} calLoc_t;


calLoc_t bi16grid[] = {
	{ -45,	45,   1 },
	{ -45,	45,   1 },
	{ -45,	45,   1 },
	{ -15,	45,   2 },
	{ -15,	45,   2 },
	{ -15,	45,   2 },
	{  15,	45,   4 },
	{  15,	45,   4 },
	{  15,	45,   4 },
	{  45,	45,   5 },
	{  45,	45,   5 },
	{  45,	45,   5 },

	{ -45,	15,   6 },
	{ -45,	15,   6 },
	{ -45,	15,   6 },
	{ -15,	15,   7 },
	{  15,	15,   9 },
	{  45,	15,  10 },
	{  45,	15,  10 },
	{  45,	15,  10 },

	{ -45,	-15, 16 },
	{ -45,	-15, 16 },
	{ -45,	-15, 16 },
	{ -15,	-15, 17 },
	{  15,	-15, 19 },
	{  45,	-15, 20 },
	{  45,	-15, 20 },
	{  45,	-15, 20 },

	{ -45,	-45, 21 },
	{ -45,	-45, 21 },
	{ -45,	-45, 21 },
	{ -15,	-45, 22 },
	{ -15,	-45, 22 },
	{ -15,	-45, 22 },
	{  15,	-45, 24 },
	{  15,	-45, 24 },
	{  15,	-45, 24 },
	{  45,	-45, 15 },
	{  45,	-45, 15 },
	{  45,	-45, 15 }
};

calLoc_t lf16grid[] = {
	{ -45,	45,   1 },
	{ -45,	45,   1 },
	{ -45,	45,   1 },
	{ -15,	45,   2 },
	{ -15,	45,   2 },
	{ -15,	45,   2 },

	{ -45,	15,   6 },
	{ -45,	15,   6 },
	{ -45,	15,   6 },
	{ -15,	15,   7 },

	{ -45,	-15, 16 },
	{ -45,	-15, 16 },
	{ -45,	-15, 16 },
	{ -15,	-15, 17 },

	{ -45,	-45, 21 },
	{ -45,	-45, 21 },
	{ -45,	-45, 21 },
	{ -15,	-45, 22 },
	{ -15,	-45, 22 },
	{ -15,	-45, 22 }
};

calLoc_t rt16grid[] = {
	{  15,	45,   4 },
	{  15,	45,   4 },
	{  15,	45,   4 },
	{  45,	45,   5 },
	{  45,	45,   5 },
	{  45,	45,   5 },

	{  15,	15,   9 },
	{  45,	15,  10 },
	{  45,	15,  10 },
	{  45,	15,  10 },

	{  15,	-15, 19 },
	{  45,	-15, 20 },
	{  45,	-15, 20 },
	{  45,	-15, 20 },

	{  15,	-45, 24 },
	{  15,	-45, 24 },
	{  15,	-45, 24 },
	{  45,	-45, 15 },
	{  45,	-45, 15 },
	{  45,	-45, 15 }
};
