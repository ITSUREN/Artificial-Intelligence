#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 6
#define INF INT_MAX

// Define structure for the graph
typedef struct {
    int cost[MAX_NODES][MAX_NODES]; // Adjacency matrix for path costs
    int heuristic[MAX_NODES];      // Heuristic values for each node
    char nodes[MAX_NODES];         // Node labels
} Graph;

// Priority queue for Greedy Best-First Search
typedef struct {
    int node;
    int priority; // heuristic value
} PriorityQueue;

PriorityQueue pq[MAX_NODES];
int pqSize = 0;

// Function prototypes
void initGraph(Graph *graph);
void addEdge(Graph *graph, char from, char to, int cost);
int getNodeIndex(Graph *graph, char node);
void push(int node, int priority);
int pop();
int isEmpty();
void reconstructPath(int cameFrom[], int current, Graph *graph);
void greedyBestFirstSearch(Graph *graph, char start, char goal);

int main() {
    Graph graph;
    initGraph(&graph);

    // Add edges and path costs
    addEdge(&graph, 'S', 'B', 9);
    addEdge(&graph, 'S', 'D', 8);
    addEdge(&graph, 'B', 'E', 8);
    addEdge(&graph, 'B', 'C', 7);
    addEdge(&graph, 'C', 'G', 6);
    addEdge(&graph, 'C', 'D', 5);
    addEdge(&graph, 'D', 'G', 10);
    addEdge(&graph, 'E', 'G', 9);

    // Set heuristic values
    graph.heuristic[getNodeIndex(&graph, 'S')] = 12;
    graph.heuristic[getNodeIndex(&graph, 'B')] = 4;
    graph.heuristic[getNodeIndex(&graph, 'C')] = 3;
    graph.heuristic[getNodeIndex(&graph, 'D')] = 5;
    graph.heuristic[getNodeIndex(&graph, 'E')] = 7;
    graph.heuristic[getNodeIndex(&graph, 'G')] = 0; // Goal heuristic is always 0

    // Perform Greedy Best-First Search
    printf("\nGreedy Best-First Search Path:\n");
    greedyBestFirstSearch(&graph, 'S', 'G');

    return 0;
}

void initGraph(Graph *graph) {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph->cost[i][j] = INF;
        }
    }
    graph->nodes[0] = 'S';
    graph->nodes[1] = 'B';
    graph->nodes[2] = 'C';
    graph->nodes[3] = 'D';
    graph->nodes[4] = 'E';
    graph->nodes[5] = 'G'; // Include the goal node
}

void addEdge(Graph *graph, char from, char to, int cost) {
    int fromIndex = getNodeIndex(graph, from);
    int toIndex = getNodeIndex(graph, to);
    if (fromIndex == -1 || toIndex == -1) {
        printf("Error: Invalid edge from '%c' to '%c'\n", from, to);
        exit(1);
    }
    graph->cost[fromIndex][toIndex] = cost;
    graph->cost[toIndex][fromIndex] = cost; // Undirected graph
}

int getNodeIndex(Graph *graph, char node) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[i] == node) {
            return i;
        }
    }
    printf("Error: Node '%c' not found in graph.\n", node);
    exit(1);
}

void push(int node, int priority) {
    pq[pqSize].node = node;
    pq[pqSize].priority = priority;
    pqSize++;
}

int pop() {
    int minIndex = 0;
    for (int i = 1; i < pqSize; i++) {
        if (pq[i].priority < pq[minIndex].priority) {
            minIndex = i;
        }
    }
    int node = pq[minIndex].node;
    // Remove the element
    for (int i = minIndex; i < pqSize - 1; i++) {
        pq[i] = pq[i + 1];
    }
    pqSize--;
    return node;
}

int isEmpty() {
    return pqSize == 0;
}

void reconstructPath(int cameFrom[], int current, Graph *graph) {
    if (cameFrom[current] != -1) {
        reconstructPath(cameFrom, cameFrom[current], graph);
    }
    printf("%c ", graph->nodes[current]);
}

void greedyBestFirstSearch(Graph *graph, char start, char goal) {
    int startIndex = getNodeIndex(graph, start);
    int goalIndex = getNodeIndex(graph, goal);

    int visited[MAX_NODES] = {0};
    int cameFrom[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        cameFrom[i] = -1;
    }

    push(startIndex, graph->heuristic[startIndex]);

    while (!isEmpty()) {
        int current = pop();

        if (visited[current]) continue;

        visited[current] = 1;

        if (current == goalIndex) {
            reconstructPath(cameFrom, current, graph);
            printf("\nGoal Reached!\n");
            return;
        }

        // Explore neighbors
        for (int i = 0; i < MAX_NODES; i++) {
            if (graph->cost[current][i] != INF && !visited[i]) {
                push(i, graph->heuristic[i]);
                cameFrom[i] = current;
            }
        }
    }

    printf("\nGoal not reachable!\n");
}
