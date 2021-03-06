#pragma once
#ifndef _FUNCT_H
#define _FUNCT_H
#include"T2.h"
#include<iostream>
#include<queue>
#include<string>
#include<chrono>
#include <thread>
#include <mutex>
#include <sstream>
#include "MICROINS.h"
#include "Functions.h"
#include "FilaIO.h"


//Feito por Joaquim Lucena e Christian Bobsin


unsigned int pc = 0;
unsigned int acc = 0, instReg = 0;
int regBank[8] = { 0,0,0,0,0,0,0,0 };
unsigned int i = 0;
unsigned int tamMemoria = 1024;

int interrupcao = 0;
unsigned int* memoriastart, *ptr;
int posicao;
std::string  instrucao, programa;

unsigned int arqbuffer = 0;
Uins UINS;
std::ifstream arq;

int id, input; //id do processo / input para programas que necessitam
std::list<ProcessControlBlock> ready; //fila de processos prontos para serem escalonados
ProcessControlBlock running; //pcb do processo que esta em execucao
bool particoes[8] = { false, false, false, false, false, false, false, false }; //vetor de particoes alocadas, falso significa que nao estao alocadas


bool desliga = false; //variavel de controle, que termina com as threads depois que a execucao de todos os processos acaba
bool startExec = false; //variavel de controle que ordena o inicio da execucao apos o comando [e]
bool processEnd = false; //variavel que indica o fim de um processo, para nao adiciona-lo novamente na fila de processos 

//############################################Trabalho 3####################################
std::string escolha; //variavel de controle para os comandos do shell
std::list<ProcessControlBlock> blocked; //lista de processos bloqueados
std::queue<pedidoConsole> filaPedidos; // lista de request para o console de IO
std::mutex mut; //mutex, para impedir que aconte�am sobreescritas no valor interrupcao"
int parametro; // Read ou Write
bool wait = false; //variavel de controle, para caso nao exista nenhum processo apto a rodar tranca a CPU
pedidoConsole pedidoPronto; //pedido atendido pelo console
std::string consoleVal; //buffer de input do console

void rotinaTratamentoIO(); //funcao que trata IO
void console(); //thread da console
void syscall(); // funcao que trata o TRAP


int baseRequest = 0; // variavel para ser colocada no pedido a ser enviado � fila
int idRequest = 0; // variavel para ser colocada no pedido a ser enviado � fila
int delayCatcher = 0; //para impedir que aconte�am sobreescritas no valor interrupcao, faz com que a interrupcao de termino de tratamento seja tratada ap�s adicionarmos um request pendente � fila
bool delay = false; //para impedir que aconte�am sobreescritas no valor interrupcao, faz com que a interrupcao de termino de tratamento seja tratada ap�s adicionarmos um request pendente � fila

std::mutex prontos, intt;



//#########################################################################################




//####################################TRABALHO 4###########################################

bool frames[64]; //vetor de frames alocados, falso significa que nao estao alocados
int logicToPhysic(int logAdd); //funcao de decodificacao de endereco logico para fisico
bool killProc = false, noProc = false; //killproc eh uma variavel que indica que o processo precisa ser morto, noproc significa que o processo precisa ser morto, mas nao ha nenhum processo para substitui-lo na fila(((necessario por causa de escolhas de design)))
void kill(); //chamada que mata o processo

//#################### Gerente De ################################
//#################### Memoria ###################################
ProcessControlBlock criaPCB(int particao) //cria nosso objeto PCB, com base, limite, estado inicial e ID
{
	ProcessControlBlock pcb = ProcessControlBlock(PART_SIZE * particao, PART_SIZE * (particao + 1) - 1, READY, id);
	return pcb;
}


