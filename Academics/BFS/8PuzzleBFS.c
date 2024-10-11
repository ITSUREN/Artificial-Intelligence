#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "../Modules/8PuzzleModules.c"

#define MAXNODES 900
#define HorizontalRule 45

// Forward declaration of the structure
typedef struct queueNode queueNode;

typedef struct queueNode {
    gameState State;
    queueNode *Previous;
    const char *Move; // Track the move made to reach this state
    int Depth;
} queueNode;

int Front = 0, Rear = -1, vIndex = 0;

void printHorizontalRule(){
    printf("\n");
    for (int i=0; i < HorizontalRule; i++) {
        printf("[]");
    };
}

int isQueueEmpty(queueNode Queue[]) {
    if (Front > Rear) {
        Front = 0;Rear = -1;
        printf("\n>>>[ERR 111] Queue Empty");
        return 1;
    }
    return 0;
}

int isQueueFull(queueNode Queue[]) {
    if ((Rear - Front +1) == MAXNODES) {
        printf("\n>>>[ERR 112] Queue Full");
        return 1;
    }
    return 0;
}

int isVisitedFull(queueNode Queue[]) {
    return vIndex==MAXNODES-1;
}

int pushVisited(queueNode Visited[], queueNode visitedNode) {
    if(!isVisitedFull(Visited)) {
        memcpy(&Visited[vIndex], &visitedNode, sizeof(visitedNode));
        ++vIndex;
        return 1;
    } 
    printf("\n>>>[ERR 121] Visited list Full");
    return 0;
}

int enQueue(queueNode Queue[], queueNode Node) {
    if (!isQueueFull(Queue)) {
        ++Rear;
        int Rear_Conv = Rear%MAXNODES;
        memcpy(&Queue[Rear_Conv], &Node, sizeof(Node));
        return 1;
    }
    return 0;
}

int deQueue(queueNode Queue[])  {
    if(!isQueueEmpty(Queue)) {
        ++Front;
        return 1;
    }
    return 0;
}

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
    sprintf(formatString, "Step %d [Move %s]", node->Depth, node->Move);
    printBoard(node->State, formatString); // Pass formatted string to printBoard
}

void printProgressBar(int Value1, int MAX1, int Value2, int MAX2) {
    printf("\r");   // Carriage return to go back to the start of the line
    int width = 20;
    int percentage1 = Value1*100/MAX1,percentage2 = Value2*100/MAX2;
    int pos1 = (percentage1 * width)/100; 
    int pos2 = (percentage2 * width)/100;
    printf("     STATUS: Queue: ["); 
    for (int i = 0; i < width; i++) {
        if (i < pos1)
            printf("#");
        else
            printf(" ");
    }
    printf("] %d", Value1); 
    printf("    Visited: ["); 
    for (int i = 0; i < width; i++) {
        if (i < pos2)
            printf("#");
        else
            printf(" ");
    }
    printf("] %d", Value2); 
    fflush(stdout); // Flush th1e output to ensure it prints immediately
    usleep(10000);
}

void solvePuzzle(gameState Initial, gameState Goal) {
    queueNode Queue[MAXNODES];
    queueNode Visited[MAXNODES]; // array
    queueNode Node;
    queueNode visitedNode;

    // Enqueue Initial State
    Node.State = Initial;
    Node.Previous = NULL;
    Node.Depth = 0;
    Node.Move = "Initial State Inclusion"; // Initial state has no move
    enQueue(Queue, Node);

    // Iteration over Queue
    printf("\n");
    while (!isQueueFull(Queue) && !isVisitedFull(Visited)) { // Added condition to avoid overflow
        printProgressBar(Rear-Front+1, MAXNODES, vIndex, MAXNODES);
        // Dequeue from the Queue
        gameState Current = Queue[Front].State;
        if(!isVisited(Current, Visited)) {
            visitedNode.State = Current;
            visitedNode.Previous = Queue[Front].Previous;
            visitedNode.Depth = Queue[Front].Depth;
            visitedNode.Move = Queue[Front].Move;
            pushVisited(Visited,visitedNode);
        }

        int Depth = Queue[Front].Depth;
        deQueue(Queue);

        // Check if current state is the goal state
        if (isSameState(Current, Goal)) {
            printHorizontalRule();
            printf("\nBacktraced Path:\n");
            backtrace(&Visited[vIndex-1]);
            printf("\nSolution Found at step %d\n\n", Depth);
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
                    Node.State = newState;
                    Node.Depth = Depth + 1;
                    Node.Previous = &Visited[vIndex - 1];
                    Node.Move = moves[i]; // Track the move
                    enQueue(Queue, Node);
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

