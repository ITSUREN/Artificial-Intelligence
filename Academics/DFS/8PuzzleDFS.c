#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "../Modules/8PuzzleModules.c"

#define MAXVISITEDSIZE 10000 
#define MAXDEPTH 20 // Size of Stack
#define HorizontalRule 45

// Forward declaration of the structure
typedef struct stackNode stackNode;

typedef struct stackNode {
    gameState State;
    stackNode *Previous;
    const char *Move; // Track the move made to reach this state
    int Depth;
} stackNode;

int Rear = -1, vIndex = 0;

void printHorizontalRule(){
    printf("\n");
    for (int i=0; i < HorizontalRule; i++) {
        printf("[]");
    };
}

int isStackEmpty(stackNode Stack[]) {
    return Rear==-1;
}

int isStackFull(stackNode Stack[]) {
    return Rear==MAXDEPTH-1;
}

int isVisitedFull(stackNode Stack[]) {
    return vIndex==MAXVISITEDSIZE-1;
}

int pushVisited(stackNode Visited[], stackNode visitedNode) {
    if(!isVisitedFull(Visited)) {
        memcpy(&Visited[vIndex], &visitedNode, sizeof(visitedNode));
        ++vIndex;
        return 1;
    } 
    printf("\n>>>[ERR 121] Visited list Full");
    return 0;
}

int pushStack(stackNode Stack[], stackNode Node) {
    if (!isStackFull(Stack)) {
        ++Rear;
        memcpy(&Stack[Rear], &Node, sizeof(Node));
        return 1;
    }
    return 0;
}

stackNode popStack(stackNode Stack[])  {
    if(!isStackEmpty(Stack)) {
        --Rear;
        return Stack[Rear+1];
    }
}

int isVisited(gameState currentState, stackNode Visited[MAXVISITEDSIZE]) {
    for (int i = 0; i < vIndex; i++) {
        if (isSameState(currentState, Visited[i].State)) {
            return i+1;
        }
    }
    return 0;
}

void backtrace(stackNode *node) {
    if (node == NULL) return;
    backtrace(node->Previous);
    char formatString[50]; // Adjust size as needed
    sprintf(formatString, "Step %d [Move %s]", node->Depth, node->Move);
    printBoard(node->State, formatString); // Pass formatted string to printBoard
}


void solvePuzzle(gameState Initial, gameState Goal) {
    stackNode Stack[MAXDEPTH];
    stackNode Visited[MAXVISITEDSIZE]; // array
    stackNode Node;

    // EnStack Initial State
    Node.State = Initial;
    Node.Previous = NULL;
    Node.Depth = 0;
    Node.Move = "Initial State Inclusion"; // Initial state has no move
    pushStack(Stack, Node);

    // Iteration over Stack
    printf("\n");
    while (!isStackFull(Stack) && !isVisitedFull(Visited) && !isStackEmpty(Stack)) { // Added condition to avoid overflow
        printProgressBar(Rear+1, MAXDEPTH, vIndex, MAXVISITEDSIZE,10000);
        // Pop from the Stack
        Node = popStack(Stack);
        int holdParentDepth = Node.Depth;
        gameState currentState = Node.State;

        // Push to Visited
        if(!isVisited(currentState,Visited)) {
            pushVisited(Visited,Node);
        }

        // Check if current state is the goal state
        if (isSameState(currentState, Goal)) {
            printHorizontalRule();
            printf("\nBacktraced Path:\n");
            backtrace(&Node);
            printf("\nSolution Found at step %d\n\n", Node.Depth);
            return;
        }

        // Find blank position
        int x = currentState.Blank.row, y = currentState.Blank.column;

        // Possible Movements: left, right, up, down
        int dy[] = {-1, 1, 0, 0};
        int dx[] = {0, 0, -1, 1};
        const char *moves[] = {"Left", "Right", "Up", "Down"};

        for (int i = 0; i < 4; i++) {
            int newx = x + dx[i], newy = y + dy[i];

            if (isSafe(newx, newy) && (Node.Depth!=MAXDEPTH)) {
                // Create a new State by moving blanks
                gameState newState = currentState;
                Swap(&newState.Board[x][y], &newState.Board[newx][newy]);
                newState.Blank.row = newx;
                newState.Blank.column = newy;

                // Check if the state has been visited
                if (!isVisited(newState, Visited)) {
                    Node.State = newState;
                    Node.Depth = holdParentDepth+1;
                    Node.Previous = &Visited[vIndex-1];
                    Node.Move = moves[i]; // Track the move
                    pushStack(Stack, Node);
                }
            }
        }
    }
    // No solution found within limits
    printf("\n>>>[ERR 1XX] Solution Not Found within Step Limits.\n\n");
}

int main() {
    system("clear");
    gameState Initial, Goal;

    // Inputs
    inputBoard(&Initial, "Initial");
    system("clear");
    printBoard(Initial, "Initial");
    inputBoard(&Goal, "Goal");
    system("clear");

    // Print Initial and Goal
    printBoard(Initial, "Initial");
    printBoard(Goal, "Goal");

    //Solve
    printHorizontalRule();
    
    solvePuzzle(Initial, Goal);
    return 0;
}