int alocaFrame() //funcao que aloca a particao, colocando seu valor em true e incrementando o ID
{
	for (int i = 0; i < 64; i++) //procura o primeiro frame desalocado
		if (frames[i] == false)
		{
			frames[i] = true;
			return i; // e retorna ele para o processo que pediu
		}

}
void carrega(int it) //carrega o arquivo na particao de memoria do processo
{
	ptr = &memoriastart[PAG_SIZE * it];
	while (arq)
	{
		if (ptr == &memoriastart[PAG_SIZE * (it + 1)])
			break;
		arq >> arqbuffer;

		if (!arq)
			break;
		*ptr = arqbuffer;

		ptr++;

	}



}
void criacaoProcesso(int pagina, ProcessControlBlock &pcb) //funcao de criacao do processo
{
	int frameid;
	frameid = alocaFrame(); //aloca no frame
	std::cout << "pagina: " << pagina << "# Frame: " << frameid << std::endl;
	pcb.pageTable.push_back(Pagina(pagina, frameid)); //jogamos na tabela de paginas a pagina com seu id e com o id do frame relacionado a ela


	carrega(frameid); //carrega no frame

}
int criaProcesso() //funcao que chama a criacao do processo
{
	int it;
	int size;
	int disponiveis = 0, pagecount = 0;
	std::list<Pagina>::iterator itt;
	ProcessControlBlock aux;
	if (programa.find(".txt") != std::string::npos)
		;
	else programa = programa + ".txt";





	std::cout << "Abrindo programa " << programa << std::endl << std::endl;
	arq.open(programa, std::ios::in);

	if (!arq)
	{
		std::cout << "Erro ao abrir o arquivo" << std::endl;
		return -2;
	}

	for (int i = 0; i < 64; i++)
		if (frames[i] == false)
			disponiveis++; //calculamos quantos frames temos disponiveis

	arq >> size; //a primeira linha do codigo eh a quantidade de frames necessarios para alocacao

	if (size < disponiveis) //se o programa couber no espaco disponivel
	{
		aux = ProcessControlBlock(); //criamos nosso processo
		for (it = 0; it < size; it++)
		{

			criacaoProcesso(it, aux); //carregamos e adicionamos cada pagina em separado, com id dela == it


		}

		aux.setLimite(PAG_SIZE * aux.pageTable.size() - 1); //depois setamos o limite do nosso processo
		aux.setBase(PAG_SIZE * aux.pageTable.front().getFrameID()); //a base
		aux.setPC(aux.getBase()); //o pc, para comecarmos no lugar certo
		ptr = memoriastart; //botamos nosso ponteiro de volta ao inicio da memoria, se quisermos adicionar outro processo
		prontos.lock();
		aux.setID(id); //setamos o id do processo
		id++;
		ready.push_back(aux); //joga o processo novo na fila deprontos


		if (running.getLimite() == 0) //se nao existir nenhum running(limite = 0)
		{
			running = ready.front(); //ele pega o primeiro da lista
			pc = running.getPC(); //ajusta o PC para la

			ready.pop_front(); //e corta da lista
		}
		prontos.unlock();
		std::cout << size << " Paginas alocadas na seguinte configuracao: " << std::endl;
		arq.close();
		return 1;
	}
	std::cout << "Nao ha memoria suficiente" << std::endl;
	return -1;


}
//#######################################################
//#######################################################


//################### Gerente de ########################
//################### Processos #########################
void salvaContexto(ProcessControlBlock  &pcb) //funcao que salva PC e regs no PCB
{
	int ilocal = 0;
	for (ilocal = 0; ilocal < 8; ilocal++)
		pcb.setReg(ilocal, regBank[ilocal]);

	pcb.setPC(pc);

}
void restauraContexto(ProcessControlBlock &pcb) //funcao que restaura os regs e o PC do PCB
{
	int ilocal = 0;
	for (ilocal = 0; ilocal < 8; ilocal++)
		regBank[ilocal] = pcb.getReg(ilocal);
	pc = pcb.getPC();

	pcb.setEstado(RUNNING);
}




