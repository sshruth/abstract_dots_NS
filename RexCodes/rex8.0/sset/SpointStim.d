#include <math.h>
#include <time.h>

#include "pcsSocket.h"
#include "GLcommand.h"

#define WIND0 			0
#define STIMCD			1100
#define STOFFCD			1101
#define ERRCD			1031
#define MAX_OBJECTS 	20		/* the number of objects used to start Vex */

clock_t seed;

/* menu variables must be initialized to legal values so that rinitf can call setPntr() */
float pedRad = 100.0;		/* radius of pedestal in tenths of a degree */
float pntRad = 50.0;		/* length of pointer in tenths of a degree */
float pntWidDeg = 150.0;	/* angle between vertex A and vertex B or C */
float pntWidRad = (150.0 / 360.0) * (M_PI * 2.0);			/* pointer angle in radians */
float pntDirDeg = 0.0f;		/* direction of pointer in degrees */
float pntDirRad = 0.0f;
float pedWhite = 0.2f;		/* proportion of pedestal that is white */
float pntWhite = 0.9f;		/* proportion of pointer that is white */
float stimX = 0.0f;			/* X coordinate of the set of objects */
float stimY = 0.0f;			/* Y coordinate of the set of objects */
int chkSiz = 2;

/* vertices of pointer triangle */
int Ax, Ay;
int Bx, By;
int Cx, Cy;

/* array to hold x and y coordinates of checks in pedestal and pointer
 * no more than 4000 coordinates (2000 points) can be sent in a single message
 */
int pointArray[4000];

int numPedestal;				/* number of checks in pedestal */
int numPointer;					/* number of checks in the pointer */
int totalPoints;				/* total number of points sent to one object */
int numObjects;					/* number of objects needed for pointer stimulus */
int objectNum;					/* number of the object being loaded */
int objectList[MAX_OBJECTS];	/* list of objects to load (max 255) */
int objOn[MAX_OBJECTS] = { OBJ_ON };
int objOff[MAX_OBJECTS] = { OBJ_OFF };
float objectX[MAX_OBJECTS];		/* X coordinates of objects (max 255) */
float objectY[MAX_OBJECTS];		/* Y coordinates of objects (max 255) */

/* RAN1 from Numerical Recipes in C pp. 210-211
 * provides uniform random numbers between 0.0 and 1.0
 * that are less likely to have sequential correlations
 * than most system supplied rand() functions
 */
#define M1      259200
#define IA1     7141
#define IC1     54773
#define RM1     (1.0 / M1)
#define M2      134456
#define IA2     8121
#define IC2     28411
#define RM2     (1.0 / M2)
#define M3      243000
#define IA3     4561
#define IC3     51349

float ran1(clock_t *idnum)
{
	static long ix1, ix2, ix3;
	static float r[98];
	static short iff = 0;

	float temp;
	short j;

	if (*idnum < 0 || iff == 0) {
		iff = 1;
		ix1 = (IC1 - (*idnum)) % M1;
		ix1 = (IA1 * ix1 + IC1) % M1;
		ix2 = ix1 % M2;
		ix1 = (IA1 * ix1 + IC1) % M1;
		ix3 = ix1 % M3;

		for (j = 1; j <= 97;j++) {
			ix1 = (IA1 * ix1 + IC1) % M1;
			ix2 = (IA2 * ix2 + IC2) % M2;
			r[j] = (ix1 + ix2 * RM2) * RM1;
		}

		*idnum = 1;
	}

	ix1 = (IA1 * ix1 + IC1) % M1;
	ix2 = (IA2 * ix2 + IC2) % M2;
	ix3 = (IA3 * ix3 + IC3) % M3;
	j = 1 + ((97 * ix3) / M3);

	if (j > 97 || j < 1) fprintf(stderr,"RAN1: This cannot happen.");

	temp = r[j];
	r[j] = (ix1 + ix2 * RM2) * RM1;

	return(temp);
}

int SQUARE(int a)
{
	return(a * a);
}

/* this variable access function sets up the pedestal and pointer when variables change */
int pedRad_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	setPntr();

	return(0);
}

int pntRad_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	setPntr();

	return(0);
}

