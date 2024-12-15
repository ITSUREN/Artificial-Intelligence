#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 6
#define INF INT_MAX

typedef struct {
    int cost[MAX_NODES][MAX_NODES]; // Adjacency matrix for path costs
    int heuristic[MAX_NODES];      // Heuristic values
    char nodes[MAX_NODES];         // Node labels
} Graph;

typedef struct {
    int node;
    int fCost; // f(n) = g(n) + h(n)
    int gCost; // g(n)
} PriorityQueue;

PriorityQueue pq[MAX_NODES];
int pqSize = 0;

// Function prototypes
void initGraph(Graph *graph);
void addEdge(Graph *graph, char from, char to, int cost);
int getNodeIndex(Graph *graph, char node);
void push(int node, int fCost, int gCost);
PriorityQueue pop();
int isEmpty();
void aStarSearch(Graph *graph, char start, char goal);

int main() {
    Graph graph;
    initGraph(&graph);

    // Add edges
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
    graph.heuristic[getNodeIndex(&graph, 'D')] = 6;
    graph.heuristic[getNodeIndex(&graph, 'E')] = 7;
    graph.heuristic[getNodeIndex(&graph, 'G')] = 0;

    printf("\nA* Search Path:\n");
    aStarSearch(&graph, 'S', 'G');

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
    graph->nodes[5] = 'G';
}

void addEdge(Graph *graph, char from, char to, int cost) {
    int fromIndex = getNodeIndex(graph, from);
    int toIndex = getNodeIndex(graph, to);
    graph->cost[fromIndex][toIndex] = cost;
    graph->cost[toIndex][fromIndex] = cost; // Undirected graph
}

int getNodeIndex(Graph *graph, char node) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[i] == node) {
            return i;
        }
    }
    return -1; // Node not found
}

void push(int node, int fCost, int gCost) {
    pq[pqSize].node = node;
    pq[pqSize].fCost = fCost;
    pq[pqSize].gCost = gCost;
    pqSize++;
}

PriorityQueue pop() {
    int minIndex = 0;
    for (int i = 1; i < pqSize; i++) {
        if (pq[i].fCost < pq[minIndex].fCost || 
           (pq[i].fCost == pq[minIndex].fCost && pq[i].gCost < pq[minIndex].gCost)) {
            minIndex = i;
        }
    }
    PriorityQueue result = pq[minIndex];
    for (int i = minIndex; i < pqSize - 1; i++) {
        pq[i] = pq[i + 1];
    }
    pqSize--;
    return result;
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

void aStarSearch(Graph *graph, char start, char goal) {
    int startIndex = getNodeIndex(graph, start);
    int goalIndex = getNodeIndex(graph, goal);

    int visited[MAX_NODES] = {0};
    int gCosts[MAX_NODES];
    int cameFrom[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        gCosts[i] = INF;
        cameFrom[i] = -1;
    }
    gCosts[startIndex] = 0;

    push(startIndex, graph->heuristic[startIndex], 0);

    while (!isEmpty()) {
        PriorityQueue current = pop();
        int currentNode = current.node;

        if (currentNode == goalIndex) {
            reconstructPath(cameFrom, currentNode, graph);
            printf("\nGoal Reached!\n");
            return;
        }

        if (visited[currentNode]) continue;

        visited[currentNode] = 1;

        for (int i = 0; i < MAX_NODES; i++) {
            if (graph->cost[currentNode][i] != INF && !visited[i]) {
                int tentativeGCost = gCosts[currentNode] + graph->cost[currentNode][i];
                if (tentativeGCost < gCosts[i]) {
                    gCosts[i] = tentativeGCost;
                    cameFrom[i] = currentNode;
                    int fCost = gCosts[i] + graph->heuristic[i];
                    push(i, fCost, gCosts[i]);
                }
            }
        }
    }

    printf("\nGoal not reachable!\n");
}
