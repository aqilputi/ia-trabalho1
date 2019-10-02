#define MINDISTANCE 50
#define TARGETCOLOR 0
#define TARGETCOLOR_CITY 59
#define CORRECTION_FACTOR .5
#define TOLERANCE 8
#define ESQUERDA 0
#define DIREITA 1
#define FRENTE 2
#define NORTE 0
#define SUL 1
#define LESTE 2
#define OESTE 3
#define TURN_DISTANCE 120
// #define TURN_DISTANCE 170
//#define CELL_SIZE 605 // Cell size: 30. Angular distance: 30*20,9 = 627
#define CELL_SIZE 1225 // Cell size: 30. Angular distance: 30*20,9 = 627

void init_robot(){
	resetMotorEncoder(motorC);
	resetMotorEncoder(motorB);

	int traveled_distance, init_dist, dir;
	int FORWARDSPEED = 10;
	init_dist = 50;
	dir = -1;

	do{
		motor[leftMotor] = dir*FORWARDSPEED;
		motor[rightMotor] = dir*FORWARDSPEED;
		wait1Msec(50);
		traveled_distance = (abs(getMotorEncoder(motorB))+abs(getMotorEncoder(motorC)))/2;

	}while(traveled_distance <= init_dist);
}

void move_straight(int ammount, int orientation, int *position)
{
	int counter = 1;
	int error;
	int FORWARDSPEED = 10;
	int adjustment;
	int traveled_distance;
	bool out_map = false;

	while(counter <= ammount){
		resetMotorEncoder(motorC);
		resetMotorEncoder(motorB);
		switch(orientation){
			case (NORTE):
				position[1] = position[1] + 1;
				out_map = position[1] == GRID_NCOLS-1 ? true : false;
			break;

			case (SUL):
				position[1] = position[1] - 1;
				out_map = position[1] == 0 ? true : false;
			break;

			case (LESTE):
				position[0] = position[0] + 1;
				out_map = position[0] == GRID_NROWS-1 ? true : false;
			break;

			case (OESTE):
				position[0] = position[0] - 1;
				out_map = position[0] == 0 ? true : false;
			break;

			default:
			break;
		}
		do
		{
			error = abs(SensorValue[colorSensor] - TARGETCOLOR);
			adjustment = round(error * CORRECTION_FACTOR);
			if (out_map && (traveled_distance >= 0.90 * CELL_SIZE)){
				writeDebugStream("Moving without corrections\n");
				motor[leftMotor] = FORWARDSPEED;
				motor[rightMotor] = FORWARDSPEED;
			}
			else{
				// motor[leftMotor] = FORWARDSPEED*0.5 + adjustment;
				// motor[rightMotor] = FORWARDSPEED*1.5 - adjustment;
				motor[leftMotor] = FORWARDSPEED*0.5 + adjustment;
				motor[rightMotor] = FORWARDSPEED*1.5 - adjustment;
			}
			wait1Msec(50);
			traveled_distance = (abs(getMotorEncoder(motorB))+abs(getMotorEncoder(motorC)))/2;

		}while(traveled_distance <= CELL_SIZE);
		traveled_distance = 0;
		counter++;		
		// writeDebugStream("Pos: (%d, %d) %s\n",  position[0], position[1], out_map? "Need fix" : "Dont need fix");		
	}
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
}

void turn(int direction, int *orientation){
	int distance;
	int error;
	int FORWARDSPEED = 10;
	resetMotorEncoder(motorC);
	resetMotorEncoder(motorB);

	if(direction == DIREITA){
		do{
			motor[leftMotor] = FORWARDSPEED;
			motor[rightMotor] = -FORWARDSPEED;
			error = abs(SensorValue[colorSensor] - TARGETCOLOR);
			distance = getMotorEncoder(motorB);
			// writeDebugStream("Distance:%d, error: %d\n", distance, error);
		}while(distance < TURN_DISTANCE || error > TOLERANCE);

		switch(*orientation){
			case (NORTE):
				*orientation = LESTE;
			break;

			case (SUL):
				*orientation = OESTE;
			break;

			case (LESTE):
				*orientation = SUL;
			break;

			case (OESTE):
				*orientation = NORTE;
			break;

			default:
			break;
		}
	}else if(direction == ESQUERDA){
		do{
			motor[leftMotor] = -FORWARDSPEED;
			motor[rightMotor] = FORWARDSPEED;
			error = abs(SensorValue[colorSensor] - TARGETCOLOR);
			distance = getMotorEncoder(motorC);
			// writeDebugStream("Distance:%d, error: %d\n", distance, error);
		}while(distance < TURN_DISTANCE || error > TOLERANCE);

		switch(*orientation){
			case (NORTE):
				*orientation = OESTE;
			break;

			case (SUL):
				*orientation = LESTE;
			break;

			case (LESTE):
				*orientation = NORTE;
			break;

			case (OESTE):
				*orientation = SUL;
			break;

			default:
			break;
		}
	}
	// writeDebugStream("Orientation: %s\n", *orientation == 0 ? "NORTE" : (*orientation == 1? "SUL" : (*orientation == 2? "LESTE": "OESTE")));
	wait1Msec(50);
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
}

