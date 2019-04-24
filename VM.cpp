#include <iostream>
#include <string>
#include <fstream>

#include "FunctionsT2.h"
#include "Functions.h"




int main()

{



		memoriastart = new unsigned int[tamMemoria];
		ptr = memoriastart;
		limpamem(ptr, tamMemoria);
		std::thread tempo(timer);
		std::thread cpu(CPU);

	while (escolha != 'S' || escolha != 's')
	{
		std::cout << "Digite:" << std::endl;
		std::cout << "[C] para carregar um arquivo na memoria" << std::endl;
		std::cout << "[D] para fazer o dump na memoria" << std::endl;
		std::cout << "[M] para acessar uma posição x na memoria" << std::endl;
		std::cout << "[E] para inicia execucao" << std::endl;
		std::cout << "[W] para escrever em uma posicao na memoria" << std::endl;
		std::cout << "[L] para limpar a memoria" << std::endl;
		std::cout << "[R] para resetar a flag de execucao" << std::endl;
		std::cout << "[S] para sair" << std::endl;

		std::cin >> escolha;
		switch (escolha)
		{
		case 'R':
		case 'r':
			startExec = false;
			desliga = false;
			break;
		case 'C':
		case 'c':
			std::cout << "Digite o nome do programa a ser carregado" << std::endl;
			std::cin >> programa;
			criaProcesso();

			break;
		case 'D':
		case 'd':
			dumpmem(memoriastart);
			std::cout << std::endl << std::endl;
			break;
		case 'E':
		case 'e':
			startExec = true;
			
			break;
		case 'M':
		case 'm':
			std::cout << "Digite a posicao que deseja verificar" << std::endl;
			std::cin >> posicao;
			std::cout << "Buscando a posicao de memoria " << posicao << std::endl;
			std::cout << "Valor encontrado: " << memoriastart[posicao] << std::endl << std::endl;
			break;
		case 'S':
		case 's':
			std::cout << "Desligando a Maquina" << std::endl << std::endl;
			desliga = true;
			 tempo.join();
			 cpu.join();
			delete[] memoriastart;
			return 0;
		case 'W':
		case 'w':
			std::cout << "Digite a posicao que deseja alterar na memoria" << std::endl;
			std::cin >> posicao;
			std::cout << "Agora digite o valor que deseja salvar na memoria" << std::endl;
			std::cin >> i;
			memoriastart[posicao] = i;
			break;
		case 'L':
		case 'l':
			std::cout << "Limpando memoria..." << std::endl;
			limpamem(memoriastart, tamMemoria);
			std::cout << "Memoria apagada" << std::endl << std::endl;
			break;
		default:
			std::cout << "Comando invalido, digite novamente" << std::endl;
			break;
		}
	}
}
