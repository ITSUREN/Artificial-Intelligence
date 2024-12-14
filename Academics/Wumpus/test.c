#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

// Structure for the Wumpus World
typedef struct {
    Cell grid[SIZE][SIZE];
    Clue clues[SIZE][SIZE];
    int agent_x, agent_y; // Agent's starting position
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
                int dx[] = {-1, 1, 0, 0};
                int dy[] = {0, 0, -1, 1};
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
                printf("A ");
            } else {
                switch (world->grid[i][j]) {
                    case AGENT_START: printf("A "); break;
                    case WUMPUS: printf("W "); break;
                    case PIT: printf("P "); break;
                    case GOLD: printf("G "); break;
                    case EMPTY:
                        switch (world->clues[i][j]) {
                            case BREEZE: printf("B "); break;
                            case STENCH: printf("S "); break;
                            case BREEZE_STENCH: printf("SB "); break;
                            default: printf(". "); break;
                        }
                        break;
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Simple Reflex Agent's decision-making function
bool move_agent(WumpusWorld *world) {
    int x = world->agent_x;
    int y = world->agent_y;

    // Check percepts in the current cell
    if (world->clues[x][y] == BREEZE || world->clues[x][y] == STENCH || world->clues[x][y] == BREEZE_STENCH) {
        printf("Percept detected at (%d, %d) - halting due to danger perception.\n", x, y);
        return false;
    }

    // Check if the current cell contains gold
    if (world->grid[x][y] == GOLD) {
        printf("Gold found at (%d, %d)!\n", x, y);
        return true;
    }

    // Randomly explore an adjacent cell
    int dx[] = {-1, 1, 0, 0}; // UP, DOWN
    int dy[] = {0, 0, -1, 1}; // LEFT, RIGHT
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (is_within_bounds(nx, ny) && world->clues[nx][ny] == NONE) {
            world->agent_x = nx;
            world->agent_y = ny;
            printf("Moving to (%d, %d)\n", nx, ny);
            display_world(world);
            return false;
        }
    }

    // If no safe move is available, stop
    printf("No safe moves available at (%d, %d) - exploration ended.\n", x, y);
    return false;
}

// Main function to run the program
int main() {
    srand(time(NULL));
    WumpusWorld world;

    // Initialize the world from the text file
    initialize_world(&world, "world2.txt");

    // Display the initial world with clues
    printf("Initial Wumpus World with Clues:\n");
    display_world(&world);

    // Start agent's exploration
    printf("Agent begins exploring...\n");
    while (!move_agent(&world));

    return 0;
}
