#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Define game constants
#define BOARD_SIZE 11
#define EMPTY ' '
#define ATTACKER 'A'
#define DEFENDER 'D'
#define KING 'K'
#define THRONE 'T'
#define CORNER 'C'

// Game state
typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    bool attackerTurn;
    bool gameOver;
    char winner;
} GameState;

// Function prototypes
void initializeGame(GameState *state);
void printBoard(GameState *state);
bool makeMove(GameState *state, int fromRow, int fromCol, int toRow, int toCol);
bool isValidMove(GameState *state, int fromRow, int fromCol, int toRow, int toCol);
void checkCaptures(GameState *state, int row, int col);
bool isGameOver(GameState *state);
void clearScreen();
void printHelp();
char* pieceToSymbol(char piece);
void getPlayerMove(GameState *state, int *fromRow, int *fromCol, int *toRow, int *toCol);

int main() {

    return 0;
}

void initializeGame(GameState *state) {
    // Initialize empty board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            state->board[i][j] = EMPTY;
        }
    }

    // Mark the corners
    state->board[0][0] = CORNER;
    state->board[0][BOARD_SIZE-1] = CORNER;
    state->board[BOARD_SIZE-1][0] = CORNER;
    state->board[BOARD_SIZE-1][BOARD_SIZE-1] = CORNER;

    // Mark the throne (center)
    state->board[BOARD_SIZE/2][BOARD_SIZE/2] = THRONE;

    // Place the king
    state->board[BOARD_SIZE/2][BOARD_SIZE/2] = KING;

    // Place defenders
    state->board[BOARD_SIZE/2-1][BOARD_SIZE/2] = DEFENDER;
    state->board[BOARD_SIZE/2+1][BOARD_SIZE/2] = DEFENDER;
    state->board[BOARD_SIZE/2][BOARD_SIZE/2-1] = DEFENDER;
    state->board[BOARD_SIZE/2][BOARD_SIZE/2+1] = DEFENDER;
    state->board[BOARD_SIZE/2-2][BOARD_SIZE/2] = DEFENDER;
    state->board[BOARD_SIZE/2+2][BOARD_SIZE/2] = DEFENDER;
    state->board[BOARD_SIZE/2][BOARD_SIZE/2-2] = DEFENDER;
    state->board[BOARD_SIZE/2][BOARD_SIZE/2+2] = DEFENDER;

    state->board[4][4] = DEFENDER;
    state->board[6][4] = DEFENDER;
    state->board[6][6] = DEFENDER;
    state->board[4][6] = DEFENDER;

    // Place attackers
    // Top
    for (int i = 3; i <= 7; i++) {
        state->board[0][i] = ATTACKER;
    }
    // Bottom
    for (int i = 3; i <= 7; i++) {
        state->board[BOARD_SIZE-1][i] = ATTACKER;
    }
    // Left
    for (int i = 3; i <= 7; i++) {
        state->board[i][0] = ATTACKER;
    }
    // Right
    for (int i = 3; i <= 7; i++) {
        state->board[i][BOARD_SIZE-1] = ATTACKER;
    }

    // Set one outer attacker piece at all 4 sides respectively
    state->board[5][1] = ATTACKER;
    state->board[9][5] = ATTACKER;
    state->board[5][9] = ATTACKER;
    state->board[1][5] = ATTACKER;

    // Set initial game state
    state->attackerTurn = true; // Attackers move first
    state->gameOver = false;
    state->winner = EMPTY;
}

void printBoard(GameState *state) {
    printf("\n    ");
    for (int j = 0; j < BOARD_SIZE; j++) {
        printf(" %c  ", 'a' + j);
    }
    printf("\n");

    printf("   ┌");
    for (int j = 0; j < BOARD_SIZE; j++) {
        printf("───");
        if (j < BOARD_SIZE - 1) printf("┬");
    }
    printf("┐\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d │", i + 1);

        for (int j = 0; j < BOARD_SIZE; j++) {
            char piece = state->board[i][j];

            if (piece == CORNER) {
                printf(" ⊡");
            } else if (piece == THRONE && state->board[i][j] != KING) {
                printf(" ⊕");
            } else if (piece == KING) {
                printf(" ♔");
            } else if (piece == DEFENDER) {
                printf(" ♙");
            } else if (piece == ATTACKER) {
                printf(" ♟");
            } else {
                printf("  ");
            }
            printf(" │");
        }
        printf(" %2d\n", i + 1);

        if (i < BOARD_SIZE - 1) {
            printf("   ├");
            for (int j = 0; j < BOARD_SIZE; j++) {
                printf("───");
                if (j < BOARD_SIZE - 1) printf("┼");
            }
            printf("┤\n");
        }
    }

    printf("   └");
    for (int j = 0; j < BOARD_SIZE; j++) {
        printf("───");
        if (j < BOARD_SIZE - 1) printf("┴");
    }
    printf("┘\n");

    printf("    ");
    for (int j = 0; j < BOARD_SIZE; j++) {
        printf(" %c  ", 'a' + j);
    }
    printf("\n");
}

