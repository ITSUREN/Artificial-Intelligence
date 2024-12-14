#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
    A,
    B
} Room;

typedef enum {
    DIRTY,
    CLEAN,
    UNKNOWN,
    PROBABLY_DIRTY,
    PROBABLY_CLEAN
} RoomState;

typedef struct {
    Room currentRoom;
    RoomState model[2];       // model[0] = Room A, model[1] = Room B
    int dirtyCount[2];        // Count of times each room was perceived as DIRTY
    int totalObservations[2]; // Total observations for each room
} ModelBasedAgent;

ModelBasedAgent agent = {A, {UNKNOWN, UNKNOWN}, {1, 1}, {1, 1}}; // starting with probabily of 1
RoomState environment[] = {DIRTY, CLEAN};

const char * translator(RoomState states) {
    if (states == DIRTY) return ("DIRTY,");
    else if (states == CLEAN) return ("CLEAN,");
    else if (states == PROBABLY_DIRTY) return ("PROBABLY_DIRTY,");
    else if (states == PROBABLY_CLEAN) return ("PROBABLY_CLEAN,");
    else return("UNKNOWN,");
}

// Print model states
void printModel() {
    printf("\nModel: [");
    for (int i = 0; i < 2; i++) {
        printf("%s",translator(agent.model[i]));
    }
    printf("\b \b] Env: [");
    for (int i = 0; i < 2; i++) {
        printf("%s", translator(environment[i]));
    }
    printf("\b \b] ");
    //printf("dirty: %d, %d total: %d, %d, probab: %f, %f", agent.dirtyCount[0], agent.dirtyCount[1], agent.totalObservations[0], agent.totalObservations[1],(double)agent.dirtyCount[0] / agent.totalObservations[0], (double)agent.dirtyCount[1] / agent.totalObservations[1]);
}
// Perceive the environment
void perceive(Room room) {
    RoomState currentState = environment[room];
    if (currentState == DIRTY) agent.dirtyCount[room]++;
    agent.totalObservations[room]++;
    agent.model[room] = currentState; // Update model with actual percept
}

// Deduce state probabilities and update the model
void deduceState(Room room) {
    for (int i = 0; i < 2; i++) {
        if (i!=room) {
            if (agent.totalObservations[i] > 0) {
                double probabilityDirty = (double)agent.dirtyCount[i] / agent.totalObservations[i];
                if (probabilityDirty > 0.5) {
                    agent.model[i] = PROBABLY_DIRTY;
                } else if (probabilityDirty < 0.35) {
                    agent.model[i] = PROBABLY_CLEAN;
                } else {
                    agent.model[i] = UNKNOWN;
                }
            }
        }
    }
}

// Check if all rooms are clean or probably clean
int allRoomsClean() {
    for (int i = 0; i < 2; i++) {
        if (agent.model[i] == DIRTY || agent.model[i] == PROBABLY_DIRTY || agent.model[i]==UNKNOWN) {
            return 0; // At least one room is dirty
        }
    }
    return 1; // All rooms clean
}

// Act based on the model
void act() {
    if (agent.model[agent.currentRoom] == DIRTY) {
        printf("\nAction: Clean room %c", agent.currentRoom == A ? 'A' : 'B');
        agent.model[agent.currentRoom] = CLEAN;
        environment[agent.currentRoom] = CLEAN;
    } else {
        agent.currentRoom = (agent.currentRoom == A) ? B : A;
        printf("\nAction: Move to room %c", agent.currentRoom == A ? 'A' : 'B');
    }
}

// Simulate environment changes (optional)
void updateEnvironment() {
    // Randomly make rooms dirty again
    if (environment[A] == CLEAN && rand() % 5 == 0) environment[A] = DIRTY;
    if (environment[B] == CLEAN && rand() % 5 == 0) environment[B] = DIRTY;
}

// Main function
int main() {
    system("clear");
    // print the environment state
    printf("Initial state: Room A = %s, Room B = %s", 
        environment[0] == DIRTY ? "DIRTY" : "CLEAN",
        environment[1] == DIRTY ? "DIRTY" : "CLEAN"
    );
    for (int i = 0; i < 10000; i++) {
        updateEnvironment();
        perceive(agent.currentRoom);
        deduceState(agent.currentRoom);
        printModel();
        
        if (allRoomsClean()) {
            printf("\nAction: Stop - All rooms clean\n");
            break;
        }
        
        act();
    }
    return 0;
}
