#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BLOCKS 4
#define STACKS 4

typedef enum {
    A, B, C, D
} blocks;

// Structure to represent a state in the block world
typedef struct {
    blocks stacks[STACKS][BLOCKS]; // Stacks holding blocks
    int heights[STACKS];           // Instantaneous Heights of each stack 
} State;

char translator(blocks block) {
    return 'A'+block;
}

int calculateHeuristic(State *current, blocks *goal) {
    int misplaced = 0;
    for (int i = 0; i < STACKS; i++) {
        for (int j = 1; j < current->heights[i]; j++) {
            if (current->stacks[i][j] != goal[j]) {
                misplaced=misplaced-j;
            } else {
                misplaced = misplaced +j;
            }
        }
    }
    return misplaced;
}

bool areStatesEqual(State *state1, State *state2) {
    for (int i = 0; i < STACKS; i++) {
        if (state1->heights[i] != state2->heights[i]) { //checks height
            return false;
        }
        for (int j = 0; j < state1->heights[i]; j++) { //checks blocks
            if (state1->stacks[i][j] != state2->stacks[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Function to check if two states are equal
bool goalTest(State *current, blocks *goal) {
    int correct=0;
    for (int i = 0; i < STACKS; i++) {
        for (int j = 1; j < current->heights[i]; j++) {
            if (current->stacks[i][j] == goal[j]) {
                correct++;
            }
        }
    }
    if (correct==BLOCKS-1) {
        return true;
    } else {
        return false;
    }
}

// Function to print a state
void printState(State *Current) {
    for (int i = 0; i < STACKS; i++) {
        printf("Stack %d: ", i);
        for (int j = 0; j < Current->heights[i]; j++) {
            printf("%c ", translator(Current->stacks[i][j]));
        }
        printf("\n");
    }
}

int getFreeStack(State *Current) {
    for (int i = 0; i < STACKS; i++) {
        if (Current->heights[i]==0) {
            return i;
        }
    }
    return -1;
}

// Function to generate a neighbor state by moving a block
bool generateNeighbor(State *current, State *neighbor, int fromStack, int toStack) {
    if (current->heights[fromStack] == 0 || fromStack == toStack) { // When stack is empty or stack is deleting and appending to itself
        return false; // Invalid move
    }

    // Copy the current state to the neighbor
    memcpy(neighbor, current, sizeof(State));

    // Perform the move
    int block = neighbor->stacks[fromStack][neighbor->heights[fromStack] - 1];
    neighbor->heights[fromStack]--;
    neighbor->stacks[toStack][neighbor->heights[toStack]] = block;
    neighbor->heights[toStack]++;

    if (areStatesEqual(current,neighbor)) {
        return false;
    } else {
        return true;
    }
}

// Hill Climbing algorithm to solve the block world problem
void hillClimbing(State *start, blocks *goal) {
    State current = *start;
    int currentHeuristic = calculateHeuristic(&current, goal);

    printf("\nInitial State (Heuristic: %d):\n", currentHeuristic);
    printState(&current);

    while (true) {
        State bestNeighbor;
        int bestHeuristic = currentHeuristic;
        //printf("\n Best Heuristic: %d\n", bestHeuristic);
        bool foundBetter = false;

        // Generate neighbors by moving blocks between stacks
        //printf("\n NEIGHBOURS:");
        for (int i = 0; i < STACKS; i++) {
            for (int j = 0; j < STACKS; j++) {
                State neighbor;
                if (generateNeighbor(&current, &neighbor, i, j)) {
                    int heuristic = calculateHeuristic(&neighbor, goal);
                    //printf("\n");
                    //printState(&neighbor);
                    //printf("\n With Heuristic: %d best heuristics: %d bool: %d", heuristic, bestHeuristic,heuristic>bestHeuristic);
                    if (heuristic > bestHeuristic) {
                        bestNeighbor = neighbor;
                        bestHeuristic = heuristic;
                        foundBetter = true;
                        //printf("\n Passed %d",foundBetter);
                    }
                }
            }
        }
        printf("\n------------------------------------------------------\n");
        // Check if the goal is reached
        if (goalTest(&current, goal)) {
            printf("Goal state reached!\n");
            break;
        }

        if (!foundBetter) {
            // No better neighbor found, stop the search
            printf("Reached local maximum or goal state.\n");
            break;
        }

        // Move to the best neighbor
        current = bestNeighbor;
        currentHeuristic = bestHeuristic;

        printf("Current State (Heuristic: %d):\n", currentHeuristic);
        printState(&current);
    }
}

int main() {
    State start = {
        .stacks = {{B, C, D, A}, {}, {},{}},
        .heights = {4, 0, 0, 0}
    };

    blocks goal[BLOCKS]= {A, B, C, D};

    hillClimbing(&start, goal);

    return 0;
}