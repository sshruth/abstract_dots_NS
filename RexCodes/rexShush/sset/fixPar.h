#define NUM_POS 12

/*
 * Structure that defines stimulus positions
 */
struct parameters {
	int lum;
	int dim;
	int size;
};

/*
 * array of positions ranging from -10 degrees to 10 degrees
 * in both the horizontal and vertical dirctions
 */
struct parameters stm_list[] = {
	{ 0, 144, 6 },
	{ 64, 192, 6 },
	{ 96, 160, 6 },
	{ 160, 224, 6 },
	{ 0, 144, 8 },
	{ 64, 192, 8 },
	{ 96, 160, 8 },
	{ 160, 224, 8 },
	{ 0, 144, 10 },
	{ 64, 192, 10 },
	{ 96, 160, 10 },
	{ 160, 224, 10 },
};
