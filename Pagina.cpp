#include "Pagina.h"

//Feito por Joaquim Lucena e Christian Bobsin

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