void rotinaTratamentoTimer() //rotina de tratamento de interrupcao por timeout
{
	std::list<Pagina>::iterator itt;
	if (startExec == true && ready.size() != 0)
		std::cout << "CPU timed out " << std::endl;
	salvaContexto(running);



	if (processEnd == true || noProc == true) //se um processo terminar
	{
		processEnd = false;
		if (ready.size() != 0) //ele testa se a fila nao esta vazia
		{
			//se nao estiver ele desaloca o processo que acabou e escalona o proximo da fila
			for (itt = running.pageTable.begin(); itt != running.pageTable.end(); itt++) //desaloca todas as frames do processo
			{
				frames[itt->getFrameID()] = false;
			}
			
			running = ready.front();
			ready.pop_front();
			restauraContexto(running);
			desliga = false;
			wait = false;
			interrupcao = 0; //depois ele tira a flag de interrupcao

			return;
		}
		else //se estiver ele desaloca e bloqueia a execucao da cpu
		{
			for (itt = running.pageTable.begin(); itt != running.pageTable.end(); itt++) //desaloca todas as frames do processo
			{
				frames[itt->getFrameID()] = false; 
			}
			running.setLimite(0);
			wait = true;
			running.setEstado(BLOCKED);
			interrupcao = 0; //depois ele tira a flag de interrupcao

			return;
		}
	}
	else //se o time slice do processo acabou
	{
		prontos.lock();
		if (running.getEstado() != BLOCKED)
		{
			running.setEstado(READY);
			ready.push_back(running); //ele joga de volta na fila
		}
		prontos.unlock();
		if (ready.size() != 0)
		{
			//e escalona o proximo se a fila nao estiver vazia
			running = ready.front();
			restauraContexto(running);
			wait = false;
			ready.pop_front();
			interrupcao = 0; //depois ele tira a flag de interrupcao
			desliga = false;

			return;
		}
		else
		{
			if (running.getEstado() != BLOCKED)
			{
				interrupcao = 0; //depois ele tira a flag de interrupcao
				wait = false;

				return;
			}
		}
		interrupcao = 0; //depois ele tira a flag de interrupcao


		return;
	}
}

void kill()
{
	if (ready.size() != 0)
	{
		std::cout << "Processo " << running.getID() << " foi morto" << std::endl;
		//pegamos o proximo processo da lista e botamos ele como running sem salvar o contexto do processo antigo
		running = ready.front();
		restauraContexto(running);
		ready.pop_front();
	}
	else
	{
		//esse snippet esta aqui por causa de uma escolha de design feita la no inicio. Como nao usamos ponteiros, nao temos como deixar o running em um estado nulo. entao trancamos a execucao do processo se ele for o unico.
		std::cout << "Processo invalido, suas operacoes nao terao efeito na execucao" << std::endl;
		noProc = true;
	}
	killProc = false;
}

//#######################################################
//#######################################################



//##################### CPU #############################
//#######################################################
void timer()
{

	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;
	double tempo = 0.0;
	while (1)
	{

		if (interrupcao == 0)
		{
			if (tempo >= 0.001) //depois que o tempo passar de 1ms ele liga o flag de interrupcao
			{


				intt.lock();
				if (interrupcao == 0)
				{

					interrupcao = 3;
					tempo = 0.0;


				}
				intt.unlock();

			}
			else
			{
				start = std::chrono::system_clock::now(); //nosso timer subtrai o horario da batida final do clock com a batida de inicio do clock
				end = std::chrono::system_clock::now();
				elapsed_seconds = end - start;
				tempo += elapsed_seconds.count(); //e vai contando este tempo ate que de 1ms

			}
		}

		if (desliga == true)
			break;

	}
}




