#ifndef NAVIGATION_H_SEEN
#define NAVIGATION_H_SEEN

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define NORTE 0
#define SUL 1
#define LESTE 2
#define OESTE 3
#define ESQUERDA 0
#define DIREITA 1
#define FRENTE 2

typedef struct robot {
	int sock;
	struct sockaddr_in serv_addr;	
	int port;
	char buffer[1024];
	int position[2];
	int orientation;
} robot;

/*
	- Função:
		void send_msg(robot *, char *)
	- Descrição:
		Envia uma mensagem via socket à simulação.
	- Parâmetros:
		- (robot *): Estrutura robot
		- (char *): Mensagem
	- Retorna:
		-
*/
void send_msg(robot *, char *);

/*
	- Função:
		void send_msg(robot *, char *)
	- Descrição:
		Recebe uma mensagem via socket da simulação e a armazena
	    na estrutura r->buffer.
	- Parâmetros:
		- (robot *): Estrutura robot
    - Retorna:
        -
*/
void receive_msg(robot *);

/*
	- Função:
		int init(robot *, float)
	- Descrição:
		Inicializa a estrutura robot.
	- Parâmetros:
		- (robot *): Estrutura robot a ser inicializada
        - int: Velocidade da simulação
	- Retorna:
		- 0 em caso de sucesso e -1 caso contrário
*/
int init(robot *, int);

/*
	- Função:
		void move_straight(robot *, int)
	- Descrição:
		Move o robô para frente uma quantidade
		determinada de células. A função atuali-
		za corretamente a posição do robô.
	- Parâmetros:
		- (robot *): Estrutura robot
		- int: Quantidade de células a serem percorridas
	- Retorna:
		-
*/
void move_straight(robot *, int);

/*
	- Função:
		void turn(robot *, int)
	- Descrição:
		Rotaciona o robô na direção especificada.
		A função atualiza corretamente a orientação
		do robô.
	- Parâmetros:
		- (robot *): Estrutura robot
		- int: Direção da rotação. Possíveis valores:
			- ESQUERDA: para mover para a esquerda
			relativa do robô
			- DIREITA: para mover para a direita
			relativa do robô
	- Retorna:
		-
*/
void turn(robot *, int);

/*
	- Função:
		void status(robot *)
	- Descrição:
		Informações de status (posição e
        orientação) do robô.
	- Parâmetros:
		- (robot *): Estrutura robot		
	- Retorna:
		-
*/
void status(robot *);

/*
	- Função:
		int has_obstacle(robot *, int)
	- Descrição:
		Verifica se há um obstáculo na direção
		especificada
	- Parâmetros:
		- (robot *): Estrutura robot
		- int: Direção da checagem. Possíveis valores:
			- FRENTE: Verifica se há obstáculo a frente
			- ESQUERDA: Verifica se há obstaculo a esquerda
			- DIREITA: Verifica se há obstaculo a direita
	- Retorna:
		- 0 em caso negativo e um valor diferente 
        de 0 caso positivo
*/
int has_obstacle(robot *, int);

/*
	- Função:
		int check_objective(robot *)
	- Descrição:
		Verifica se o robô está sobre uma célula objetivo
	- Parâmetros:
		- (robot *): Estrutura robot		
	- Retorna:
		- 0 em caso negativo e um valor diferente
		de 0 caso positivo
*/
int check_objective(robot *);

#endif