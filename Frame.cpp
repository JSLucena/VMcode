#include "Frame.h"

//Feito por Joaquim Lucena e Christian Bobsin


Frame::Frame(int b, int l, int i)
{
	base = b;
	limite = l;
	id = i;
}
int Frame::getBase()
{
	return base;
}
int Frame::getID()
{
	return id;
}
int Frame::getLimite()
{
	return limite;
}
void Frame::setID(int x)
{
	id = x;
}
void Frame::setBase(int x)
{
	base = x;
}
void Frame::setLimite(int x)
{
	limite = x;
}