void CPU()
{
	int address;
	int addressSTX;
	while (1)
	{

		if (startExec == true) //a CPU so ira executar se esta flag for ativada com o comando [e]
		{

			if (interrupcao == 3)
				rotinaTratamentoTimer(); //executa a rotina de tratamento de interrupcao se a flag for ativada
			else if (interrupcao == 1 || interrupcao == 2 || interrupcao == 4 || interrupcao == 5)
				rotinaTratamentoIO();

			else
			{

				if (wait == false && running.getEstado() == RUNNING && noProc == false)
				{



					instReg = IFetch(pc, memoriastart); //busca de instrucoes
					UINS.setINST(decode(instReg)); //decodificacao
					UINS.setR1(GetR1(instReg, UINS.getINST())); //decodificao
					UINS.setR2(getR2_IMM(instReg, UINS.getINST())); //decodificacao

					//#########################   OPERACOES MEMORIA   #####################################

					if (UINS.getINST() == "LDD" || UINS.getINST() == "STD")
					{
						address = logicToPhysic(UINS.getR2()); //decodificamos o endereco logico para fisico
						if (killProc == false) //se o endereco for valido ele continua
						{
							if (UINS.getINST() == "STX")
								MemOps(regBank[UINS.getR2()], UINS.getINST(), address, memoriastart);
							else
								MemOps(regBank[UINS.getR1()], UINS.getINST(), address, memoriastart);
						}
					}
					else if (UINS.getINST() == "LDX" || UINS.getINST() == "STX")
					{
						if (UINS.getINST() == "STX")
							address = logicToPhysic(regBank[UINS.getR1()]);  //decodificamos o endereco logico para fisico, nosso stx foi implementado ao contrario
						else
							address = logicToPhysic(regBank[UINS.getR2()]); //decodificamos o endereco logico para fisico

						if (killProc == false)//se o endereco for valido ele continua
						{
							if (UINS.getINST() == "STX")
								MemOps(regBank[UINS.getR2()], UINS.getINST(), address, memoriastart);
							else
								MemOps(regBank[UINS.getR1()], UINS.getINST(), address, memoriastart);
						}

					}
					//##########################################################################################

					//#############################    operacoes com a ULA    ##################################
					if (UINS.getINST() == "LDI" || UINS.getINST() == "ADDI" || UINS.getINST() == "SUBI" || UINS.getINST() == "ANDI" || UINS.getINST() == "ORI")
						ULA(regBank[UINS.getR1()], UINS.getR2(), UINS.getINST());
					else
						ULA(regBank[UINS.getR1()], regBank[UINS.getR2()], UINS.getINST());
					//#########################################################################################
					if (UINS.getINST() == "STOP") //se o processo terminar ele liga o flag de interrupcao e o flag de fim de processo
					{
						interrupcao = 3;
						processEnd = true;


					}


					//####################################   operacoes de salto   ##############################

					if (Branch(regBank[UINS.getR1()], regBank[UINS.getR2()], UINS.getINST()))
						if (UINS.getINST() == "JMP")
						{
							address = logicToPhysic(UINS.getR2()); //decodificamos o endereco logico para fisico
							if (killProc == false) //se o endereco for valido ele salta
								pc = address; //jumps e branches dependentes da particao
						}
						else
						{
							address = logicToPhysic(regBank[UINS.getR1()]); //decodificamos o endereco logico para fisico
							if (killProc == false) //se o endereco for valido ele salta
								pc = address; //jumps e branches dependentes da particao
						}
					else
						pc++;

					if (UINS.getINST() == "TRAP")
					{

						syscall();

					}
					if (killProc == true)
						kill();
					//#############################################################################################
					//std::cout << pc << "::::::" << regBank[2] << "!!!!!! " << ready.size() << " xxx " << blocked.size() << " ttt ";


					std::cout << "Processo:" << running.getID() << " " << UINS.getINST() << " " << UINS.getR1() << " " << UINS.getR2() << std::endl;
				}


			}
			if (desliga == true) //flag ativada depois de usar o comando [s] para desligar a maquina
				break;
		}
	}
}

