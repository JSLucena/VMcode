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
		std::thread SHELL(shell);

		tempo.join();
		cpu.join();
		SHELL.join();

	
}
