#include <stdio.h>

#define GRID_SIZE 3

typedef struct {
    int x, y; // Current position
} Agent;

// The grid world
char grid[GRID_SIZE][GRID_SIZE] = {
    {'S', '_', '_'},
    {'_', 'X', '_'},
    {'_', '_', 'G'}
};

// Function to check if a position is valid and not an obstacle
int isValidMove(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && grid[x][y] != 'X';
}

// Move the agent toward the goal
void moveGoalBased(Agent *agent) {
    if (agent->x < 2 && isValidMove(agent->x + 1, agent->y)) {
        agent->x++; // Move down
    } else if (agent->y < 2 && isValidMove(agent->x, agent->y + 1)) {
        agent->y++; // Move right
    }
    printf("Agent moved to (%d, %d)\n", agent->x, agent->y);
}

// Main function
int main() {
    Agent agent = {0, 0}; // Start at (0, 0)

    printf("Goal-Based Agent:\n");
    while (grid[agent.x][agent.y] != 'G') {
        moveGoalBased(&agent);
    }
    printf("Goal reached at (%d, %d)!\n", agent.x, agent.y);
    return 0;
}
