/* GDFRAME -- put a frame around gould */
#include <stdio.h>
#include "tig.h"

gdframe()
{
	penup();
	rmove2pt(0,0,0);
	pendown();
	rmove2pt(10549,0,0);
	rmove2pt(10549,8249,0);
	rmove2pt(0,8249,0);
	rmove2pt(0,0,0);
	penup();
}
