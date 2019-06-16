#pragma once
#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include "MICROINS.h"
#include "T2.h"


//Feito por Joaquim Lucena e Christian Bobsin

unsigned int GetR1(unsigned int instReg, std::string inst);

unsigned int getR2_IMM(unsigned int instReg, std::string inst);

void limpamem(unsigned int* memoria, unsigned int tamanho);

unsigned int IFetch(unsigned int PC, unsigned int* mem);

std::string decode(unsigned int instR);

void MemOps(int& reg, std::string instrucao, unsigned int address, unsigned int* memoria);

bool Branch(int rs, int rc, std::string instrucao);

void ULA(int& op1, unsigned int op2, std::string instrucao);

void dumpmem(unsigned int* mem);

#endif /*_FUNCION_H*/
