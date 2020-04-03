#include <math.h> /* Put math first to eliminate a peculiar problem. */
#include <stdio.h>
#include "tig.h"

/* CGA */
static int xlast = -32000;
static int ylast = -32000;	/* last drawn point */
static BOUNDS *bds;	/* pointer to bounds table */
static int nbds;	/* # elts in bounds table */

/* SURFACE
 *	Draws a function of two variables as a pseudo-3-Dimensional
 * surface at the TIG origin.
 *
 * INPUT:
 *	spacep	= pointer to 3-D data space
 *	cross	= 0 -- no cross hatching
 *	     	= 1 -- cross hatching
 *	     	= 2 -- edges only
 *	hide	= 0 -- normal lines
 *	    	= 1 -- hidden lines
 *	first	= 0 -- no initialization of boundary
 *	     	= 1 -- initialize boundary
 *
 * RETURN:  -1 indicates error.
 *
 * function needed:
 *	get_uv	= pointer to function that accepts five arguments:
 *			get_uv(x, y, &u, &v, spacep)
 *			taking (x,y) grid and returning
 *			the (u,v) real TIG coordinates.
 */

surface(spacep, cross, hide, first, grid)
	int cross, hide, first, grid;
	struct space *spacep;
{
	register int x, y;
	static int u0, v0, u1, v1;
	int nx, ny, xlen;
	int nym1, xdel;
	int dr_hddn(), dr_solid();
	register int (*drw)();

	/*
	 * get ranges from space pointer.
	 * calculate length of real x-axis as
	 * longest diagonal in mapped space.  Then, if the data
	 * don't go outside of the rectangle of the axes,
	 * no rotation can produce a longer real x-axis.
	 */
	nx = spacep->xaxis.datarp->nelems;
	ny = spacep->yaxis.datarp->nelems;
	
	xlen = sqrt((double) spacep->xaxis.axscale * spacep->xaxis.axscale +
		    (double) spacep->yaxis.axscale * spacep->yaxis.axscale +
		    (double) spacep->zaxis.axscale * spacep->zaxis.axscale);

	switch(hide) {
	case 1:
		drw = dr_hddn;
		break;
	default:
		drw = dr_solid;
		break;
	}


	if (hide == 1 && first == 1)
		if (init_bnd(xlen) < 0) return(-1);

	for (y = 0, nym1 = ny - 1; y < ny; y++) {
		/*
		 * generate a line of constant y-value
		 */ 
		get_uv(0, y, &u0, &v0, spacep);
		for (x = 1; x < nx; x++) {
			get_uv(x, y, &u1, &v1, spacep);
			(*drw)(u0, v0, u1, v1);
			u0 = u1;
			v0 = v1;
		}

		/*
		 * generate the pieces of all lines of constant
		 * x-value that lie between y and y + 1
		 */
		if (cross && y < nym1)
			for (x = 0, xdel = (cross == 1 ? 1 : nx - 1);
				x < nx; x += xdel) {
				get_uv(x, y, &u0, &v0, spacep);
				get_uv(x, y+1, &u1, &v1, spacep);
				(*drw)(u0, v0, u1, v1);
			}
	} /* end x */

	fflush(stdout);
	return(0);
}

/*
 * GET_UV
 *	This function takes x,y indices into data space, and returns
 * u,v coordinates in real space.
 * INPUT:
 *	kx, ky = indices
 *	spacep = pointer to 3-D TIG space structure where
 *		xaxis = 1 dimensional array column variable
 *		yaxis = 1 dimensional array row variable
 *		zaxis = 2 dimensional array, ordered as A[ky][kx],
 *			i.e., x is the column, y is the row variable.
 * OUTPUT:
 *	fills in pointer pu and pv to the real TIG space.
 */