int logicToPhysic(int logAdd)
{
	std::list<Pagina>::iterator it;
	int aux, offset;
	aux = logAdd / PAG_SIZE;
	offset = logAdd % PAG_SIZE;
	if (aux > running.pageTable.size()) //se o endereco nao fizer parte das paginas do processo
		killProc = true; //prepara para matar ele
	else
	{
		for (it = running.pageTable.begin(); it != running.pageTable.end(); it++) //busca pela frame da pagina que precisamos
		{
			if (it->getID() == aux) //quando encontramos
			{
				return (PAG_SIZE * it->getFrameID() + offset); //retornamos endereco da memoria fisica
			}
		}

		killProc = true; //se por algum motivo nao encontramos a pagina que estavamos procurando, algo deu errado, entao: matar processo
		return -1;
	}
}

//#######################################################
//#######################################################


//##################### Shell ###########################
//#######################################################
void shell()
{
	while (1)
	{
		std::cout << "Digite:" << std::endl;
		std::cout << "[shell C] para carregar um arquivo na memoria" << std::endl;
		std::cout << "[shell D] para fazer o dump na memoria" << std::endl;
		std::cout << "[shell M] para acessar uma posi��o x na memoria" << std::endl;
		std::cout << "[shell E] para inicia execucao" << std::endl;
		std::cout << "[shell W] para escrever em uma posicao na memoria" << std::endl;
		std::cout << "[shell L] para limpar a memoria" << std::endl;
		std::cout << "[shell R] para resetar a flag de execucao" << std::endl;
		std::cout << "[shell S] para sair" << std::endl;


		std::getline(std::cin, escolha);
		if (escolha.find("shell") != escolha.npos)
		{
			if (escolha == "shell R" || escolha == "shell r")
			{
				startExec = false;
				desliga = false;
			}
			else if (escolha == "shell C" || escolha == "shell c")
			{
				std::cout << "Digite o nome do programa a ser carregado" << std::endl;
				std::cin >> programa;
				criaProcesso();
				std::cin.ignore();
			}
			else if (escolha == "shell D" || escolha == "shell d")
			{
				dumpmem(memoriastart);
				std::cout << std::endl << std::endl;
			}
			else if (escolha == "shell E" || escolha == "shell e")
			{
				startExec = true;
			}
			else if (escolha == "shell M" || escolha == "shell m")
			{
				std::cout << "Digite a posicao que deseja verificar" << std::endl;
				std::cin >> posicao;
				std::cout << "Buscando a posicao de memoria " << posicao << std::endl;
				std::cout << "Valor encontrado: " << memoriastart[posicao] << std::endl << std::endl;
				std::cin.ignore();
			}
			else if (escolha == "shell S" || escolha == "shell s")
			{
				std::cout << "Desligando a Maquina" << std::endl << std::endl;
				desliga = true;

				delete[] memoriastart;
				return;
			}
			else if (escolha == "shell W" || escolha == "shell w")
			{
				std::cout << "Digite a posicao que deseja alterar na memoria" << std::endl;
				std::cin >> posicao;
				std::cout << "Agora digite o valor que deseja salvar na memoria" << std::endl;
				std::cin >> i;
				memoriastart[posicao] = i;
				std::cin.ignore();
			}
			else if (escolha == "shell L" || escolha == "shell l")
			{
				std::cout << "Limpando memoria..." << std::endl;
				limpamem(memoriastart, tamMemoria);
				std::cout << "Memoria apagada" << std::endl << std::endl;
			}
			else
			{
				std::cout << "Comando invalido, digite novamente" << std::endl;
			}

		}
	}
}
//#######################################################
//#######################################################




