#include "Pagina.h"

Pagina::Pagina( int i, int f)
{

	id = i;
	idFrame = f;
}


int Pagina::getID()
{
	return id;
}

int Pagina::getFrameID()
{
	return idFrame;
}


void Pagina::setID(int x)
{
	id = x;
}

void Pagina::setFrameID(int x)
{
	idFrame = x;
}