get_uv(kx, ky, pu, pv, spacep)
	int kx, ky, *pu, *pv;
	register struct space *spacep;
{
	register struct TIGdvec *rdvec;
	struct TIGdvec mdvec;

	/*
	 * convert data space to mapped space
	 * note: kx is column, ky is row index
	 */
	mdvec.XXpos = TIGconv(&spacep->xaxis, kx);
	mdvec.YYpos = TIGconv(&spacep->yaxis, ky);
	mdvec.ZZpos = TIGconv(&spacep->zaxis,
		(kx + ky * spacep->xaxis.datarp->nelems));
	/*
	 * convert mapped space to real space
	 */
	rdvec = TIGddot(&mdvec, &_ppmtor);

	/*
	 * fill in output values with projection on x-y plane
	 * if necessary, apply perspective transformation, since
	 * surface uses rmove(), not move()
	 */
	if (_Z_persp != 0) {
		double fac;
		fac = 1 + (rdvec->ZZpos/_Z_persp); 
		*pu = rdvec->XXpos / fac;
		*pv = rdvec->YYpos / fac;
	}
	else {
		*pu = rdvec->XXpos;
		*pv = rdvec->YYpos;
	}
}



/* INIT_BND
 *	Initialize the boundary array for a hidden line plot.
 * A boundary array is allocated if necessary.
 *
 * INPUT:
 *	xl = length of x-axis in 1000ths of an inch
 * RETURN:  -1 indicates allocation error
 */
init_bnd(xl)
	register int xl;
{
	register BOUNDS *b, *bl;
	static int x_old = -1;

	if (xl != x_old) {
		if (x_old > 0) free(bds);
		x_old = xl;
		nbds = 1 + xl;
		bds = (BOUNDS *) malloc(nbds * sizeof(BOUNDS));
		if (bds == 0) return(-1);
	}
	for (b = bds, bl = &bds[nbds]; b < bl; b++) {
		b->lower = 32000;
		b->upper = -32000;
	}
	return(0);
}

/* DR_SOLID
 *	Draw a line for surfaces
 */
int dr_solid(x0, y0, x1, y1)
	register int x0, y0, x1, y1;
{
	if (x0 != xlast || y0 != ylast) {
		penup();
		rmove2pt(x0, y0, 0);
		pendown();
	}
	rmove2pt(x1, y1, 0);
	xlast = x1;
	ylast = y1;
	return(0);
}

/* DR_HDDN
 *	Draw a (possibly) hidden line.
 */
#define INVIS	00
#define VIS	01

#define SWAP(a,b)	(hold = a, a = b, b = hold)

#define MV(u0,v0,u1,v1)	(penup(),		\
			rmove2pt((u0), (v0), 0),	\
			pendown(),		\
			rmove2pt((u1), (v1), 0))

#define VCHK(z)		(((z) <= bds[x].lower) || ((z) >= bds[x].upper))

