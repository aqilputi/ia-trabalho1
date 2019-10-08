#include "navigation.h"

void send_msg(robot *r, char *msg) { send(r->sock, msg, strlen(msg), 0); }

void receive_msg(robot *r) {
	int i;
	for (i = 0; i < 1024; i++) {
		r->buffer[i] = 0;
	}
	read(r->sock, r->buffer, 1024);
}
int init(robot *r, int simulation_speed) {
	int i;
	char cmd[50];

	r->port = 1234;
	for (i = 0; i < 1024; i++) {
		r->buffer[i] = 0;
	}
	if ((r->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	r->serv_addr.sin_family = AF_INET;
	r->serv_addr.sin_port = htons(r->port);

	if (inet_pton(AF_INET, "127.0.0.1", &(r->serv_addr.sin_addr)) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(r->sock, (struct sockaddr *)&(r->serv_addr),
				sizeof(r->serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	sprintf(cmd, "update_sim_spd %d!", simulation_speed);
	send_msg(r, cmd);
	receive_msg(r);
    return 0;
}

void move_straight(robot *r, int amount) {
	char cmd[50];
	if (amount < 1) {
		printf("\nQuantidade de celulas invalidas: %d\n", amount);
		return;
	}
	sprintf(cmd, "move_straight %d!", amount);
	send_msg(r, cmd);
	receive_msg(r);
	if (r->orientation == NORTE) {
		r->position[1] = r->position[1] + amount;
	} else if (r->orientation == SUL) {
		r->position[1] = r->position[1] - amount;
	} else if (r->orientation == LESTE) {
		r->position[0] = r->position[0] + amount;
	} else if (r->orientation == OESTE) {
		r->position[0] = r->position[0] - amount;
	}
}

void turn(robot *r, int direction) {
	char cmd[50];
	if (direction == ESQUERDA) {
		sprintf(cmd, "rotate_left!");
	} else if (direction == DIREITA) {
		sprintf(cmd, "rotate_right!");
	} else {
		printf("\nDireção inválida.\n");
		return;
	}
	send_msg(r, cmd);
	receive_msg(r);

	if (((r->orientation == NORTE) && (direction == ESQUERDA)) ||
		((r->orientation == SUL) && (direction == DIREITA))) {
		r->orientation = OESTE;
	} else if (((r->orientation == LESTE) && (direction == ESQUERDA)) ||
			   ((r->orientation == OESTE) && (direction == DIREITA))) {
		r->orientation = NORTE;
	} else if (((r->orientation == SUL) && (direction == ESQUERDA)) ||
			   ((r->orientation == NORTE) && (direction == DIREITA))) {
		r->orientation = LESTE;
	} else if (((r->orientation == OESTE) && (direction == ESQUERDA)) ||
			   ((r->orientation == LESTE) && (direction == DIREITA))) {
		r->orientation = SUL;
	}
}

void status(robot *r) {
	char orientation_str[20];
	if (r->orientation == NORTE) {
		sprintf(orientation_str, "NORTE");
	} else if (r->orientation == SUL) {
		sprintf(orientation_str, "SUL");
	} else if (r->orientation == LESTE) {
		sprintf(orientation_str, "LESTE");
	} else if (r->orientation == OESTE) {
		sprintf(orientation_str, "OESTE");
	}
	printf("Position (%d, %d). Orientation: '%s'\n", r->position[0],
		   r->position[1], orientation_str);
}

int has_obstacle_ahead(robot *r) {
	char *cmd = "has_obstacle!";
	send_msg(r, cmd);
	receive_msg(r);
	return (!strcmp(r->buffer, "True"));
}

int has_obstacle(robot *r, int direction) {
	int ret;
	if (direction == FRENTE) {
		return has_obstacle_ahead(r);
	} else {
		if (direction == ESQUERDA) {
			turn(r, ESQUERDA);
			ret = has_obstacle_ahead(r);
			turn(r, DIREITA);
			return ret;
		} else {
			turn(r, DIREITA);
			ret = has_obstacle_ahead(r);
			turn(r, ESQUERDA);
			return ret;
		}
	}
}

int check_objective(robot *r) {
	char *cmd = "check_objective!";
	send_msg(r, cmd);
	receive_msg(r);
	return (!strcmp(r->buffer, "True"));
}