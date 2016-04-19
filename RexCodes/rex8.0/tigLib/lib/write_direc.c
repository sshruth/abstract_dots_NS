#include <stdio.h>
#include "tig.h"

static struct TIGvec vector;

/*
 * WRITE_DIREC
 *	Sets up TIG vectors for writing and axis drawing.
 * INPUT:
 *	char c = 'x', 'y', or 'z' for direction to write and draw axis
 *	struct TIGdvec *direcp = pointer to TIG vector of doubles
 *		to be used by call to axis()
 *
 * History:
 *	6-30-86	LMO
 */
write_direc(c, direcp)
	register char c;
	register struct TIGdvec *direcp;
{
	vector.Xpos = vector.Ypos = vector.Zpos = 0;

	switch (c) {
	default:
	case 'x':
	case 'X':
		vector.Xpos = 1;
		break;
	case 'y':
	case 'Y':
		vector.Ypos = 1;
		break;
	case 'z':
	case 'Z':
		vector.Zpos = 1;
		break;
	}
	vcpyid(&vector, direcp);

	newplane(&vector, 0);
}

vcpyid(a, b)
	register struct TIGvec *a;
	register struct TIGdvec *b;
{
	b->XXpos = a->Xpos;
	b->YYpos = a->Ypos;
	b->ZZpos = a->Zpos;
}