int dr_hddn(x0, y0, x1, y1)
	int x0, y0, x1, y1;
{
	register int x, y, error;
	int dx, dy, inc;
	int hold, oldstat, newstat;
	int xs, ys, xl, yl;	/* start and last points */

	xlast = x1; ylast = y1;

	/*
	 * put x0, y0 in registers
	 * get resolution limited index into bds[]
	 */
	x = x0;
	y = y0;

	/*
	 * Single Point
	 */
	if (x == x1 && y == y1) {
		error = 0;
		if (y < bds[x].lower) {
			bds[x].lower = y;
			error = 1;
		}
		else if (y > bds[x].upper) {
			bds[x].upper = y;
			error = 1;
		}
		if (error) {
			penup();
			rmove2pt(x, y, 0);
			pendown();
			rmove2pt(x, y, 0);
		}
		return(0);
	}

	/*
	 * Vertical Line
	 */
	if (x == x1) {
		if (y1 < y) SWAP(y, y1);
		if (y >= bds[x].upper) {
vl1:
			MV(x, y, x, y1);
			bds[x].upper = y1;
			if(y <= bds[x].lower) bds[x].lower = y;
			return(0);
		}
		if (y <= bds[x].lower) {
			if (y1 < bds[x].lower) {
vl2:
				MV(x, y, x, y1);
				bds[x].lower = y;
				if (y1 > bds[x].upper) bds[x].upper = y1;
				return(0);
			}
			else if (y1 <= bds[x].upper) {
				y1 = bds[x].lower;
				goto vl2;
			}
			else {
				MV(x, bds[x].upper, x, y1);
				bds[x].upper = y1;
				y1 = bds[x].lower;
				goto vl2;
			}
		}
		else if (y1 > bds[x].upper) {
			y = bds[x].upper;
			goto vl1;
		}
		else return(0);
	}

	/*
	 * Horizontal Line
	 */
	if (y == y1) {
		if (x1 < x) SWAP(x, x1);
		oldstat = VCHK(y) ? VIS : INVIS;
		xs = xl = x;
		for (x++; x <= x1; xl = x++) {
			newstat = INVIS;
			if (y <= bds[x].lower) {
				newstat = VIS;
				bds[x].lower = y;
			}
			if (y >= bds[x].upper) {
				newstat = VIS;
				bds[x].upper = y;
			}

			if (newstat != oldstat) {
				if (oldstat) MV(xs, y, xl, y);
				xs = x;
				oldstat = newstat;
			}
		} /* end for */
		if (newstat) MV(xs, y, xl, y);
		return(0);
	}

	/*
	 * Line with abs(slope) <= 1
	 */
	if (abs(x-x1) >= abs(y-y1)) {
		if (x1 < x) { SWAP(x,x1); SWAP(y,y1); }
		dx = x1 - x;
		dy = y1 - y;
		if (dy < 0) {
			inc = -1;
			dy = -dy;
		}
		else inc = 1;
		error = dx / 2;
		xl = xs = x; yl = ys = y;
		oldstat = VCHK(y) ? VIS : INVIS;

		while(x <= x1) {
			xl = x++;
			error += dy;
			if (error >= dx) {
				error -= dx;
				yl = y;
				y += inc;
			}
			newstat = INVIS;
			if (y <= bds[x].lower) {
				bds[x].lower = y;
				newstat = VIS;
			}
			if (y >= bds[x].upper) {
				bds[x].upper = y;
				newstat = VIS;
			}
			if (newstat != oldstat) {
				if (oldstat) MV(xs, ys, xl, yl);
				xs = xl;
				xl = x;
				ys = yl;
				yl = y;
				oldstat = newstat;
			}
		} /* end while */
		if (newstat) MV(xs, ys, xl, yl);
		return(0);
	}

	/*
	 * Line with abs(slope) > 1
	 */
	if (y1 < y) { SWAP(x,x1); SWAP(y,y1); }
	dx = x1 - x;
	dy = y1 - y;
	if (dx < 0) {
		inc = -1;
		dx = -dx;
	}
	else inc = 1;
	error = dy / 2;
	xl = xs = x; ys = y;

	oldstat = VCHK(y) ? VIS : INVIS;

	while ( y <= y1 ) {
		yl = y++;
		error += dx;
		if (error >= dy) {
			error -= dy;
			xl = x;
			x += inc;
		}

		newstat = INVIS;
		if (y <= bds[x].lower) {
			bds[x].lower = y;
			newstat = VIS;
		}
		if (y >= bds[x].upper) {
			bds[x].upper = y;
			newstat = VIS;
		}

		if (newstat != oldstat) {
			if (oldstat) MV(xs, ys, xl, yl);
			xs = xl;
			xl = x;
			ys = yl;
			yl = y;
			oldstat = newstat;
		}
	} /* end while */
	if (newstat) MV(xs, ys, xl, yl);
	return(0);
}


#ifdef ROT

/* BLD_ROT
 *	Build the 3-dimensional rotation matrix.  All rotations are about
 * the origin.
 *	These rotations are the same as the TIG rotations.
 * INPUT:
 *	R = pointer to 3x3 matrix
 *	alpha, beta, theta = TIG angles in degrees
 */
