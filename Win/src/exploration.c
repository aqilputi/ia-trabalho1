#include "exploration.h"

void check_surroundings(robot *r){
    printf("###############################\n");
    printf("Tem obstáculo à esquerda? %d\n", has_obstacle(r, ESQUERDA));
    printf("Tem obstáculo à frente? %d\n", has_obstacle(r, FRENTE));
    printf("Tem obstáculo à direita? %d\n", has_obstacle(r, DIREITA));
	printf("Sobre objetivo? %d\n", check_objective(r));
	printf("###############################\n");
}