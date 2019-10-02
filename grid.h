#define GRID_NROWS 5
#define GRID_NCOLS 5

bool isAdjacent(int x1, int y1, int x2, int y2){
	if((abs(x1-x2) + abs(y1 - y2)) == 1)
		return true;
	else
		return false;
}

void print_grid(char *grid){
	int i,j;

	for(i=GRID_NROWS - 1;i>=0;i--){
		for(j=0;j<GRID_NCOLS;j++){
			writeDebugStream("%c\t", grid[GRID_NCOLS*i + j]);
		}
		writeDebugStreamLine("");
	}

}

bool set_grid(char *grid, int x, int y, char value){
	if(x >=0 && y >=0 && x < GRID_NCOLS && y < GRID_NROWS){
		grid[GRID_NCOLS*y + x] = value;
		return true;
	}
	return false;
}

void init_grid(char *grid, char default_value){
	int i;
	for(i=0;i<GRID_NROWS*GRID_NCOLS;i++)
		grid[i] = default_value;
}