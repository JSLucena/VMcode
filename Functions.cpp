#include "Functions.h"


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

void limpamem(unsigned int* memoria, unsigned int tamanho)
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

	instR = instR >> 27;

	switch (instR)
	{
	case 0: return "JMP";
	case 1: return "JMPI";
	case 2: return "JMPIG";
	case 3: return "JMPIL";
	case 4: return "JMPIE";

	case 8: return "ADDI";
	case 9: return "SUBI";
	case 10: return "ANDI";
	case 11: return "ORI";
	case 12: return "LDI";
	case 13: return "LDD";
	case 14: return "STD";

	case 16: return "ADD";
	case 17: return "SUB";
	case 18: return "AND";
	case 19: return "OR";
	case 20: return "LDX";
	case 21: return "STX";

	case 24: return "NOT";
	case 25: return "SHL";
	case 26: return "SHR";
	case 27: return "SWAP";
	case 28: return "MULT";

	case 31: return "STOP";

	default: return "INVALID_INSTRUCTION";
	}

}

void MemOps(int& reg, std::string instrucao, unsigned int address, unsigned int* memoria)
{
	if (instrucao == "LDD" || instrucao == "LDX")
		reg = memoria[address];
	else if (instrucao == "STD" || instrucao == "STX")
		memoria[address] = reg;
	else return;
}
bool Branch(int rs, int rc, std::string instrucao)
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
void ULA(int& op1, unsigned int op2, std::string instrucao)
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
		op1 = op1 >> 1;
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
