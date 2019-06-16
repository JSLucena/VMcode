#pragma once
#ifndef _MICROINS_H
#define _MICROINS_H
#include <iostream>
#include <string>

//Feito por Joaquim Lucena e Christian Bobsin


class Uins
{
protected:
	 int r1;
	 int r2_Imm;
	std::string inst;
public:
	void setR1(unsigned int reg);
	 int getR1();
	void setR2(unsigned int reg);
	 int getR2();
	void setINST(std::string instrucao);
	std::string getINST();

};
#endif /*_MICROINS_H*/