#pragma once
#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <iostream>
#include <string>
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


unsigned int GetR1(unsigned int instReg, std::string inst)
{
	unsigned int aux;

	aux = instReg << 5;
	aux = aux >> 29;
	return aux;
}
unsigned int getR2_IMM(unsigned int instReg, std::string inst)
{
	unsigned int aux;

	if (inst == "JMP" || inst == "ADDI" || inst == "SUBI" || inst == "ANDI" || inst == "ORI" || inst == "LDI" || inst == "LDD" || inst == "STD")
	{
		aux = instReg << 16;
		aux = aux >> 16;
	}
	else
	{
		aux = instReg << 8;
		aux = aux >> 29;
	}
	return aux;

}

void limpamem(unsigned int *memoria, unsigned int tamanho)
{
	int i;
	for (i = 0; i < tamanho; i++)
		memoria[i] = 0;

	return;
}

unsigned int IFetch(unsigned int PC, unsigned int* mem)
{
	return mem[PC];
}
std::string decode(unsigned int instR)
{
	//  char inst[10];
	instR = instR >> 27;

	switch (instR)
	{
	case 0: return "JMP";//strcpy(inst,"JMP"); break;
	case 1: return "JMPI";//strcpy(inst,"JMPI"); break;
	case 2: return "JMPIG";//strcpy(inst,"JMPIG"); break;
	case 3: return "JMPIL";//strcpy(inst,"JMPIL"); break;
	case 4: return "JMPIE";//strcpy(inst,"JMPIE"); break;

	case 8: return "ADDI";//strcpy(inst,"ADDI"); break;
	case 9: return "SUBI";//strcpy(inst,"SUBI"); break;
	case 10: return "ANDI";//strcpy(inst,"ANDI");break;
	case 11: return "ORI";//strcpy(inst,"ORI"); break;
	case 12: return "LDI";//strcpy(inst,"LDI"); break;
	case 13: return "LDD";//strcpy(inst,"LDD"); break;
	case 14: return "STD";//strcpy(inst,"STD"); break;

	case 16: return "ADD";//strcpy(inst,"ADD"); break;
	case 17: return "SUB";//strcpy(inst,"SUB"); break;
	case 18: return "AND";//strcpy(inst,"AND"); break;
	case 19: return "OR";//strcpy(inst,"OR"); break;
	case 20: return "LDX";//strcpy(inst,"LDX"); break;
	case 21: return "STX";//strcpy(inst,"STX"); break;

	case 24: return "NOT";//strcpy(inst,"NOT"); break;
	case 25: return "SHL";//strcpy(inst, "SHL"); break;
	case 26: return "SHR";//strcpy(inst, "SHR"); break;
	case 27: return "SWAP";//strcpy(inst, "SWAP"); break;
	case 28: return "MULT";//strcpy(inst,"MULT"); break;

	case 31: return "STOP";//strcpy(inst, "STOP"); break;

	default: return "INVALID_INSTRUCTION";
	}
	//   return inst;
}

void MemOps( int& reg, std::string instrucao, unsigned int address, unsigned int* memoria)
{
	if (instrucao == "LDD" || instrucao == "LDX")
		reg = memoria[address];
	else if (instrucao == "STD" || instrucao == "STX")
		memoria[address] = reg;
	else return;
}
bool Branch( int rs, int rc, std::string instrucao)
{
	if (instrucao == "JMP" || instrucao == "JMPI")
		return true;
	else if (instrucao == "JMPIG" && rc > 0)
		return true;
	else if (instrucao == "JMPIL" && rc < 0)
		return true;
	else if (instrucao == "JMPIE" && rc == 0)
		return true;
	else return false;
}
void ULA( int &op1, unsigned int op2, std::string instrucao)
{
	if (instrucao == "ADD" || instrucao == "ADDI")
		op1 = op1 + op2;
	else if (instrucao == "SUBI" || instrucao == "SUB")
		op1 = op1 - op2;
	else if (instrucao == "AND" || instrucao == "ANDI")
		op1 = op1 & op2;
	else if (instrucao == "OR" || instrucao == "ORI")
		op1 = op1 | op2;
	else if (instrucao == "MULT")
		op1 = op1 * op2;
	else if (instrucao == "NOT")
		op1 = ~op1;
	else if (instrucao == "SHL")
		op1 = op1 << 1;
	else if (instrucao == "SHR")
		op1 = op1 >> 2;
	else if (instrucao == "SWAP")
		std::cout << "swap ainda nao funciona!" << std::endl;
	else if (instrucao == "LDI")
		op1 = op2;
	else
		return;
	return;
}
void dumpmem(unsigned int* mem)
{
	int i;
	int viewcounter = 0;
	for (i = 0; i <= 1023; i++)
	{
		printf("%d ", mem[i]);
		viewcounter++;
		if (viewcounter == 8)
		{
			std::cout << std::endl;
			viewcounter = 0;
		}
	}

}

#endif /*_FUNCION_H*/