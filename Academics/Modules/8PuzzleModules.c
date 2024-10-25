#ifndef STRING_H
#define STRING_H
    #include <string.h>
#endif
#ifndef STDIO_H
#define STDIO_H
    #include <stdio.h>
#endif
#ifndef STDBOOL_H
#define STDBOOL_H
    #include <stdbool.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
    #include <stdlib.h>
#endif
#include "./smallComponents.c"

// {FOR DEBUGGING] External declaration of debugFile
// extern FILE *debugFile;

#define N 3
#define TAB 8

typedef struct {
    int row, column;
} Posix;

// Structure to represent the puzzle state
typedef struct {
    int Board[N][N];
    Posix Blank;
} gameState;

bool isSameState(gameState State1, gameState State2) {
    return memcmp(State1.Board, State2.Board, sizeof(State1.Board)) == 0; //inverting output as same=0 from memcmp
}

bool isSafe(int x, int y) {
    return ((x >=0 && x<N) && (y >=0 && y < N));
}

// [EXPERIMENTAL] Hash function to create a unique value for each state 
unsigned long hashState(gameState State) {
    unsigned long hash = 5381; //arbitrary prime number
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            hash = ((hash << 5) + hash) + State.Board[i][j]; // hash * 33 + state.board[i][j] for lesser collisions
        }
    }
    return hash;
}

void inputBoard(gameState *State, char *Index) {
    int blank=0,tableCollision=0, verifiedNumberHashTable[9]={0};
    printf("Enter the Values for State %s:\n", Index);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("\tEnter the value for [%d][%d]: ", i+1, j+1);
            scanf("%d", &(State->Board[i][j]));
            int Locator=State->Board[i][j];
            if ((Locator >= 0) && (Locator <= 8)) {
                if (verifiedNumberHashTable[Locator]==1) {
                    tableCollision=1;
                    system("clear");
                    printf(">>>[ERR 012] Table Collided at %d\n", State->Board[i][j]);
                    inputBoard(State, Index);
                } else {
                    verifiedNumberHashTable[Locator]=1;
                }
            } else {
                system("clear");
                printf(">>>[ERR 011] Value out of range (0-8)\n");
                inputBoard(State, Index);
            }
            if (Locator == 0) {
                printf("\tBlank Noted.\n");
                blank=1;
                State->Blank.row = i; State->Blank.column=j;
            }
        }
    } 
    if (!blank) {
        printf("\n>>>[ERR 023] No Blanks Found... (Denoted by 0 as input)\n");
        inputBoard(State, Index);
    }
}

void printBoard(gameState State, char *Index) {
    printf("\n STATE: %s: \n", Index);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (State.Board[i][j]!=0) {
                printf("\t%d\t|", State.Board[i][j]);
            } else {
                printf("\t \t|");
            }
        }
        printf("\b \b\n");
        if (i != 2 ) {
            for (int k = 0; k < 3*2*TAB; k++) {
                printf("-");
            }
            printf("\n");
        }      
    }
}
