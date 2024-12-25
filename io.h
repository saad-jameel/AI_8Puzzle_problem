//external variables declared from main.c
extern unsigned int nodesExpanded; 
extern unsigned int nodesGenerated;
extern unsigned int solutionLength;
extern double runtime;             

void welcomeUser(void) {
    printf("\n*/*/*/*/*/*/*/*/ 8 - PUZZLE PROBLEM SOLVER USING A* ALGORITHM /*/*/*/*/*/*/*/* \n");
}

void printInstructions(void) {
    printf(
        "______________________________________________________________________\n"
        "                   WELCOME TO 8 PUZZLE GAME. \n"
        "        PLEASE ENTER YOUR INITIAL AND TARGETED STATE BELOW.\n"
        "______________________________________________________________________\n"
    );
}

void inputState(State * const state) {
    state->action = NOT_APPLICABLE;
    char row, col;
    int symbol;

    char isNumUsed[9] = { 0 };

    for(row = 0; row < 3; ++row) {
        for(col = 0; col < 3; ++col) {
            printf("    board[%i][%i]: ", row, col);

            scanf("%i", &symbol);

            if(symbol >= 0 && symbol < 9) {
                if(!isNumUsed[symbol]) {
                    state->board[row][col] = symbol + '0';
                    isNumUsed[symbol] = 1;
                }
                else {
                    printf("    ERROR: Number %c is already used. Try again with different input.\n", symbol);
                    --col;
                }
            }
            else {
                printf("    ERROR: Invalid input. Enter a number from 0 to 8.\n");
                --col;
            }
        }
    }
    printf("\n");
}

void printBoard(char const board[][3]) {
    char row, col;

    for(row = 0; row < 3; ++row) {
        printf("_____________\n");
        for(col = 0; col < 3; ++col) {
            printf("| %c ", board[row][col]);
        }
        printf("|\n");
    }
    printf("_____________\n");
}

void printSolution(struct SolutionPath *path) {
	//check if solution exists
    if(!path) {
        printf("You entered Unsolveable State .\n");
        return;
    }

	if(!path->next) {
		printf("Sorry, Inital state is Equal to Target state. Please Enter different Inial or target states\n");
		return;
	}

    printf("SOLUTION: (Relative to the space character)\n");

    char *move[4] = { "UP", "DOWN", "LEFT", "RIGHT" };  //will use hash map to speed up the proccess a bit
    int counter = 1;

    for(path = path->next; path; path = path->next, ++counter) { //will be skipping the first node since it represents the initial state with no action
        printf("%i. Move %s\n", counter, move[path->action]);
    }

    printf(
        "DETAILS:\n"
        " - Solution length : %i\n"
        " - Nodes expanded  : %i\n"
        " - Nodes generated : %i\n"
        " - Runtime         : %g milliseconds\n"
        " - Memory used     : %i bytes\n", //only counting allocated `Node`s
        solutionLength, nodesExpanded, nodesGenerated, runtime, nodesGenerated * sizeof(Node));
}
