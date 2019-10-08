#include "navigation.h"
#include "exploration.h"
#include <stdio.h>
//#include <sys/cygwin.h>
#include "p1.h"

//movimentos do carro em relação ao mapa
#define M_CIMA 1
#define M_DIREITA 2
#define M_BAIXO 3
#define M_ESQUERDA 4


int main(int argc, char const *argv[]) {
  //cygwin_internal (CW_SYNC_WINENV);

	robot r;
	int spd, i, j;
	int grid[5][5];

	// Convertendo argumento passado como parametro para inteiro
	if(argc > 1){
		spd = strtol(argv[1], NULL, 10);
	}
	else{
		spd = 3;
	}

	// Inicializando estrutura robot
	init(&r, spd);

	// Inicializando grid
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			grid[i][j] = NAO_EXPLORADO;
		}
	}
	// Inicializando posição e orientação do robô
	r.position[0] = 4;
	r.position[1] = 0;
	r.orientation = NORTE;

	// Posição inicial do robô já foi explorada, logo:
	grid[r.position[0]][r.position[1]] = EXPLORADO;

	// Sabemos onde é o fim do labirinto, logo:
	grid[2][0] = FIM;

	// Contador de numero de objetivos encontrados
	int goals = 0;

	int nao_ha_caminho = 0;
	//status de fim do labirinto
	//int end = 0;

	check_surroundings(&r);

	//pilha auxiliar para execução do dfs
    list* l = malloc(sizeof(list));
    list_n node;
    initialize_list(l, QEUE);

	int stack[1000];
	int *stack_p = stack;

	//rotina de busca pelos objetivos
	while(goals < 3 || grid[r.position[0]][r.position[1]] != FIM){
      printf("caminho atual: %d %d\n", r.position[0], r.position[1]);
			nao_ha_caminho = 1;

            node.x = r.position[0];
            node.y = r.position[1];
			//checa para ver se está em um goal

			if(check_objective(&r) && grid[r.position[0]][r.position[1]] != EXPLORADO)
				goals++;
			//marca o lugar atual como visitado
			grid[r.position[0]][r.position[1]] = EXPLORADO;
			printf("goals: %d\n", goals);


            
            
            if(grid[r.position[0]][r.position[1] - 1] == NAO_EXPLORADO ){ //SUL (baixo)
              printf("passando por aqui\n");
              turn(&r, ESQUERDA);
              turn(&r, ESQUERDA);
              if(!has_obstacle(&r, FRENTE)){
                node.direcao = M_BAIXO;
                lpush(l, node);
              }
              turn(&r, DIREITA);
              turn(&r, DIREITA);
			}

            if(!has_obstacle(&r, ESQUERDA)
               && grid[r.position[0] - 1][r.position[1]] == NAO_EXPLORADO){//OESTE (esquerda)
              printf("NAO TEM OBSTACULO A ESQUERDA, MOVENDO PARA ESQUERDA\n");

              node.direcao = M_ESQUERDA;
              lpush(l, node);
			}

            if(!has_obstacle(&r, DIREITA)
               && grid[r.position[0] + 1][r.position[1]] == NAO_EXPLORADO){//LESTE (direita)
              printf("NAO TEM OBSTACULO A DIREITA, MOVENDO PARA DIREITA\n");
              node.direcao = M_DIREITA;
              lpush(l, node);
			}


			// Checa a proxima direção que seguirá. A ordem de checagem é: Norte -> Leste -> Sul -> Oeste
			if(!has_obstacle(&r, FRENTE)
               && grid[r.position[0]][r.position[1] + 1] == NAO_EXPLORADO){//NORTE (cima)
              printf("NAO TEM OBSTACULO A FRENTE, MOVENDO PARA FRENTE\n");

              node.direcao = M_CIMA;
              lpush(l, node);
			}

            			

			

            list_n  proximo;
            int rollback;

            lpop(l, &proximo);
            printf("proximo no: %d %d\n", proximo.x, proximo.y);
            while(proximo.x != r.position[0] || proximo.y != r.position[1]){
              rollback = pop(stack_p);
              printf("BACKTRACKING...\n");

              if(rollback == M_CIMA){
					move_straight(&r,1);
				}
				else if(rollback == M_DIREITA){
					turn(&r, DIREITA);
					move_straight(&r, 1);
					turn(&r, ESQUERDA);
				}
				else if (rollback == M_ESQUERDA){
					turn(&r, ESQUERDA);
					move_straight(&r, 1);
					turn(&r, DIREITA);
				}else if (rollback == M_BAIXO){
					turn(&r, ESQUERDA);
					turn(&r, ESQUERDA);
					move_straight(&r, 1);
					turn(&r, DIREITA);
					turn(&r, DIREITA);
				}
              
            }

            switch(proximo.direcao){
            case M_CIMA:
              move_straight(&r, 1);
              push(stack_p, M_BAIXO);
              break;
            case M_DIREITA:
              turn(&r, DIREITA);
              move_straight(&r, 1);
              turn(&r, ESQUERDA);
              push(stack_p, M_ESQUERDA);
              break;
            case M_ESQUERDA:
              turn(&r, ESQUERDA);
              move_straight(&r, 1);
              turn(&r, DIREITA);
              push(stack_p, M_DIREITA);
              break;
            case M_BAIXO:
				turn(&r, ESQUERDA);
				turn(&r, ESQUERDA);
                move_straight(&r, 1);
                nao_ha_caminho = 0;
				turn(&r, DIREITA);
				turn(&r, DIREITA);
                push(stack_p, M_CIMA);
              break;
            }


			if(nao_ha_caminho == 1){ // NAO HÁ  MAIS CAMINHO, FAÇA BACKTRACKING
			}
	}

/*
########## demo ##########
	check_surroundings(&r);

	move_straight(&r, 4);
	turn(&r, ESQUERDA);
	check_surroundings(&r);

	move_straight(&r, 4);
	turn(&r, ESQUERDA);
	check_surroundings(&r);

	move_straight(&r, 3);
	turn(&r, ESQUERDA);
	check_surroundings(&r);

	move_straight(&r, 2);
	turn(&r, DIREITA);
	check_surroundings(&r);

	move_straight(&r, 1);
	check_surroundings(&r);
##########################
*/
	return 0;
}