bool makeMove(GameState *state, int fromRow, int fromCol, int toRow, int toCol) {
    // Check if this is a valid move
    if (!isValidMove(state, fromRow, fromCol, toRow, toCol)) {
        return false;
    }

    // Make the move
    char piece = state->board[fromRow][fromCol];
    state->board[fromRow][fromCol] = (fromRow == BOARD_SIZE/2 && fromCol == BOARD_SIZE/2) ? THRONE : EMPTY;
    state->board[toRow][toCol] = piece;

    // Check for captures
    checkCaptures(state, toRow, toCol);

    return true;
}

bool isValidMove(GameState *state, int fromRow, int fromCol, int toRow, int toCol) {
    // Check if coordinates are within the board
    if (fromRow < 0 || fromRow >= BOARD_SIZE || fromCol < 0 || fromCol >= BOARD_SIZE ||
        toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) {
        return false;
    }

    // Check if there is a piece at the starting position
    char piece = state->board[fromRow][fromCol];
    if (piece == EMPTY || piece == CORNER || piece == THRONE) {
        return false;
    }

    // Check if the piece belongs to the current player
    if ((state->attackerTurn && piece != ATTACKER) ||
        (!state->attackerTurn && piece != DEFENDER && piece != KING)) {
        return false;
    }

    // Check if the destination is empty or is a corner (only the king can go to corners)
    if (state->board[toRow][toCol] != EMPTY &&
        !(piece == KING && state->board[toRow][toCol] == CORNER)) {
        return false;
    }

    // Special rule: only the king can enter the throne and corners
    if (state->board[toRow][toCol] == THRONE && piece != KING) {
        return false;
    }

    // Check if the move is either horizontal or vertical
    if (fromRow != toRow && fromCol != toCol) {
        return false;
    }

    // Check if the path is clear
    if (fromRow == toRow) {
        // Horizontal move
        int start = (fromCol < toCol) ? fromCol + 1 : toCol + 1;
        int end = (fromCol < toCol) ? toCol : fromCol;
        for (int col = start; col < end; col++) {
            if (state->board[fromRow][col] != EMPTY &&
                !(state->board[fromRow][col] == THRONE && state->board[BOARD_SIZE/2][BOARD_SIZE/2] != KING)) {
                return false;
            }
        }
    } else {
        // Vertical move
        int start = (fromRow < toRow) ? fromRow + 1 : toRow + 1;
        int end = (fromRow < toRow) ? toRow : fromRow;
        for (int row = start; row < end; row++) {
            if (state->board[row][fromCol] != EMPTY &&
                !(state->board[row][fromCol] == THRONE && state->board[BOARD_SIZE/2][BOARD_SIZE/2] != KING)) {
                return false;
            }
        }
    }

    return true;
}

void checkCaptures(GameState *state, int row, int col) {
    char currentPlayer = state->attackerTurn ? ATTACKER : DEFENDER;
    char opponent = state->attackerTurn ? DEFENDER : ATTACKER;

    // Check for captures in all four directions
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    for (int dir = 0; dir < 4; dir++) {
        int captureRow = row + dy[dir];
        int captureCol = col + dx[dir];

        // Check if there's an opponent piece to potentially capture
        if (captureRow >= 0 && captureRow < BOARD_SIZE && captureCol >= 0 && captureCol < BOARD_SIZE) {
            char capturePiece = state->board[captureRow][captureCol];

            // Check if it's an opponent piece
            if ((opponent == DEFENDER && (capturePiece == DEFENDER || capturePiece == KING)) ||
                (opponent == ATTACKER && capturePiece == ATTACKER)) {

                // Check if there's a sandwiching piece
                int sandwichRow = captureRow + dy[dir];
                int sandwichCol = captureCol + dx[dir];

                if (sandwichRow >= 0 && sandwichRow < BOARD_SIZE &&
                    sandwichCol >= 0 && sandwichCol < BOARD_SIZE) {

                    // Special case for capturing the king
                    if (capturePiece == KING) {
                        // King requires 4 attackers to capture (or 3 if against the throne)
                        int kingCaptureCount = 0;
                        bool againstThrone = false;

                        for (int kingDir = 0; kingDir < 4; kingDir++) {
                            int adjacentRow = captureRow + dy[kingDir];
                            int adjacentCol = captureCol + dx[kingDir];

                            if (adjacentRow >= 0 && adjacentRow < BOARD_SIZE &&
                                adjacentCol >= 0 && adjacentCol < BOARD_SIZE) {

                                char adjacentPiece = state->board[adjacentRow][adjacentCol];

                                if (adjacentPiece == ATTACKER) {
                                    kingCaptureCount++;
                                } else if (adjacentPiece == THRONE || adjacentPiece == CORNER) {
                                    againstThrone = true;
                                    kingCaptureCount++;
                                }
                            }
                        }

                        // Check if king is captured
                        if ((againstThrone && kingCaptureCount >= 3) || kingCaptureCount >= 4) {
                            state->board[captureRow][captureCol] = EMPTY;
                            state->gameOver = true;
                            state->winner = ATTACKER;
                        }
                    }
                    // Regular piece capture
                    else if (state->board[sandwichRow][sandwichCol] == currentPlayer ||
                             state->board[sandwichRow][sandwichCol] == KING ||
                             state->board[sandwichRow][sandwichCol] == CORNER ||
                             state->board[sandwichRow][sandwichCol] == THRONE) {
                        // Special rule: the throne acts as an attacker for capturing defenders
                        if (!(state->board[sandwichRow][sandwichCol] == THRONE && currentPlayer == DEFENDER)) {
                            state->board[captureRow][captureCol] = EMPTY;
                        }
                    }
                }
            }
        }
    }
}