int pntWid_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	float val;

	/* code executed before global variable is set */
	if(flag & ME_BEF) {
		if(*astr != 0) {
			val = (float)atof(astr);
			if((val < 130.0) || (val > 170.0)) {
				rxerr("pntDir_vaf(): bad pointer direction");
				return(1);		/* returning non-zero prevents change of global variable */
			}
		}
	}
	/* code executed after global variable is set */
	else {
		pntWidRad = (pntWidDeg / 360.0) * (M_PI * 2.0);	/* convert degrees to radians */
		setPntr();
	}

	return(0);
}

int pntDir_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	float val;

	/* code executed before global variable is set */
	if(flag & ME_BEF) {
		if(*astr != 0) {
			val = (float)atof(astr);
			if((val < 0.0) || (val > 360.0)) {
				rxerr("pntDir_vaf(): bad pointer direction");
				return(1);		/* returning non-zero prevents change of global variable */
			}
		}
	}
	/* code executed after global variable is set */
	else {
		pntDirRad = (pntDirDeg / 360.0) * (M_PI * 2.0);	/* convert degrees to radians */
		setPntr();
	}

	return(0);
}

int pedWhite_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	float val;

	/* code executed before global variable is set */
	if(flag & ME_BEF) {
		if(*astr != 0) {
			val = (float)atof(astr);
			if((val < 0.0) || (val > 1.0)) {
				rxerr("pedWhite_vaf(): bad pedestal proportion white");
				return(1);		/* returning non-zero prevents change of global variable */
			}
		}
	}
	/* code executed after global variable is set */
	else {
		setPntr();
	}

	return(0);
}

int pntWhite_vaf(int flag, MENU *mp, char *astr, VLIST *vlp, int *tvadd)
{
	float val;

	/* code executed before global variable is set */
	if(flag & ME_BEF) {
		if(*astr != 0) {
			val = (float)atof(astr);
			if((val < 0.0) || (val > 1.0)) {
				rxerr("pntWhite_vaf(): bad pointer proportion white");
				return(1);		/* returning non-zero prevents change of global variable */
			}
		}
	}
	/* code executed after global variable is set */
	else {
		setPntr();
	}

	return(0);
}

/*
 * This function sets up the parameters for the pedestal and the pointer
 * It is called from the rinitf function when the paradigm is first started
 * and from the variable access functions whenever one of the parameters
 * are changed. This function is not called from a state
 */
