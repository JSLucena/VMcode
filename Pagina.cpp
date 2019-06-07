#include "Pagina.h"

Pagina::Pagina(int b, int l, int i, int f)
{
	base = b;
	limite = l;
	id = i;
	idFrame = f;
}

int Pagina::getBase()
{
	return base;
}

int Pagina::getLimite()
{
	return limite;
}

int Pagina::getID()
{
	return id;
}

int Pagina::getFrameID()
{
	return idFrame;
}

void Pagina::setBase(int x)
{
	base = x;
}

void Pagina::setLimite(int x)
{
	limite = x;
}

void Pagina::setID(int x)
{
	id = x;
}

void Pagina::setFrameID(int x)
{
	idFrame = x;
}
