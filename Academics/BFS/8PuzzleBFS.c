#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./Modules/8PuzzleModules.c"

#define MAXNODES 1000
#define HorizontalRule 30

typedef struct Node {
    gameState State;
    struct Node *Previous;
    const char *Move; // Track the move made to reach this state
    int Depth;
} queueNode;

int Front = 0, Rear = 0, vIndex = 0;

int isVisited(gameState Current, queueNode Visited[MAXNODES]) {
    for (int i = 0; i < vIndex; i++) {
        if (isSameState(Current, Visited[i].State)) {
            return 1;
        }
    }
    return 0;
}

void backtrace(queueNode *node) {
    if (node == NULL) return;
    backtrace(node->Previous);
    char formatString[50]; // Adjust size as needed
    sprintf(formatString, "Depth %d [Move %s]", node->Depth, node->Move);
    printBoard(node->State, formatString); // Pass formatted string to printBoard
}

void solvePuzzle(gameState Initial, gameState Goal) {
    queueNode Queue[MAXNODES];
    queueNode Visited[MAXNODES]; // array

    // Enqueue Initial State
    Queue[Rear].State = Initial;
    Queue[Rear].Previous = NULL;
    Queue[Rear].Depth = 0;
    Queue[Rear].Move = "Initial State Inclusion"; // Initial state has no move
    Rear++;

    // Iteration over Queue
    while (Front < Rear && Rear < MAXNODES) { // Added condition to avoid overflow
        // Dequeue from the Queue
        gameState Current = Queue[Front].State;
        Visited[vIndex].State = Current;
        Visited[vIndex].Previous = Queue[Front].Previous;
        Visited[vIndex].Depth = Queue[Front].Depth;
        Visited[vIndex].Move = Queue[Front].Move;

        int Depth = Queue[Front].Depth;
        Front++;
        vIndex++; // Dequeue operation

        // Check if current state is the goal state
        if (isSameState(Current, Goal)) {
            printf("\nBacktrace Path:\n");
            backtrace(&Visited[vIndex - 1]);
            printf("\nSolution Found at depth %d\n\n", Depth);
            return;
        }

        // Find blank position
        int x = Current.Blank.x, y = Current.Blank.y;

        // Possible Movements: left, right, up, down
        int dy[] = {-1, 1, 0, 0};
        int dx[] = {0, 0, -1, 1};
        const char *moves[] = {"Left", "Right", "Up", "Down"};

        for (int i = 0; i < 4; i++) {
            int newx = x + dx[i], newy = y + dy[i];

            if (isSafe(newx, newy)) {
                // Create a new State by moving blanks
                gameState newState = Current;
                Swap(&newState.Board[x][y], &newState.Board[newx][newy]);
                newState.Blank.x = newx;
                newState.Blank.y = newy;

                // Check if the state has been visited
                if (!isVisited(newState, Visited)) {
                    Queue[Rear].State = newState;
                    Queue[Rear].Depth = Depth + 1;
                    Queue[Rear].Previous = &Visited[vIndex - 1];
                    Queue[Rear].Move = moves[i]; // Track the move
                    Rear++;
                }
            }
        }
    }
    // No solution found within limits
    printf("\nSolution Not Found within Limits.\n\n");
}

int main() {
    system("clear");
    gameState Initial, Goal;
    inputBoard(&Initial, "Initial");
    system("clear");
    printBoard(Initial, "Initial");
    inputBoard(&Goal, "Goal");
    system("clear");
    printBoard(Initial, "Initial");
    printBoard(Goal, "Goal");
    for (int i=0; i < HorizontalRule; i++) {
        printf("[]");
    };
    solvePuzzle(Initial, Goal);
    return 0;
}
