#pragma once
#ifndef _GPROCESS_H
#define _GPROCESS_H
#include <list>
#include "Pagina.h"
#include <algorithm>
#include <fstream>
#define PART_SIZE 128
#define PAG_SIZE 16


//Feito por Joaquim Lucena e Christian Bobsin


enum state {READY,RUNNING,BLOCKED};
class ProcessControlBlock
{
	protected:
		state estado;
		int id = 0;
		int base;
		int limite;
		int regBank[8] = { 0,0,0,0,0,0,0,0 };
		int pc = 0;
		

	public:
		std::list<Pagina> pageTable;
		ProcessControlBlock()
		{
			base = 0;
			limite = 0;
			estado = READY;
		}
		ProcessControlBlock(int bas, int limit, state s, int n)
		{
			base = bas;
			limite = limit;
			estado = s;
			pc = bas;
			id = n;
		}
	void setEstado(state s)
	{
		estado = s;
	}
	void setID(int i)
	{
		id = i;
	}
	void setBase(int b)
	{
		base = b;
	}
	void setLimite(int l)
	{
		limite = l;
	}
	void setPC(int x)
	{
		pc = x;
	}
	void setReg(int i, int val)
	{
		regBank[i] = val;
	}
	state getEstado()
	{
		return estado;
	}
	int getID()
	{
		return id;
	}
	int getBase()
	{
		return base;
	}
	int getLimite()
	{
		return limite;
	}
	int getPC()
	{
		return pc;
	}
	int getReg(int i)
	{
		return regBank[i];
	}

};
#endif

