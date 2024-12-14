#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 4

// Enum to represent different entities in the world
typedef enum {
    EMPTY,      // .
    WUMPUS,     // W
    PIT,        // P
    GOLD,       // G
    AGENT_START // A
} Cell;

// Enum to represent sensory clues in the world
typedef enum {
    NONE,       // No clue
    BREEZE,     // Adjacent to a pit
    STENCH,     // Adjacent to the Wumpus
    BREEZE_STENCH // Both breeze and stench
} Clue;

// Directions for agent movement
typedef enum {
    UP, DOWN, LEFT, RIGHT
} Direction;

// Structure for the Wumpus World
typedef struct {
    Cell grid[SIZE][SIZE];
    Clue clues[SIZE][SIZE];
    int agent_x, agent_y; // Agent's starting position
    bool visited[SIZE][SIZE]; // Tracks visited cells
} WumpusWorld;

// Check if cell is within bounds
bool is_within_bounds(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

// Function to set clues around Wumpus and pits
void set_clues(WumpusWorld *world) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (world->grid[i][j] == WUMPUS) {
                // Add stench to adjacent cells
                int dx[] = {-1, 1, 0, 0}; // UP, DOWN
                int dy[] = {0, 0, -1, 1}; // LEFT, RIGHT
                for (int k = 0; k < 4; k++) {
                    int nx = i + dx[k], ny = j + dy[k];
                    if (is_within_bounds(nx, ny)) {
                        if (world->clues[nx][ny] == BREEZE) {
                            world->clues[nx][ny] = BREEZE_STENCH;
                        } else {
                            world->clues[nx][ny] = STENCH;
                        }
                    }
                }
            } else if (world->grid[i][j] == PIT) {
                // Add breeze to adjacent cells
                int dx[] = {-1, 1, 0, 0}; // UP, DOWN
                int dy[] = {0, 0, -1, 1}; // LEFT, RIGHT
                for (int k = 0; k < 4; k++) {
                    int nx = i + dx[k], ny = j + dy[k];
                    if (is_within_bounds(nx, ny)) {
                        if (world->clues[nx][ny] == STENCH) {
                            world->clues[nx][ny] = BREEZE_STENCH;
                        } else {
                            world->clues[nx][ny] = BREEZE;
                        }
                    }
                }
            }
        }
    }
}

// Function to initialize the world from a file
void initialize_world(WumpusWorld *world, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    
    char ch;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, " %c", &ch);
            switch (ch) {
                case 'A':
                    world->grid[i][j] = AGENT_START;
                    world->agent_x = i;
                    world->agent_y = j;
                    break;
                case 'W':
                    world->grid[i][j] = WUMPUS;
                    break;
                case 'P':
                    world->grid[i][j] = PIT;
                    break;
                case 'G':
                    world->grid[i][j] = GOLD;
                    break;
                case '.':
                    world->grid[i][j] = EMPTY;
                    break;
                default:
                    printf("Unknown symbol in file: %c\n", ch);
                    exit(EXIT_FAILURE);
            }
            world->visited[i][j] = false;
            world->clues[i][j] = NONE;
        }
    }
    
    fclose(file);
    set_clues(world);
}

// Display the current world with clues
void display_world(const WumpusWorld *world) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == world->agent_x && j == world->agent_y) {
                printf("AG ");
            } else {
                switch (world->grid[i][j]) {
                    case AGENT_START: printf("   "); break;
                    case WUMPUS: printf("WP "); break;
                    case PIT: printf("PT "); break;
                    case GOLD: printf("GL "); break;
                    case EMPTY:
                        switch (world->clues[i][j]) {
                            case BREEZE: printf("BR "); break;
                            case STENCH: printf("ST "); break;
                            case BREEZE_STENCH: printf("SB "); break;
                            default: printf(".. "); break;
                        }
                        break;
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Move agent and find gold recursively
bool find_gold(WumpusWorld *world, int x, int y) {
    // Check if out of bounds or already visited
    if (!is_within_bounds(x, y) || world->visited[x][y]) return false;
    
    // Mark as visited
    world->visited[x][y] = true;
    
    // Display the current position of the agent
    world->agent_x = x;
    world->agent_y = y;
    display_world(world);
    
    // Check if cell contains a pit or Wumpus
    if (world->grid[x][y] == PIT || world->grid[x][y] == WUMPUS) {
        printf("Agent encountered danger! Backtracking...\n");
        return false;
    }
    
    // Check if gold is found
    if (world->grid[x][y] == GOLD) {
        printf("Gold found at (%d, %d)!\n", x, y);
        return true;
    }
    
    // Try moving in each direction
    int dx[] = {-1, 1, 0, 0}; // UP, DOWN
    int dy[] = {0, 0, -1, 1}; // LEFT, RIGHT
    for (int i = 0; i < 4; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        
        if (find_gold(world, new_x, new_y)) {
            return true;
        }
    }
    
    // Backtrack if no path found
    printf("Backtracking from (%d, %d)...\n", x, y);
    return false;
}

// Main function to run the program
int main() {
    WumpusWorld world;

    // Initialize the world from the text file
    initialize_world(&world, "world1.txt");

    // Display the initial world with clues
    printf("Initial Wumpus World with Clues:\n");
    display_world(&world);

    // Start finding the solution
    printf("Starting agent's journey to find gold...\n");
    if (!find_gold(&world, world.agent_x, world.agent_y)) {
        printf("No path to the gold was found.\n");
    }

    return 0;
}
