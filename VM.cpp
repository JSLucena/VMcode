#include <iostream>
#include <string>
#include <fstream>

#include "FunctionsT2.h"
#include "Functions.h"


//Feito por Joaquim Lucena e Christian Bobsin

//fibV2.txt � o programa 1 com trap
//fio.txt � o programa 2 com trap
//fatV2.txt � o programa 3 com trap

//Nenhum dos fibonaccis printam os uns iniciais, pois o trap est� dentro do loop e os uns s�o tratados fora do loop(Nao deu tempo de refazer o programa para incluir os uns)

//Como eh muito dificil debugar threads, n�o temos certeza se funciona 100% das vezes, os ultimos testes feitos todos funcionaram. Mas n�o podemos dizer que sempre � assim pois n�o deu tempo para fazer uma bateria melhor de testes

int main()

{



		memoriastart = new unsigned int[tamMemoria];
		ptr = memoriastart;
		limpamem(ptr, tamMemoria);
		std::thread tempo(timer);
		std::thread cpu(CPU);
		std::thread SHELL(shell);
		std::thread CONSOLE(console);
	//	std::thread system(syscall);
		tempo.join();
		cpu.join();
		SHELL.join();
		CONSOLE.join();
	
}
