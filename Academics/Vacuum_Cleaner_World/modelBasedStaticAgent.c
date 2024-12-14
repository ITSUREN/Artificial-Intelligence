#include <stdio.h>
#include <string.h>

typedef enum {
    A,
    B
} Room;

typedef enum {
    DIRTY,
    CLEAN,
    UNKNOWN
} RoomState;

typedef struct {
    Room currentRoom;
    RoomState model[2]; // model[0] = Room A, model[1] = Room B
} ModelBasedAgent;

ModelBasedAgent agent = {A, {UNKNOWN, UNKNOWN}};
RoomState environment[] = {DIRTY, CLEAN};

void modelInstancePrinter() {
    printf("\nModel: [");
    for (int i = 0; i < 2; i++) {
        if (agent.model[i] == DIRTY) printf("DIRTY,");
        else if (agent.model[i] == CLEAN) printf("CLEAN,");
        else printf("UNKN,");
    }
    printf("\b \b]");
}

void perceive(Room room) {
    agent.model[room] = environment[room];
}

void act() { //condition action rules + What action I shoudl do now.
    if (agent.model[agent.currentRoom] == DIRTY) {
        printf("\nRoom %c: Action - clean", agent.currentRoom == A ? 'A' : 'B');
        agent.model[agent.currentRoom] = CLEAN; // Update model
        environment[agent.currentRoom] = CLEAN; // Update environment
    } else {
        agent.currentRoom = (agent.currentRoom == A) ? B : A;
        printf("\nRoom %c: Action - move", agent.currentRoom == A ? 'A' : 'B');
    }
}

int main() {
    // print the environment state
    printf("Initial state: Room A = %s, Room B = %s", 
        environment[0] == DIRTY ? "DIRTY" : "CLEAN",
        environment[1] == DIRTY ? "DIRTY" : "CLEAN"
    );
    //print initial model 
    modelInstancePrinter();
    for (int i = 0; i < 4; i++) {
        perceive(agent.currentRoom);
        modelInstancePrinter();
        act();
    }
    printf("\nAgent simulation complete.\n");
    return 0;
}
