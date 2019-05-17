#pragma once
#ifndef _FILAIO_H
#define _FILAIO_H
class pedidoConsole
{
protected:
	int parametro;
	int tipo;
public:
	pedidoConsole(int N, int T)
	{
		parametro = N;
		tipo = T;
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
};
#endif