// OBS: NUNCA CHAMAR A FUNCAO ABAIXO SE A PONTA DO ROBO NAO ESTIVER NA FAIXA!!!
bool has_obstacle(int direction, int *position, int *orientation){
	bool ret;

	switch (direction){
		case ESQUERDA:
			if ((position[0] == 0 && *orientation == NORTE) ||
			(position[1] == GRID_NROWS-1 && *orientation == LESTE) ||
			(position[0] == GRID_NCOLS-1 && *orientation == SUL) ||
			(position[1] == 0 && *orientation == OESTE)){
				return true;
			}
			else{
				turn(ESQUERDA, orientation);				
				ret = SensorValue[sonarSensor] <= MINDISTANCE;				
				turn(DIREITA, orientation);
				return ret;								
			}
		
		case DIREITA:	
			if ((position[0] == 0 && *orientation == SUL) ||
			(position[1] == GRID_NROWS-1 && *orientation == OESTE) ||
			(position[0] == GRID_NCOLS-1 && *orientation == NORTE) ||
			(position[1] == 0 && *orientation == LESTE)){
				return true;
			}
			else{	
				turn(DIREITA, orientation);
				ret = SensorValue[sonarSensor] <= MINDISTANCE;				
				turn(ESQUERDA, orientation);
				return ret;
			}							

		case FRENTE:
			if ((position[0] == 0 && *orientation == OESTE) ||
			(position[1] == GRID_NROWS-1 && *orientation == NORTE) ||
			(position[0] == GRID_NCOLS-1 && *orientation == LESTE) ||
			(position[1] == 0 && *orientation == SUL)){
				return true;
			}			
			return SensorValue[sonarSensor] <= MINDISTANCE;
			
		default:
			break;
	}
	return true;
}


bool check_objective(){
	bool detected_collor;
	int distance;
	int error;
	int FORWARDSPEED = 10;

	resetMotorEncoder(motorC);
	resetMotorEncoder(motorB);
	detected_collor = false;
	do{
		motor[leftMotor] = FORWARDSPEED;
		motor[rightMotor] = -FORWARDSPEED;
		error = abs(SensorValue[colorSensor] - TARGETCOLOR);
		distance = getMotorEncoder(motorB);
	}while(distance < TURN_DISTANCE/3);

	error = abs(SensorValue[colorSensor] - TARGETCOLOR_CITY);
	if (error < TOLERANCE)
		detected_collor = true;

	resetMotorEncoder(motorC);
	resetMotorEncoder(motorB);
	do{
			motor[leftMotor] = -FORWARDSPEED;
			motor[rightMotor] = FORWARDSPEED;
			error = abs(SensorValue[colorSensor] - TARGETCOLOR);
			distance = getMotorEncoder(motorC);
			// writeDebugStream("Distance:%d, error: %d\n", distance, error);
	}while(distance < TURN_DISTANCE/3);

	return detected_collor;
}

void check_surrounding(int *position, int *orientation){
    writeDebugStream("Obs esq? %s\n", has_obstacle(ESQUERDA, position, orientation)? "SIM":"NAO");
	writeDebugStream("Obs frente? %s\n", has_obstacle(FRENTE, position, orientation)? "SIM":"NAO");
	writeDebugStream("Obs dir? %s\n", has_obstacle(DIREITA, position, orientation)? "SIM":"NAO");
	writeDebugStream("Goal? %s\n", check_objective()? "SIM":"NAO");
}