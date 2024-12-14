#include <stdio.h>
#include <string.h>

typedef enum {
    A,
    B
} Room;

typedef enum {
    DIRTY,
    CLEAN,
} RoomState;

typedef struct {
    Room currentRoom;
    RoomState currentState;
} SimpleReflexAgent;

void act(SimpleReflexAgent *agent, RoomState *environment) {
    if (environment[agent->currentRoom] == DIRTY) {
        printf("Room %c: Action - clean\n", agent->currentRoom==A?'A':'B');
        environment[agent->currentRoom] = CLEAN;
    } else {
        agent->currentRoom = (agent->currentRoom == A) ? B : A;
        printf("Room %c: Action - move\n", agent->currentRoom==A?'A':'B');
    }
}

int main() {
    SimpleReflexAgent agent; 
    agent.currentRoom=A;
    RoomState environment[] = {DIRTY, DIRTY}; // D: dirty, C: clean for rooms A and B

    for (int i = 0; i < 4; i++) {
        act(&agent, environment);
    }
    return 0;
}