bool isGameOver(GameState *state) {
    // Check if king escaped (reached a corner)
    bool kingFound = false;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (state->board[i][j] == KING) {
                kingFound = true;

                // Check if king is on a corner
                if ((i == 0 && j == 0) ||
                    (i == 0 && j == BOARD_SIZE-1) ||
                    (i == BOARD_SIZE-1 && j == 0) ||
                    (i == BOARD_SIZE-1 && j == BOARD_SIZE-1)) {
                    state->winner = DEFENDER;
                    return true;
                }
            }
        }
    }

    // Check if king was captured
    if (!kingFound) {
        state->winner = ATTACKER;
        return true;
    }

    // Check if all attackers are captured
    bool attackersFound = false;
    for (int i = 0; i < BOARD_SIZE && !attackersFound; i++) {
        for (int j = 0; j < BOARD_SIZE && !attackersFound; j++) {
            if (state->board[i][j] == ATTACKER) {
                attackersFound = true;
            }
        }
    }

    if (!attackersFound) {
        state->winner = DEFENDER;
        return true;
    }

    // Check if all defenders (except king) are captured and king is surrounded
    bool defendersFound = false;
    for (int i = 0; i < BOARD_SIZE && !defendersFound; i++) {
        for (int j = 0; j < BOARD_SIZE && !defendersFound; j++) {
            if (state->board[i][j] == DEFENDER) {
                defendersFound = true;
            }
        }
    }

    // Game continues
    return false;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHelp() {
    printf("\nHnefatafl Rules:\n");
    printf("- The game is played on an 11x11 board.\n");
    printf("- Attackers (♟) move first and try to capture the King (♔).\n");
    printf("- Defenders (♙) try to help the King escape to one of the four corners (⊡).\n");
    printf("- All pieces move like rooks in chess (horizontally or vertically, any number of squares).\n");
    printf("- Pieces are captured by 'sandwiching' them between two of your pieces.\n");
    printf("- The King needs to be surrounded on all four sides to be captured.\n");
    printf("- The central square (⊕) is the throne and only the King can occupy it.\n\n");
    printf("How to play:\n");
    printf("- Enter moves in the format 'from to' (e.g., 'e1 e5' or 'a3 c3').\n");
    printf("- Type 'help' for these instructions, 'quit' to exit.\n\n");
}

void getPlayerMove(GameState *state, int *fromRow, int *fromCol, int *toRow, int *toCol) {
    char input[20];
    char fromFile, toFile;
    int fromRank, toRank;

    while (1) {
        printf("Enter your move (e.g., 'e1 e5') or 'help' or 'quit': ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            clearScreen();
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = 0;

        // Check for commands
        if (strcmp(input, "help") == 0) {
            clearScreen();
            printHelp();
            printBoard(state);
            continue;
        } else if (strcmp(input, "quit") == 0) {
            printf("Thanks for playing!\n");
            exit(0);
        }

        // Parse move input
        if (sscanf(input, "%c%d %c%d", &fromFile, &fromRank, &toFile, &toRank) == 4) {
            // Convert to 0-indexed coordinates
            *fromCol = tolower(fromFile) - 'a';
            *fromRow = fromRank - 1;
            *toCol = tolower(toFile) - 'a';
            *toRow = toRank - 1;

            // Basic validation of coordinates
            if (*fromCol < 0 || *fromCol >= BOARD_SIZE || *fromRow < 0 || *fromRow >= BOARD_SIZE ||
                *toCol < 0 || *toCol >= BOARD_SIZE || *toRow < 0 || *toRow >= BOARD_SIZE) {
                printf("Invalid coordinates. Please enter coordinates within the board.\n");
                continue;
            }

            break;
        } else {
            printf("Invalid input format. Please use format 'e1 e5' (file-rank file-rank).\n");
        }
    }
}

char* pieceToSymbol(char piece) {
    switch(piece) {
        case KING: return "♔";
        case DEFENDER: return "♙";
        case ATTACKER: return "♟";
        case CORNER: return "⊡";
        case THRONE: return "⊕";
        default: return " ";
    }
}
