#ifndef EXPLORATION_H_SEEN
#define EXPLORATION_H_SEEN

#include "navigation.h"
#include <stdio.h>

#define NAO_EXPLORADO 0
#define EXPLORADO 1
#define OBSTACULO 2
#define OBJETIVO 3
#define FIM 4

void check_surroundings(robot *);

#endif