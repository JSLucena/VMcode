#include <iostream>
#include <string>
#include <fstream>

#include "FunctionsT2.h"
#include "Functions.h"


//Feito por Joaquim Lucena e Christian Bobsin

//fibV2.txt é o programa 1 com trap
//fio.txt é o programa 2 com trap
//fatV2.txt é o programa 3 com trap

//Nenhum dos fibonaccis printam os uns iniciais, pois o trap está dentro do loop e os uns são tratados fora do loop(Nao deu tempo de refazer o programa para incluir os uns)

//Como eh muito dificil debugar threads, não temos certeza se funciona 100% das vezes, os ultimos testes feitos todos funcionaram. Mas não podemos dizer que sempre é assim pois não deu tempo para fazer uma bateria melhor de testes

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