int setPntr()
{
	double costhetaA, costhetaB, costhetaC;
	double sinthetaA, sinthetaB, sinthetaC;
	double transx;
	double transy;
	double a;				/* length of pointer side A - B */
	double b;				/* length of pointer side B - C */
	double c;				/* length of pointer size C - A */
	double s;				/* length of pointer perimeter */
	double pointerArea;
	int i;

	/* seed the random number generator */
	seed = clock() * -1;

	/* compute the number of checks in the pedestal */
	numPedestal = (int)((M_PI * (pedRad * pedRad)) * pedWhite);

	/* find the vertices of the pointer triangle */
	costhetaA = cos(pntDirRad);
	sinthetaA = sin(pntDirRad);
	costhetaB = cos(pntDirRad + pntWidRad);
	sinthetaB = sin(pntDirRad + pntWidRad);
	costhetaC = cos(pntDirRad - pntWidRad);
	sinthetaC = sin(pntDirRad - pntWidRad);

	Ax = (int)rint(costhetaA * pntRad);
	Ay = (int)rint(sinthetaA * pntRad);
	Bx = (int)rint(costhetaB * pntRad);
	By = (int)rint(sinthetaB * pntRad);
	Cx = (int)rint(costhetaC * pntRad);
	Cy = (int)rint(sinthetaC * pntRad);

/*	this algorithm makes the direction of the pointer reflect around the X axis
 *	That is, specifying an upward angle such as 45 or 135 degrees, makes the pointer
 *  point down.

	transx = -pntRad / 6.0;	
	transy = -pntRad / 2.0;

	Ax = (int) rint((costheta * (0.0 + transx)) + (sintheta * ((pntRad / 2.0) - (pntRad / 6.0) + transy)));
	Ay = (int) rint((-sintheta * (0.0 + transx)) + (costheta * ((pntRad / 2.0) - (pntRad / 6.0) + transy)));
	Bx = (int) rint((costheta * (0.0 + transx)) + (sintheta * ((pntRad / 2.0) + (pntRad / 6.0) + transy)));
	By = (int) rint(-sintheta * (0.0 + transx)) + (costheta * ((pntRad / 2.0) + (pntRad / 6.0) + transy)));
	Cx = (int) rint((costheta * ((pntRad / 2.0) + transx)) + (sintheta * ((pntRad / 2.0) + transy)));
	Cy = (int) rint((-sintheta * ((pntRad / 2.0) + transx)) + (costheta * ((pntRad / 2.0) + transy)));
*/

	/* find the area of the pointer using Heron's formula */
	a = sqrt((double)(SQUARE(Ax - Bx) + SQUARE(Ay - By)));	// length of side A - B
	b = sqrt((double)(SQUARE(Bx - Cx) + SQUARE(By - Cy)));	// length of side B - C
	c = sqrt((double)(SQUARE(Cx - Ax) + SQUARE(Cy - Ay)));	// length of side C - A
	s = (a + b + c) / 2.0;									// half the perimeter
	pointerArea = sqrt(s * (s - a) * (s - b) * (s - c));

	/* computer the number of checks in the pointer */
	numPointer = (int)(pointerArea * (double)pntWhite);

	/* determine the number of objects to use */
	numObjects = ((numPedestal + numPointer) / 2000) + 1;
	if(numObjects < MAX_OBJECTS) {
		for(i = 0; i < numObjects; ++i) {
			objectList[i] = i + 1;
			objectX[i] = stimX;
			objectY[i] = stimY;
			objOn[i] = OBJ_ON;
			objOff[i] = OBJ_OFF;
		}
	}
	else {
		rxerr("Too many objects needed for specified coverage");
		rxerr("Reduce white proportion in pedestal or pointer");
		rxerr("or reduce radius of pedestal or pointer");

		numObjects = 0;
	}

	/* set objectNum to 1 more than the number of objects
	 * so that it can be decremented at the start of loadPntr()
	 * to hold the number of the object to load */
	objectNum = numObjects + 1;

	return(0);
}

/* this action is called once for each object that needs to be loaded */
int loadPntr()
{
	double q;
	double r;
	double a;
	double b;
	double c;
	short int x;
	short int y;
	int i;					/* number of coordinates */
	int j;					/* number of coordinate pairs */
	int k;

	/* decrement the variable objectNum so that it holds the number of
	 * the object to load on this iteration of the loop
	 */
	--objectNum;

	/* sanity check */
	if(objectNum < 1) objectNum = 1;

	/* load the pedestal checks for this object */
	i = 0;
	totalPoints = 0;
	memset(pointArray, 0, sizeof(pointArray));

	/* generate an array of random, uniformly distributed points
	 * that lie within the pedestal
	 */
	for(k = 0; k < (numPedestal / numObjects); ++k) {
		q = ran1(&seed) * (M_PI * 2.0);					/* pick a random angle */
		r = sqrt(ran1(&seed)) * (double)pedRad;			/* pick a random distance from the center */

		x = (short int)rint(r * cos(q));				/* x coordinate of the point */
		y = (short int)rint(r * sin(q));				/* y coordinate of the point */

		pointArray[i++] = x;
		pointArray[i++] = y;

		totalPoints++;
	}

	/* generate an array of random, uniformly distributed points
	 * that lie within the pointer using barycentric coordinates
	 */
	for(k = 0; k < (numPointer / numObjects); ++k) {
		/* generate three coefficients such that a + b + c = 1.0 */
		/* first generate coefficents a and b using random numbers */
		a = ran1(&seed);
		b = ran1(&seed);

		/* if the sum of a and b is greater than 1.0, reflect a and b around 1.0 */
		if((a + b) > 1.0) {
			a = 1.0 - a;
			b = 1.0 - b;
		}

		/* now compute c by subtracting a and b from 1.0 */
		c = 1.0 - a - b;

		/* compute the X and Y coordinates of the point by weighting the X and Y coordinates
		 * of the vertices of the pointer by the random coefficients and summing
		 */
		x = (short int)rint(((a * Ax) + (b * Bx) + (c * Cx)));
		y = (short int)rint(((a * Ay) + (b * By) + (c * Cy)));

		pointArray[i++] = x;
		pointArray[i++] = y;

		totalPoints++;
	}

	/* this return statement will cause Rex to load an ecode
	 * reflecting the pointer direction
	 */
	return(pntDirDeg + 2000);
}

