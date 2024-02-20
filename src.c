#include <stdio.h>
#include <string.h>

// ------------------ BASIC STUFF STARTS HERE ------------------------------------------

typedef struct Board {
    // Represents a tic tac toe board.

    int turn; // represents the current turn. 1 = 'X', -1 = 'O'
    int boxes[9]; // represents the boxes of the board.
    float outcome; // represents the outcome of the board/game. -1 = 'O' won, 0 = ongoing, 0.5 = draw, 1 = 'X' won.
    int moves[9]; // represents an array of all possible moves, a value of -1 represents no move.
} Board; // declares Board as kind of a semi-type/alias to refer to this kind of struct as.

void initBoard(Board *board) {
    // Initializes the board object.

    board->turn = 1; // sets the turn to 'X' i.e. 1

    // Sets the boxes to their default value of 0 representing an empty box.
    // Also sets the possible moves in the list.
    // // b.boxes = {0, 0, 0, 0, 0, 0, 0, 0, 0}; <-- does not work.
    for (int k = 0; k < 9; k++) {
        // loop through the indexes from 0 to 8.
        board->boxes[k] = 0; // Sets the value of the box at that index to be zero i.e. empty.
        board->moves[k] = k; // Adds the index to the list of possible moves.
    }

    board->outcome = 0.0; // defaultly sets the game's outcome as ongoing.
    return;
}

void displayBoard(Board board) {
    // Displays the board

    char symbol = '_'; // declares a char that stores the symbol to be displayed.

    for (int k = 0; k < 9; k++) {
        // loops over the 9 indexes/boxes of the board

        // checks and assigns a symbol for every box.
        if (board.boxes[k] == 0) {
            symbol = '_'; // empty space
        }
        else if (board.boxes[k] == 1) {
            symbol = 'X'; // cross. represented by 1
        }
        else if (board.boxes[k] == -1) {
            symbol = 'O'; // zero. represented by -1
        }

        // checks to see if a newline character has to be introduced.
        if ((k + 1) % 3 == 0) {
            printf("%c\n", symbol);
        }
        else {
            printf("%c | ", symbol);
        }
    }

    printf("\n");
    return;
}

void changeTurn(Board *board) {
    // Changes/flips over the turn of the board.

    board->turn = -(board->turn); // flips the turn over. using the -> operator cuz pointer of a struct.
    return;
}

void setBoard(Board *board, int boxes[9]) {
    // Sets the board to a given array.

    for (int k = 0; k < 9; k++) {
        board->boxes[k] = boxes[k];
    }

    return;
}

void playMove(Board *board, int index) {
    // Plays a move on the board if its legal.

    if (!(index > -1 && index < 9)) {
        printf("ERROR: %d is not a valid move.\n", index);
        return;
    }

    if (board->boxes[index] != 0) {
        // Checks whether the box to be played in, is empty or not.
        printf("ERROR: The box is already filled at index [%d] and this move can not be played.\n", index);
        return;
    }

    board->boxes[index] = board->turn; // plays the move on the index.
    changeTurn(board); // changes turn.
    return;
}

float getOutcome(Board board) {
    // Determines the outcome of the board.
    // -1 = win for 'O'
    // 0 = ongoing
    // 0.5 = draw
    // 1 = win for 'X'

    // Checking for win patterns
    for (int k = -1; k < 3; k += 2) {
        // Loops over the two numbers -1 and 1 which represent 'O' and 'X'.

        // Checks for possible pattern matches and if true then returns the outcome of the game accordingly.
        if (board.boxes[0] == k && board.boxes[1] == k && board.boxes[2] == k) {
            return (float)k; // 012
        }
        else if (board.boxes[3] == k && board.boxes[4] == k && board.boxes[5] == k) {
            return (float)k; // 345
        }
        else if (board.boxes[6] == k && board.boxes[7] == k && board.boxes[8] == k) {
            return (float)k; // 678
        }
        else if (board.boxes[0] == k && board.boxes[3] == k && board.boxes[6] == k) {
            return (float)k; // 036
        }
        else if (board.boxes[1] == k && board.boxes[4] == k && board.boxes[7] == k) {
            return (float)k; // 147
        }
        else if (board.boxes[2] == k && board.boxes[5] == k && board.boxes[8] == k) {
            return (float)k; // 258
        }
        else if (board.boxes[0] == k && board.boxes[4] == k && board.boxes[8] == k) {
            return (float)k; // 048
        }
        else if (board.boxes[2] == k && board.boxes[4] == k && board.boxes[6] == k) {
            return (float)k; // 246
        }
    }

    // Checking for a tie.
    int count = 0;
    for (int k = 0; k < 9; k++) {
        if (board.boxes[k] == 0) {
            count++;
        }
    }
    if (count == 0) {
        return 0.5; // if the game has not been won and all boxes are filled its a tie so return 0.5
    }

    return 0.0; // if the game hasn't been won or tied then its ongoing.
}

void generateMoves(Board *board) {
    // Generates moves for the board.

    int move_c = 0; // initialize move count.
    float outcome = getOutcome(*board); // fetch the outcome of the game.

    if (outcome == 0.0) {
        // If the game is still ongoing and has not been drawn, or won yet.
        for (int k = 0; k < 9; k++) {
            // Loop through the boxes to check for empty ones and add them to the move list.
            if (board->boxes[k] == 0) {
                board->moves[move_c] = k; // Add them to the move list.
                move_c++; // Increment move count by 1.
            }
        }
    }

    // No more moves are possible so we fill them up with -1 to represent No move.
    while (move_c < 9) {
        board->moves[move_c] = -1;
        move_c++;
    }
}