bld_rot(R, alpha, beta, theta)
	MATRIX R[3][3];
	float alpha, beta, theta;
{
	register double del;
	double acos, bcos, tcos;	/* cosines of Alpha, beta, theta */
	double asin, bsin, tsin;	/* sines of Alpha, beta, theta */

	acos = cos( alpha * deg2rad );
	bcos = cos( beta * deg2rad );
	tcos = cos( theta * deg2rad );
	asin = sin( alpha * deg2rad );
	bsin = sin( beta * deg2rad );
	tsin = sin( theta * deg2rad );

	if( (del=acos*acos + asin*asin*bcos*bcos) == 0 ) {
		fprintf(stderr,"ALPHA-BETA angles such that inverse matrix is impossible\n");
		return;
	}

	R[0][0] =  (acos*tcos - asin*acos*bsin*tsin)/del;
	R[0][1] = -(acos*tsin + tcos*bsin*acos*asin)/del;
	R[0][2] = - bcos*asin/del;
	R[1][0] =  (bcos*tsin - bcos*asin*bsin*tcos)/del;
	R[1][1] =  (tcos*bcos + bsin*tsin*asin*bcos)/del;
	R[1][2] = - acos*bsin/del;
	R[2][0] =  (acos*acos*bsin*tsin + bcos*bcos*asin*tcos)/del;
	R[2][1] =  (acos*acos*tcos*bsin - bcos*bcos*tsin*asin)/del;
	R[2][2] =   acos*bcos/del;
}

/* SET_SCALE
 *	Sets up scale factors in the transform structure to convert
 * from transformed projection coordinates to TIG units
 * INPUT:
 *	R = rotation matrix
 *	xf = transform structure with xlo, xhi, ylo, yhi, zlo, zhi filled in
 */
set_scale(R, xf, hlen, vlen)
	MATRIX *R;
	XFORM *xf;
	int hlen, vlen;
{
	register int i, j, k;
	float *x, *y, *z;
	float hlo, hhi, vlo, vhi;
	VECTOR p[3], q[3];

	/*
	 * start with identity transform
	 */
	xf->hscl = xf->vscl = 1.0;
	xf->hoff = xf->voff = 0.0;

	x = &xf->xlo;
	y = &xf->ylo;
	z = &xf->zlo;

	hlo = vlo = 1e10; hhi = vhi = -1e10;

	/*
	 * generate the transform of the 8 vertices of the enclosing rectangle
	 */
	for (i = 0; i < 2; i++) {
		p[0] = x[i];
		for (j = 0; j < 2; j++) {
			p[1] = y[j];
			for (k = 0; k < 2; k++) {
				p[2] = z[k];
				matcopy(p,q,3,1);
				ptxf(q, R, xf);

				if (q[0] < hlo) hlo = q[0];
				if (q[0] > hhi) hhi = q[0];
				if (q[1] < vlo) vlo = q[1];
				if (q[1] > vhi) vhi = q[1];
			}
		}
	}

	xf->hscl = (double) hlen / (hhi - hlo);
	xf->hoff = hlo;

	xf->vscl = (double) vlen / (vhi - vlo);
	xf->voff = vlo;
}

/* PTXF
 *	Point tranformation.  Apply translation, rotation, perspective
 * transformations to a given point.
 * RETURN:  -1 if perspective height is too low
 */
ptxf(p, R, xf)
	register VECTOR *p;
	register MATRIX *R;
	register XFORM *xf;
{
	VECTOR pt[3];
	double fac;

	/*
	 * translate
	 */
	pt[0] = p[0] - xf->xtrans;
	pt[1] = p[1] - xf->ytrans;
	pt[2] = p[2] - xf->ztrans;

	vecmpy(R,pt,p,3,3);	/* rotate */

	/*
	 * perspective
	fac = 1 - (p[2] / xf->zht);
	if (fac == 0) return(-1);
	p[0] /= fac;
	p[1] /= fac;
	 */

	/*
	 * convert to TIG units
	 */
	p[0] = (p[0] - xf->hoff) * xf->hscl;
	p[1] = (p[1] - xf->voff) * xf->vscl;

	return(0);
}
#endif
