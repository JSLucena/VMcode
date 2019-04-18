#pragma once
#ifndef _FUNCT_H
#define _FUNCT_H
#include"T2.h"
#include<iostream>
#include<string>
#include<chrono>
unsigned int pc = 0;
unsigned int acc = 0, instReg = 0;
int regBank[8] = { 0,0,0,0,0,0,0,0 };
unsigned int i = 0;
unsigned int tamMemoria = 1024;
bool processEnd = false;
int interrupcaoTIMER = 0;
unsigned int* memoriastart, * ptr, * memoriaend;
int posicao;
std::string  instrucao, programa;
unsigned int arqbuffer = 0;
Uins UINS;
std::ifstream arq;
char escolha = 'x';
int id;
std::list<ProcessControlBlock> ready;
ProcessControlBlock running;
bool particoes[8] = { false, false, false, false, false, false, false, false };
bool desliga = false;



ProcessControlBlock criaPCB(int particao)
{
	ProcessControlBlock pcb = ProcessControlBlock(PART_SIZE * particao, PART_SIZE * (particao + 1) - 1, READY, id);
	return pcb;
}

void alocaParticao(int it)
{
	particoes[it] = true;
	id++;
}
void carrega(int it)
{
	ptr = &memoriastart[PART_SIZE * it];
	while (arq)
	{

		arq >> arqbuffer;

		if (!arq)
			break;
		*ptr = arqbuffer;

		ptr++;

	}
	arq.close();
	memoriaend = ptr--;

	ptr = memoriastart;
}
void criacaoProcesso(int particao)
{
	alocaParticao(particao);
	carrega(particao);
	ready.push_back(criaPCB(particao));
	std::cout << ready.size();
	std::cout << ready.front().getBase() << " " << ready.front().getLimite() << std::endl;
	//rotinaTratamentoTimer();
    if(running.getLimite()== 0)
    {
        running = ready.front();
		std::cout << running.getBase() << " " << running.getLimite() << std::endl;
        pc = running.getPC();
		//std::cout <<"1";
        ready.pop_front();
    }
}
int criaProcesso()
{
	int it;
    if (programa.find(".txt") != std::string::npos)
        ;
    else programa = programa + ".txt";


    //programa =  programa;


	std::cout << "Abrindo programa " << programa << std::endl << std::endl;
	arq.open(programa, std::ios::in);

	if (!arq)
	{
		std::cout << "Erro ao abrir o arquivo" << std::endl;
		return -2;
	}
	for (it = 0; it < 8; it++)
	{
		if (particoes[it] == false)
		{
			criacaoProcesso(it);
			return 1;
		}
	}
	std::cout << "Nao ha memoria suficiente" << std::endl;
	return -1;


}

void salvaContexto(ProcessControlBlock  pcb)
{
	int ilocal = 0;
	for (ilocal = 0; ilocal < 8; ilocal++)
		pcb.setReg(ilocal, regBank[ilocal]);
	pcb.setPC(pc);
}
void restauraContexto(ProcessControlBlock pcb)
{
	int ilocal = 0;
	for (ilocal = 0; ilocal < 8; ilocal++)
		regBank[ilocal]= pcb.getReg(ilocal);
	pc = pcb.getPC();
}
void rotinaTratamentoTimer()
{
	salvaContexto(running);
	running.setEstado(READY);

	if (processEnd == true)
	{
		processEnd = false;
		if (ready.size() != 0)
		{
			running = ready.front();
			ready.pop_front();
			restauraContexto(running);
		}
		else desliga = true;
	}
	if (ready.size() != 0)
	{
		running = ready.front();
		restauraContexto(running);
		ready.pop_front();
		interrupcaoTIMER = 0;
	}
	else desliga = true;
	return;
}
#endif
void CPU()
{
	while (1)
    {

    if(interrupcaoTIMER == 1)
        rotinaTratamentoTimer();
	if (desliga == true)
		break;
	if(running.getLimite() != 0)
        {
            instReg = IFetch(pc, memoriastart);
            UINS.setINST(decode(instReg));
            UINS.setR1(GetR1(instReg, UINS.getINST()));
            UINS.setR2(getR2_IMM(instReg, UINS.getINST()));
            std::cout <<"Processo:"<< running.getID()<< " "<< UINS.getINST() << " " << UINS.getR1() << " " << UINS.getR2() << std::endl;


            if (UINS.getINST() == "LDD" || UINS.getINST() == "STD")
            {
                //if(UINS.getR2() < running->getBase() || UINS.getR2() > running->getLimite())
              //  {
              //      std::cout << "fudeu, o processo vai fechar";
               //     processEnd = true;
              //      rotinaTratamentoTimer();
              //  }

                if (UINS.getINST() == "STX")
                    MemOps(regBank[UINS.getR2()], UINS.getINST(), running.getBase()+UINS.getR1(), memoriastart);
                else
                    MemOps(regBank[UINS.getR1()], UINS.getINST(), running.getBase()+UINS.getR2(), memoriastart);
            }
            else
            {


                if (UINS.getINST() == "STX")
                    MemOps(regBank[UINS.getR2()], UINS.getINST(), running.getBase()+regBank[UINS.getR1()], memoriastart);
                else
                    MemOps(regBank[UINS.getR1()], UINS.getINST(), running.getBase()+regBank[UINS.getR2()], memoriastart);
            }
            if (UINS.getINST() == "LDI" || UINS.getINST() == "ADDI" || UINS.getINST() == "SUBI" || UINS.getINST() == "ANDI" || UINS.getINST() == "ORI")
                ULA(regBank[UINS.getR1()], UINS.getR2(), UINS.getINST());
            else
                ULA(regBank[UINS.getR1()], regBank[UINS.getR2()], UINS.getINST());

            if (UINS.getINST() == "STOP")
            {
                interrupcaoTIMER = 1;
                processEnd = true;

                //break;
            }
            if (Branch(regBank[UINS.getR1()], regBank[UINS.getR2()], UINS.getINST()))
                if (UINS.getINST() == "JMP")
                    pc = UINS.getR2()+running.getBase();
                else
                    pc = regBank[UINS.getR1()]+running.getBase();
            else
                pc++;


           // if (flagTIMER == true)
             //   flagTIMER = false;

        }
    }
}
void timer()
{
	//duration;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;
	double tempo = 0.0;
	while (1)
	{
		if (interrupcaoTIMER == 0)
		{
			if (tempo >= 1.5)
			{
				std::cout << "CPU timed out" << std::endl;
				interrupcaoTIMER = 1;
				tempo = 0.0;


			}
			else
			{
				start = std::chrono::system_clock::now();

				end = std::chrono::system_clock::now();
				elapsed_seconds = end - start;
				tempo += elapsed_seconds.count();

			}
		}
		if (desliga == true)
			break;
	}
}