void printMoves(int arr[9]) {
    printf("{");
    for (int k = 0; k < 9; k++) {
        printf("%d, ", arr[k]);
    }
    printf("\b\b}\n");
    return;
}

// ------------------ BASIC STUFF ENDS HERE ------------------------------------------

// ------------------- AI STUFF STARTS HERE ------------------------------------------

float evaluate(Board board, int turn) {
    // Evaluates a given board position for the side.
    float outcome = getOutcome(board); // Fetch the outcome of the game.

    if (outcome == 0) {
        // If the game is ongoing return zero.
        return 0;
    }
    if (outcome == 0.5) {
        // If the game is tied, thats still worse than it could be so return 0.5
        return -0.5;
    }

    return ((float)turn * outcome); // return 1 or -1 for win or loss, it multiplies it with the turn int to adjust for -1 being the outcome for O's win.
}

float negamax(Board board, int turn, int depth) {
    if (depth == 0) {
        return evaluate(board, turn);
    }

    generateMoves(&board);

    int k = 0;
    float maxScore = -999.9, currScore = -999.9;
    Board pseudoBoard;
    while (board.moves[k] != -1 && k < 9) {
        setBoard(&pseudoBoard, board.boxes);
        playMove(&pseudoBoard, board.moves[k]);

        currScore = -negamax(pseudoBoard, -board.turn, (depth - 1));
        if (currScore > maxScore) {
            maxScore = currScore;
        }

        k++;
    }

    return maxScore;
}

int predictMove(Board board, int turn, int depth) {
    float outcome = getOutcome(board);
    if (outcome != 0.0 || board.turn != turn) {
        return -1; // Return -1 as a No move, if no moves are possible due to a draw, loss or win.
    }
    
    generateMoves(&board);

    printMoves(board.moves);

    int k = 0, bestMove = board.moves[k];
    float bestScore = -999.9, currScore = -999.9;
    Board pseudoBoard;
    initBoard(&pseudoBoard);
    while (board.moves[k] != -1 && k < 9) {
        setBoard(&pseudoBoard, board.boxes);
        playMove(&pseudoBoard, board.moves[k]);

        currScore = -negamax(pseudoBoard, -turn, depth);
        if (currScore > bestScore) {
            bestMove = board.moves[k];
            bestScore = currScore;
        }
        displayBoard(pseudoBoard);
        printf("%d %f %f %d %d %d\n", board.moves[k], currScore, bestScore, bestMove, pseudoBoard.turn, board.turn);
        k++;
    }

    return bestMove;
}

// ------------------- AI STUFF ENDS HERE --------------------------------------------

// ------------------- USER INTERFACE STUFF STARTS HERE ------------------------------

void playGame(int depth) {
    Board board;
    int playerTurn = 1, playerMove, aiMove;

    // ----

    char turnSymbol, aiTurnSymbol;
    printf("Choose a turn:\n1. X\n2. O\n\n");
    scanf("%c", &turnSymbol);
    if (turnSymbol == 'X') {
        playerTurn = 1;
        aiTurnSymbol = 'O';
    }
    else if (turnSymbol == 'O') {
        playerTurn = -1;
        aiTurnSymbol = 'X';
    }
    else {
        printf("Invalid input supplied, exiting.");
        return;
    }
    printf("You\'ve chosen the %c turn! AI will be playing with %c turn.\n", turnSymbol, aiTurnSymbol);

    // ----

    initBoard(&board);
    if (playerTurn == 1) {
        displayBoard(board);
        printf("Possible Moves: ");
        printMoves(board.moves);
        printf("Type the index of the box you wanna play:\n 0 | 1 | 2\n 3 | 4 | 5\n 6 | 7 | 8\n\n");
        scanf("%d", &playerMove);
        
        playMove(&board, playerMove);
        printf("Player has played the move %d!\n", playerMove);

        displayBoard(board);
    }


    while (getOutcome(board) == 0.0) {
        generateMoves(&board);

        displayBoard(board);
        aiMove = predictMove(board, -playerTurn, depth);
        playMove(&board, aiMove);
        printf("The AI has played the move %d!\n", aiMove);
        displayBoard(board);

        // ----
        generateMoves(&board);

        displayBoard(board);
        printf("Possible Moves: ");
        printMoves(board.moves);
        printf("Type the index of the box you wanna play:\n 0 | 1 | 2\n 3 | 4 | 5\n 6 | 7 | 8\n\n");
        scanf("%d", &playerMove);
        
        playMove(&board, playerMove);
        printf("Player has played the move %d!\n", playerMove);
        displayBoard(board);
        continue;
    }

    float outcome = getOutcome(board);
    if (outcome == 0.5) {
        printf("The game was drawn.");
    }
    else if (outcome == playerTurn) {
        printf("The game was won by the player!!");
    }
    else {
        printf("The game was won by the AI!!");
    }
}

// ------------------- USER INTERFACE STUFF ENDS HERE --------------------------------

// Main function ---------------------------------------------------------------------
int main() {
    Board b; // declaring an instance b of Board.

    initBoard(&b);
    displayBoard(b); // this displays the board ofc :)

    // // int dummyArray[9] = {-1, 1, 1, 1, -1, -1, 1, -1, 1}; // initializes a dummy array with box values.
    // // setBoard(&b, dummyArray);
    // displayBoard(b);

    // printf("%f \n", getOutcome(b));
    // printf("%f \n", evaluate(b, 1));

    // generateMoves(&b);
    // printMoves(b.moves);

    // printf("NEGAMAX-SCORE: %f \n", negamax(b, -1, 10));
    // printf("PREDICTED-MOVE %d\n", predictMove(b, 10));

    // printf("\n\n\n");
    // getchar();

    playGame(10);
    getchar();
    return 0;
}
// -------------------------------------------------------------------------------------