//##################### Modulo Input/Output #############
//#######################################################
void rotinaTratamentoIO()
{
	pedidoConsole pedido;
	int aux1, aux2;
	std::list<ProcessControlBlock>::iterator it;





	if (interrupcao == 1 || interrupcao == 2) //se nosso console tem algum pedido pronto
	{

		for (it = blocked.begin(); it != blocked.end(); it++)
		{

			if (pedidoPronto.getIDProcesso() == it->getID()) //procuramos o processo que pediu o IO
			{
				prontos.lock();
				ready.push_back(*it); //e colocamos ele de volta na fila de prontos


				blocked.erase(it);

				prontos.unlock();
				break;
			}
		}
		// Este snippet � para consertarmos um bug onde caso s� existisse 1 processo ele estava se multiplicando infinitamente.

		if (ready.size() == 1) // se a fila so tiver um processo
		{

			aux1 = ready.front().getID();
			aux2 = running.getID();

			if (aux1 == aux2) // e os dois tiverem PIDS iguais
			{
				//encontramos o farsante
				ready.pop_front(); // matamos ele
				restauraContexto(running); //e ficamos com o verdadeiro

			}
		}
		//////////////////////////////////////////////
	}
	interrupcao = 0;

}
void console()
{
	pedidoConsole pedido;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;
	int aux;
	std::string aux2;

	double tempo = 0.0;
	while (1)
	{

		intt.lock();//para controlar a concorr�ncia
		if (interrupcao == 0) //so podemos tratar se nao estivermos escalonando nem tratando outra interrupcao
		{
			if (filaPedidos.size() != 0) //enquanto tiverem pedidos, o console pega e vai tratando
			{
				pedido = filaPedidos.front();

				if (pedido.getTipo() == 1) //se for leitura da memoria para escrita no terminal
				{
					interrupcao = 1; //sinalizamos que o pedido foi tratado
					pedidoPronto = pedido; //sinalizamos que o pedido foi tratado
					filaPedidos.pop(); //e tiramos ele da fila

					std::cout << "Processo " << pedido.getIDProcesso() << " pediu uma leitura da memoria" << std::endl;
					std::cout << "Valor da posicao " << pedido.getParametro() << ": " << memoriastart[pedido.getParametro()] << std::endl;

				}
				else if (pedido.getTipo() == 2) //se for leitura do console para escrita na memoria
				{
					std::cout << "Processo " << pedido.getIDProcesso() << " pediu uma escrita na memoria" << std::endl;
					std::cout << "Digite o valor: ";

					std::getline(std::cin, consoleVal);
					if (consoleVal.find("io") != consoleVal.npos) //pescamos um input at� ele ser direcionado ao console
					{
						interrupcao = 2; //sinalizamos que o pedido foi tratado
						pedidoPronto = pedido; //sinalizamos que o pedido foi tratado
						filaPedidos.pop(); //e tiramos ele da fila

						aux2 = consoleVal.substr(consoleVal.find(' ')); //cortamos a string inicial para deixar s� o numero

						aux2 = aux2.erase(0, 1); //tiramos o espaco em branco

						std::istringstream aux3(aux2);
						aux3 >> aux; //e botamos da stringstream para um inteiro
						memoriastart[pedido.getParametro()] = aux;





					}
					else
					{

						interrupcao = 0;


					}

				}
			}

		}
		intt.unlock();//para controlar a concorr�ncia
		tempo = 0.0;

		if (desliga == true)
			break;
	}
}
void syscall()
{
	pedidoConsole pedido;

	{
		int address = 0;
		address = logicToPhysic(regBank[UINS.getR2()]); //verificamos se o endereco do trap esta dentro do limite
		if (killProc == true) //se nao estiver, retorna sem enfileirar o pedido
			return;
		pedido = pedidoConsole(address, regBank[UINS.getR1()], running.getID(), running.getBase()); //criamos um novo pedido

		salvaContexto(running); //salvamos contexto
		running.setEstado(BLOCKED);
		filaPedidos.push(pedido); //mandamos o pedido

		blocked.push_back(running); //e bloqueamos running
		if (ready.size() != 0) //se depois de bloquearmos o processo existe um processo pronto para executar 
		{
			//fazemos o escalonamento
			running = ready.front();
			restauraContexto(running);
			ready.pop_front();

		}
		else
		{
			wait = true; //se nao, trancamos a CPU

		}
	}
}
//#######################################################
//#######################################################





#endif