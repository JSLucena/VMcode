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
		for (int i = 0; i < 64; i++)
			Frames[i] = false;

		limpamem(ptr, tamMemoria);
		std::thread tempo(timer);
		std::thread cpu(CPU);
		std::thread SHELL(shell);
		std::thread CONSOLE(console);
		tempo.join();
		cpu.join();
		SHELL.join();
		CONSOLE.join();
	
}


//Modulo de gerencia de Memoria:
/*
O gerente de memoria eh ativado quando temos um request de criacao/carregacao de um processo/programa na memoria
Ele pesquisa a primeira particao disponivel e aloca o processo dentro dela
Dentro da funcao de tratamentoTimer ela tem uma pequena atividade onde ela desaloca o processo que ja acabou
----------------------------------Estruturas Compartilhadas Utilizadas-------------------------------------
O Gerente de memoria se ativa na thread da shell, quando eh executado o comando 'shell c'

As estruturas utilizadas por ela sao:
-fila de processos ready
-pcb running
O PCB running eh thread safe, pois ele soh vai escalonar se nao houver nenhum processo running
A fila ready eh protegida para escrita, com os push_backs, atraves do Mutex Prontos, fazendo com que nao possa haver dois push_backs ao mesmo tempo
A fila ready eh protegida para leitura e pops, pois ela sempre pega a primeira posicao, ou seja, nao tem como sobreescrever aquele dado
*/

//Modulo de gerencia de processos:
/*
O gerente de processos cuida do escalonamento round robin entre o processo running e os processos da fila ready
Ele funciona sempre que o timeslice acaba, ou um processo acaba. Salvando o contexto do PCB running, pegando o primeiro processo da fila ready e restaurando seu contexto para dentro da CPU
--------------------------Estruturas Compartilhadas Utilizadas----------------------------------------------
O gerente de processos funciona dentro da thread da CPU, ja que o timeslice eh sinalizado por uma interrupcao e eh a CPU que trata as interrupcoes

As estruturas utilizadas por ela sao:
-a variavel de interrupcao
-a fila ready 
-o PCB running
O PCB running eh thread safe, pois os unicos momentos em que ele eh mudado, tirando o primeiro escalonamento da maquina, sao executados dentro da mesma thread, entao nao tem problema de ter duas operacoes ao mesmo tempo
A fila ready eh protegida para escrita, com os push_backs, atraves do Mutex Prontos, fazendo com que nao possa haver dois push_backs ao mesmo tempo
A fila ready eh protegida para leitura e pops, pois ela sempre pega a primeira posicao, ou seja, nao tem como sobreescrever aquele dado
A variavel de interrupcao, compartilhada com o timer, com a CPU e com o Console, eh protegida pelo mutex intt, que faz com que o recurso interrupcao esteja sob exclusao mutua
*/

//Modulo da CPU:
/*
A nossa CPU eh a responsavel pelas chamadas de sistema, tratamento de interrupcoes, e execucao de instrucoes do nosso programa
-----------------------------------Estruturas Compartilhadas Utilizadas----------------------------------------------
O Modulo da CPU eh composto por uma thread dela, e uma thread com o nosso timer para escalonamento
A CPU utiliza: 
-a interrupcao 
-processo running 
como variaveis compartilhadas com outras threads
A interrupcao eh protegida pois ela so reescreve o valor depois de entrar em uma das rotinas de tratamento, ou seja, as outras threads ja escreveram os valores necessarios
O processo running nao eh escrito pela CPU, eh utilizado para execucao do programa, e dentro do escalonador apenas.
*/

//Modulo IO:
/*
Nosso modulo de IO tem uma parte dentro da thread da CPU, que sao os tratamentos de TRAP e de interrupcoes
e uma parte na thread do Console, que funciona como nosso periferico que produz/consome os dados
--------------------------Estruturas Compartilhadas Utilizadas-------------------------------------------------
O Modulo de IO utiliza:
-variavel de interrupcao
-lista de processos bloqueados
-lista de processos prontos
-lista de pedidos
-processo running

A variavel de interrupcao eh protegida por exclusao mutua pelo mutex intt
A lista de processos bloqueados eh utilizada apenas dentro da CPU, pelo syscall ou pela rotina de tratamento de interrupcoes, entao nao eh possivel haver escritas concorrentes
a lista de processos prontos eh protegida por exclusao mutua pelo mutex prontos
a lista de pedidos eh protegida, pois apenas uma thread escreve(CPU) e uma thread le(console), se nao houver nada o console nao faz nada
o processo running eh protegido pois so mexemos nele se estamos escalonando ou no syscall, e como tanto o escalonamento, quanto o syscall acontecem na mesma thread, entao nao eh possivel acontecer escritas concorrentes
*/