#include "MICROINS.h"
#ifndef _MICROINS_CPP
#define _MICROINS_CPP
void Uins::setR1(unsigned int reg)
{
	r1 = reg;
}
 int Uins::getR1()
{
	return r1;
}
void Uins::setR2(unsigned int reg)
{
	r2_Imm = reg;
}
 int Uins::getR2()
{
	return r2_Imm;
}
void Uins::setINST(std::string instrucao)
{
	inst = instrucao;
}
std::string Uins::getINST()
{
	return inst;
}
#endif /*_MICROINS_CPP*/