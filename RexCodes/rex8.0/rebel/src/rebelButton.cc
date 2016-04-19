#include <Pt.h>
#include <Ap.h>
#include "rebel.h"

rebelButton::rebelButton()
{

}

rebelButton::rebelButton(const char *name)
{

	strcpy(_name, name);

}

rebelButton::rebelButton(const char *name, PhPoint_t pos)
{

	strcpy(_name, name);

	_origin = pos;

}

rebelButton::~rebelButton()
{
	PtDestroyWidget(_button);
}

bool rebelButton::operator==(const rebelButton& rs)
{
	return((_origin.x == rs._origin.x) && (_origin.y == rs._origin.y));
}

bool rebelButton::operator<(const rebelButton& rs)
{
	return((_origin.y < rs._origin.y) ||((_origin.y == rs._origin.y) && (_origin.x < rs._origin.x)));
}

bool rebelButton::operator>(const rebelButton& rs)
{
	return((_origin.y > rs._origin.y) ||((_origin.y == rs._origin.y) && (_origin.x > rs._origin.x)));
}

char *rebelButton::name()
{
	return(_name);
}
void rebelButton::origin(PhPoint_t o)
{
	_origin = o;
	PtSetResource(_button, Pt_ARG_POS, &_origin, 0);
}
