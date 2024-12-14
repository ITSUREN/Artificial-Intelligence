#include <stdio.h>

#define GRID_SIZE 3
#define COST_DOWN 2 // This makes the agent less likely to move down
#define COST_RIGHT 1

typedef struct {
    int x, y;  // Current position
    int totalCost; // Total cost of the path
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

// Move the agent toward the goal with minimal cost
void moveUtilityBased(Agent *agent) {
    if (agent->y < 2 && isValidMove(agent->x, agent->y + 1) && COST_RIGHT <= COST_DOWN) {
        agent->y++; // Move right
        agent->totalCost += COST_RIGHT;
        printf("Agent moved right to (%d, %d), Cost: %d\n", agent->x, agent->y, agent->totalCost);
    } else if (agent->x < 2 && isValidMove(agent->x + 1, agent->y)) {
        agent->x++; // Move down
        agent->totalCost += COST_DOWN;
        printf("Agent moved down to (%d, %d), Cost: %d\n", agent->x, agent->y, agent->totalCost);
    }
}

// Main function
int main() {
    Agent agent = {0, 0, 0}; // Start at (0, 0) with 0 cost

    printf("Utility-Based Agent:\n");
    while (grid[agent.x][agent.y] != 'G') {
        moveUtilityBased(&agent);
    }
    printf("Goal reached at (%d, %d) with Total Cost: %d!\n", agent.x, agent.y, agent.totalCost);
    return 0;
}