void
rinitf(void)
{
	char *vexHost = "lsr-jwmvex";		/* if you are going to use Vex */
	char *mexHost = "lsr-jwmmex";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);		/* if you are going to use Vex */
	pcsConnectMex(mexHost);		/* if you are going to use Mex */
	
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_cntrl(WIND0, WD_ON);

	setPntr();					/* set the default pointer parameters */
}

/*
 * statelist menu
 */
VLIST state_vl[] = {
	"pedestal_radius",		&pedRad, NP, pedRad_vaf, ME_AFT, ME_FLOAT,
	"pointer_radius",		&pntRad, NP, pntRad_vaf, ME_AFT, ME_FLOAT,
	"pointer_width",		&pntWidDeg, NP, pntWid_vaf, ME_AFT, ME_FLOAT,
	"pointer_dir",			&pntDirDeg, NP, pntDir_vaf, ME_BEF | ME_AFT, ME_FLOAT,
	"pedestal_%_white",		&pedWhite, NP, pedWhite_vaf, ME_BEF | ME_AFT, ME_FLOAT,
	"pointer_%_white",		&pntWhite, NP, pntWhite_vaf, ME_BEF | ME_AFT, ME_FLOAT,
	"pedestal_center_X",	&stimX, NP, NP, 0, ME_FLOAT,
	"pedestal_center_Y",	&stimY, NP, NP, 0, ME_FLOAT,
	"check_size",			&chkSiz, NP, NP, 0, ME_DEC,
	NS,
};
char hm_sv_vl[]= "
pedestal_radius:	radius of pedestal [tenths of a degree]
pointer_radius:		length of long side of pointer [tenths of a degree]
pointer_width:		degrees between vertex A and vertex B
pointer_dir:		pointer direction [degrees]
pedestal_%_white:	proportion of pedestal area covered by checks [0.0 to 1.0]
pointer_%_white:	proportion of pointer area covered by checks [0.0 to 1.0]
pedestal_center_X:	X coordinate of pedestal/pointer on screen [tenths of a degree]
pedestal_center_Y:	Y coordinate of pedestal/pointer on screen [tenths of a degree]
check_size:			size of checks in pedestal and pointer [pixels]";

/*
 * list of user menus
 */
MENU umenus[] = {
	"state_vars",	&state_vl,	NP,	NP,	0,	NP,	hm_sv_vl,
	NS,
};

/*
 *
 */
USER_FUNC ufuncs[] = {
	{""},
};

/*
 * User-supplied real-time variables
 */
RTVAR rtvars[] = {
	{"number of objects",	&numObjects},
	{"", 0},
};

%%
id 100
restart rinitf
main_set {
status ON
begin	first:
		to disabl
	disabl:
		to enable on -PSTOP & softswitch  /*is bar enabled?*/
	enable:
		code ENABLECD
	    to loadLoop
	loadLoop:
		/* load up pointArray with check coordinates for pedestal and pointer */
		do loadPntr()
		to sendPntr on 0 < objectNum
		to error
	sendPntr:
		/* send the array of points to Vex */
		do PvexLoadPointArray(&objectNum, &totalPoints, &chkSiz, pointArray);
		to tstLoop on 0 % tst_rx_new
	tstLoop:
		to setStmPos on 1 = objectNum /* when objectNum is 1 all objects have been loaded */
		to loadLoop
	setStmPos:
		do PvexStimLocation(&numObjects, objectList, objectX, objectY)
		to stncmd on 0 % tst_rx_new
	stncmd:
		do PvexSwitchStim(&numObjects, objectList, objOn)
		to stimon on 0 % tst_rx_new
	stimon:
		code STIMCD
		time 1000
		rand 1000
		to stfcmd
	stfcmd:
		do PvexSwitchStim(&numObjects, objectList, objOff)
		to stimoff on 0 % tst_rx_new
	stimoff:
		code STOFFCD
		time 500
		to first
	error:
		code ERRCD
		time 2000
		to first
}
