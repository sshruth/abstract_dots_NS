/* HDDN.H -- Header for hidden-line surface generation */

#define MIN_RES	2	/* Minimum resolution in 1000ths of an inch */

/* XFORM
 *	structure to hold all variables needed to perform transformation
 * from user space to TIG page
 */
typedef struct {
	float xtrans, ytrans, ztrans;	/* user-unit translations
					 * to center of rotation */
	float alpha, beta, theta;	/* rotation angles (in deg), same as TIG */
	float zht;			/* distance to observer on +z-axis */
	float hscl, vscl;		/* user-to-TIG scale factor */
	float hoff, voff;		/* user-to-TIG offset */
	float xlo, xhi;			/* min, max in user-units */
	float ylo, yhi;
	float zlo, zhi;
} XFORM;

/* BOUNDS
 *	structure for lower and upper bounds of plot space for
 * hidden lines
 */
typedef struct {
	int lower;
	int upper;
} BOUNDS;
