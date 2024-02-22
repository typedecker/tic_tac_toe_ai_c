// TIC TAC TOE Negamax AI
// By typedecker

// Description:
// A practice project to get used to C's advanced syntactic sugar. A negamax tic tac toe AI.

// Notes:
// I might make a few more versions of this with minor/major code optimizations and fixes in the hopes of learning more from it!

// ---------------------------------------------------------------------------------------------------------------------------

// Header file imports ----
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// ----

// ------------------ BASIC STUFF STARTS HERE ------------------------------------------

typedef struct Board {
    // Represents a tic tac toe board.

    bool turn; // represents the current turn. true = 'X', false = 'O'
    uint8_t outcome; // represents the outcome of the board/game. 0 = 'O' won, 1 = ongoing, 2 = draw, 3 = 'X' won.
    uint8_t boxes[9]; // represents the boxes of the board. 0 = empty, 1 = 'X', 2 = 'O'
    uint8_t moves[9]; // represents an array of all possible moves, a value of 0 represents no move. Move indices start from 1.. to 9.
} Board; // declares Board as kind of a semi-type/alias to refer to this kind of struct as.

void initBoard(Board * restrict board) {
    // Initializes the board object.

    board->turn = true; // sets the turn to 'X' i.e. 1

    // Sets the boxes to their default value of 0 representing an empty box.
    // Also sets the possible moves in the list.
    // // b.boxes = {0, 0, 0, 0, 0, 0, 0, 0, 0}; <-- does not work.
    for (int k = 0; k < 9; k++) {
        // loop through the indexes from 0 to 8.
        board->boxes[k] = 0; // Sets the value of the box at that index to be zero i.e. empty.
        board->moves[k] = (k + 1); // Adds the index to the list of possible moves.
    }

    board->outcome = 1; // defaultly sets the game's outcome as ongoing.
    return;
}

