#pragma once
#ifndef _FILAIO_H
#define _FILAIO_H
class pedidoConsole
{
protected:
	int parametro;
	int tipo;
	int idProcesso;
	int base;
public:
	pedidoConsole()
	{
		parametro = 0;
		tipo = 0;
		idProcesso = 0;
		base = 0;
	}
	pedidoConsole(int N, int T, int id, int l)
	{
		parametro = N;
		tipo = T;
		idProcesso = id;
		base = l;
	}
	void setParametro(int N)
	{
		parametro = N;
	}
	int getParametro()
	{
		return parametro;
	}
	void setTipo(int N)
	{
		tipo = N;
	}
	int getTipo()
	{
		return tipo;
	}
	void setIdProcesso(int N)
	{
		idProcesso = N;
	}
	int getIDProcesso()
	{
		return idProcesso;
	}
	void setBase(int N)
	{
		base = N;
	}
	int getBase()
	{
		return base;
	}
};
#endif