void displayBoard(const Board * restrict board) {
    // Displays the board

    char symbol = '_'; // declares a char that stores the symbol to be displayed.

    for (int k = 0; k < 9; k++) {
        // loops over the 9 indexes/boxes of the board

        // checks and assigns a symbol for every box.
        if (board->boxes[k] == 0) {
            symbol = '_'; // empty space. represented by 0
        }
        else if (board->boxes[k] == 1) {
            symbol = 'X'; // cross. represented by 1
        }
        else if (board->boxes[k] == 2) {
            symbol = 'O'; // zero. represented by 2
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

void changeTurn(Board *restrict board) {
    // Changes/flips over the turn of the board.
    // NOTE: Might not get used anymore eventually cuz the one liner can be directly added to code without much extra space requirement.

    board->turn = !(board->turn); // flips the turn over. using the -> operator cuz pointer of a struct.
    return;
}

void setBoard(Board *restrict board, uint8_t boxes[9]) {
    // Sets the board to a given array.

    for (int k = 0; k < 9; k++) {
        board->boxes[k] = boxes[k];
    }

    return;
}

void playMove(Board *restrict board, int index) {
    // Plays a move on the board if its legal.

    if (!(index > 0 && index < 10)) {
        printf("ERROR: %d is not a valid move.\n", index);
        return;
    }

    if (board->boxes[(index - 1)] != 0) {
        // Checks whether the box to be played in, is empty or not.
        int val = board->boxes[(index - 1)];
        printf("ERROR: The box is already filled at index [%d] and this move can not be played.\n", index);
        return;
    }

    board->boxes[(index - 1)] = board->turn? 1 : 2; // plays the move on the index. [0 = empty, 1 = 'X', 2 = 'O'].
    board->turn = !(board->turn); // changes turn.
    return;
}

uint8_t getOutcome(const Board *restrict board) {
    // Determines the outcome of the board.
    // 0 = win for 'O'
    // 1 = ongoing
    // 2 = draw
    // 3 = win for 'X'

    // Checking for win patterns
    for (int k = 1; k < 3; k += 1) {
        // Loops over the two numbers -1 and 1 which represent 'O' and 'X'.

        // Checks for possible pattern matches and if true then returns the outcome of the game accordingly.
        if (board->boxes[0] == k && board->boxes[1] == k && board->boxes[2] == k) {
            return 3 * !(bool)(k - 1); // 012
        }
        else if (board->boxes[3] == k && board->boxes[4] == k && board->boxes[5] == k) {
            return 3 * !(bool)(k - 1); // 345
        }
        else if (board->boxes[6] == k && board->boxes[7] == k && board->boxes[8] == k) {
            return 3 * !(bool)(k - 1); // 678
        }
        else if (board->boxes[0] == k && board->boxes[3] == k && board->boxes[6] == k) {
            return 3 * !(bool)(k - 1); // 036
        }
        else if (board->boxes[1] == k && board->boxes[4] == k && board->boxes[7] == k) {
            return 3 * !(bool)(k - 1); // 147
        }
        else if (board->boxes[2] == k && board->boxes[5] == k && board->boxes[8] == k) {
            return 3 * !(bool)(k - 1); // 258
        }
        else if (board->boxes[0] == k && board->boxes[4] == k && board->boxes[8] == k) {
            return 3 * !(bool)(k - 1); // 048
        }
        else if (board->boxes[2] == k && board->boxes[4] == k && board->boxes[6] == k) {
            return 3 * !(bool)(k - 1); // 246
        }
    }

    // Checking for a tie.
    int count = 0;
    for (int k = 0; k < 9; k++) {
        if (board->boxes[k] == 0) {
            count++;
        }
    }
    if (count == 0) {
        return 2; // if the game has not been won and all boxes are filled its a tie so return 0.5
    }

    return 1; // if the game hasn't been won or tied then its ongoing.
}

void generateMoves(Board *restrict board) {
    // Generates moves for the board.

    int move_c = 0; // initialize move count.
    uint8_t outcome = getOutcome(board); // fetch the outcome of the game.

    if (outcome == 1) {
        // If the game is still ongoing and has not been drawn, or won yet.
        for (int k = 0; k < 9; k++) {
            // Loop through the boxes to check for empty ones and add them to the move list.
            if (board->boxes[k] == 0) {
                board->moves[move_c] = (k + 1); // Add them to the move list.
                move_c++; // Increment move count by 1.
            }
        }
    }

    // No more moves are possible so we fill them up with -1 to represent No move.
    while (move_c < 9) {
        board->moves[move_c] = 0;
        move_c++;
    }
}

void printMoves(uint8_t arr[9]) {
    printf("{");
    for (int k = 0; k < 9; k++) {
        printf("%d, ", arr[k]);
    }
    printf("\b\b}\n");
    return;
}

// ------------------ BASIC STUFF ENDS HERE ------------------------------------------

// ------------------- AI STUFF STARTS HERE ------------------------------------------

float evaluate(Board *restrict board, bool turn) {
    // Evaluates a given board position for the side.
    uint8_t outcome = getOutcome(board); // Fetch the outcome of the game.

    if (outcome == 1) {
        // If the game is ongoing return zero.
        return 0.0;
    }
    else if (outcome == 2) {
        // If the game is tied, thats still worse than it could be so return 0.5
        return -0.5;
    }
    else if (outcome == 3) {
        return 99.9 * (turn? 1 : -1); // Return 99.9 as the score for a win and -99.9 for a loss.
    }
    else if (outcome == 0) {
        return 99.9 * (turn? -1 : 1); // Return 99.9 as the score for a win and -99.9 for a loss.
    }
    printf("%d", (int)outcome);
    return 0.0;
}

float negamax(Board *restrict board, bool turn, int depth) {
    // Negamax function that can search a certain number of moves deep to score a position accordingly.

    if (depth == 0 || getOutcome(board) != 1) {
        // If depth is zero or the game outcome has already been decided then directly return the evaluation.
        return evaluate(board, turn);
    }

    generateMoves(board); // Generate the moves array for the board internally.

    int k = 0; // Move index.
    float maxScore = 0.0, currScore = 0.0; // Score trackers.
    Board pseudoBoard; // Pseudo Board for making moves.
    initBoard(&pseudoBoard); // Initialize the pseudo board with default values.
    while (board->moves[k] != 0 && k < 9) {
        // Loop till we hit a "No Move" or hit the max move count.
        setBoard(&pseudoBoard, board->boxes); // Set the pseudo board to the board's position.
        pseudoBoard.turn = board->turn; // Set the turn of the pseudo board to the board's turn.
        playMove(&pseudoBoard, board->moves[k]); // Play the move being checked on the pseudo board.

        currScore = -negamax(&pseudoBoard, !turn, (depth - 1)); // Fetch the score for this newly obtained position.
        // if (currScore > 1) printf("%f", currScore);
        if (currScore > maxScore) {
            // If the score for this move's after position is higher than the current max, then set it as max.
            maxScore = currScore;
        }
        k++; // Increment the move index.
    }

    return maxScore; // Return the maximum score value from the search.
}

int predictMove(Board *restrict board, bool turn, int depth) {
    // Predicts the best move to play for a given position.

    float outcome = getOutcome(board); // Fetch the outcome of the game.
    if (outcome != 1.0 || board->turn != turn) {
        // If the game has already ended, no moves can be played.
        return 0; // Return -1 as a No move, if no moves are possible due to a draw, loss or win.
    }
    
    generateMoves(board); // Generates the internal moves list for the board object.
    printMoves(board->moves);

    int k = 0, bestMove = board->moves[k]; // Move index and the default Best Move is the first one in the list.
    float bestScore = -999.9, currScore = -999.9; // We take the best and current scores to be highly negative to incentivize choosing a higher score.
    Board pseudoBoard; // Pseudo Board for making moves,
    initBoard(&pseudoBoard); // Initialize the pseudo board with default values.
    while (board->moves[k] != 0 && k < 9) {
        // Loop till we hit a "No Move" or hit the max move count.
        setBoard(&pseudoBoard, board->boxes); // Set the pseudo board to the board's position.
        pseudoBoard.turn = board->turn; // Set the turn of the pseudo board to the board's turn.
        playMove(&pseudoBoard, board->moves[k]); // Play the move being checked on the pseudo board.

        currScore = -negamax(&pseudoBoard, !turn, depth); // Fetch the score for this newly obtained position.
        if (currScore > bestScore) {
            // If the score for this move's after position is higher than the current max, then set it as max
            // Also set the current best move as this move.
            bestMove = board->moves[k];
            bestScore = currScore;
        }
        k++; // Increment the move index
    }

    return bestMove; // Return the best move.
}

// ------------------- AI STUFF ENDS HERE --------------------------------------------

// ------------------- USER INTERFACE STUFF STARTS HERE ------------------------------

void playGame(int depth) {
    // Provides a user interface over the console for a player vs AI experience.

    // Initialize default stuff ---
    Board board;
    bool playerTurn = 1;
    int playerMove;
    uint8_t aiMove;

    // ----

    // Turn Selection for the player ----
    char turnSymbol, aiTurnSymbol;
    printf("Choose a turn:\n1. X\n2. O\n\n");
    scanf("%c", &turnSymbol);
    if (turnSymbol == 'X') {
        playerTurn = 1;
        aiTurnSymbol = 'O';
    }
    else if (turnSymbol == 'O') {
        playerTurn = 2;
        aiTurnSymbol = 'X';
    }
    else {
        printf("Invalid input supplied, exiting.");
        return;
    }
    printf("You\'ve chosen the %c turn! AI will be playing with %c turn.\n", turnSymbol, aiTurnSymbol);

    // ----

    // If the player is first, then this allows the player to play a move.
    initBoard(&board);
    if (playerTurn == 1) {
        displayBoard(&board);
        printf("Possible Moves: ");
        printMoves(board.moves);
        printf("Type the index of the box you wanna play:\n 1 | 2 | 3\n 4 | 5 | 6\n 7 | 8 | 9\n\n");
        scanf("%d", &playerMove);
        
        playMove(&board, playerMove);
        printf("Player has played the move %d!\n", playerMove);

        displayBoard(&board);
    }

    // While the game is still ongoing...
    while (getOutcome(&board) == 1) {
        // AI Move play ----
        generateMoves(&board); // Generate the internal moves array for the board.

        displayBoard(&board);
        printMoves(board.moves);
        aiMove = predictMove(&board, !playerTurn, depth);
        playMove(&board, aiMove);
        printf("The AI has played the move %d!\n", aiMove);
        displayBoard(&board);

        // Checking for game end ----

        if (getOutcome(&board) != 1) {
            break;
        }

        // ----

        // Player Move play ----
        generateMoves(&board);

        displayBoard(&board);
        printf("Possible Moves: ");
        printMoves(board.moves);
        printf("Type the index of the box you wanna play:\n 1 | 2 | 3\n 4 | 5 | 6\n 7 | 8 | 9\n\n");
        scanf("%d", &playerMove);
        
        playMove(&board, playerMove);
        printf("Player has played the move %d!\n", playerMove);
        displayBoard(&board);

        // ----
        continue;
    }

    // Fetch the outcome of the match and print it accordingly.
    uint8_t outcome = getOutcome(&board);
    if (outcome == 2) {
        printf("The game was drawn.");
    }
    else if (outcome == (playerTurn? 3 : 0)) {
        printf("The game was won by the player!!");
    }
    else {
        printf("The game was won by the AI!!");
    }
    
    return;
}

// ------------------- USER INTERFACE STUFF ENDS HERE --------------------------------

// Main function ---------------------------------------------------------------------
int main() {
    // Main function, this is where the control flow starts from.

    playGame(20); // Play a game with a depth of 20.
    getchar(); // For cases where the console might suddenly close.
    return 0; // Return 0 to signify no error.
}
// -------------------------------------------------------------------------------------