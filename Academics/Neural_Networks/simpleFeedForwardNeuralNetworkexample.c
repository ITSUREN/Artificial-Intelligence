#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXSIZE 30
#define e 2.7182818284590452353602874713527

// for a single node
typedef struct {
    char name;
    float outValue;
    float netInput;
} node; 

// A collection of nodes with weight matrix, layerSize represents the number of nodes
typedef struct {
    node nodes[MAXSIZE];
    int layerSize;
    char layerName[50];
    float weights[MAXSIZE][MAXSIZE];
} layer;

// A collection of layers, networkSize represents the number of layers
typedef struct {
    layer layers[MAXSIZE];
    int networkSize;
} network;

// for input layer only
void nodeinputs(layer *inputLayer) {
    printf("\nINPUTS:");
    float value;
    for(int i=0; i<inputLayer->layerSize;i++) {
        printf("\n\tEnter the value for the %c Input Node: ", inputLayer->nodes[i].name);
        scanf("%f", &value);
        inputLayer->nodes[i].outValue = inputLayer->nodes[i].netInput = value;
    }
}

void networkInitializer(network *ANN) {
    //printf("\n NETWORK INITIALIZER");
    static int nodeCounter= 0 +'A';
    static int hiddenLayerCounter = 0;
    for (int i = 0; i < ANN->networkSize; i++) {
        if (i == 0 ) {
            strcpy(ANN->layers[i].layerName, "Input Layer");
        } else if (i == ANN->networkSize-1) {
            strcpy(ANN->layers[i].layerName, "Output Layer");
        } else {
            sprintf(ANN->layers[i].layerName, "Hidden Layer %d", ++hiddenLayerCounter);
        }
        for (int j = 0; j < ANN->layers[i].layerSize; j++) {
            ANN->layers[i].nodes[j].name=(char)nodeCounter++;
            ANN->layers[i].nodes[j].netInput = 0;
            ANN->layers[i].nodes[j].outValue = 0;
        }
    }
}

// whole network referenced because it uses previous layer too (multiple layer). User Input type
void weightInputsLayerWise(network *ANN, int layer) {
    printf("\n WEIGHTS: ");
    int rows = ANN->layers[layer-1].layerSize, columns = ANN->layers[layer].layerSize;
    for (int i=0; i < columns; i++) {
        for (int j=0; j < rows; j++) {
            printf("\n\tEnter the weight for %c and %c: ", ANN->layers[layer-1].nodes[j].name, ANN->layers[layer].nodes[i].name);
            scanf("%f", &ANN->layers[layer].weights[j][i]);
        }
    }
}

// Set the weights to a fixed weight
void weightSetterLayerWise(network *ANN, int layer, float Weights[MAXSIZE][MAXSIZE]) {
    int rows = ANN->layers[layer-1].layerSize, columns = ANN->layers[layer].layerSize;
    for (int i=0; i < columns; i++) {
        for (int j=0; j < rows; j++) {
            ANN->layers[layer].weights[j][i] = Weights[j][i];
        }
    }
}

void nodeOutputsPrinter(layer currentLayer) {
    printf("\n%s OUTPUTS:", currentLayer.layerName);
    for(int i=0; i<currentLayer.layerSize;i++) {
        printf("\n\t For %c, netInputs: %.2f and outValue=%.2f.", currentLayer.nodes[i].name, currentLayer.nodes[i].netInput, currentLayer.nodes[i].outValue);
    }
}

void netInputs(network *ANN, int layer) {
    // Get the weighted fum of inputs without the bias
    for (int node=0; node<ANN->layers[layer].layerSize; node++) {
        float result = 0;
        for (int i=0; i< ANN->layers[layer-1].layerSize; i++) {
            result += ANN->layers[layer-1].nodes[i].outValue * ANN->layers[layer].weights[i][node];
        }
        ANN->layers[layer].nodes[node].netInput = result;
    }
}

void sigmoid(layer *currentLayer) {
    // Uses a Sigmoid fuction to calculate the output
    for (int node = 0; node < currentLayer->layerSize; node++) {
        float netInputValue = currentLayer->nodes[node].netInput;
        float result = (1/(1+pow(e,-netInputValue)));
        currentLayer->nodes[node].outValue = result;
    }
}

void outValues(layer *currentLayer, int type) {
    switch (type)  {
        case 1:
            sigmoid(currentLayer);
            break;
        default:
            printf("\n[ERR] Activation Function not implemented yet.");
            break;
    }
}

int main() {
    system("clear");
    int activationFuntionType = 1; // Selecting Binary Sigmoid function
    // Fixing the Weights
    float layer1Weights[MAXSIZE][MAXSIZE] = {
        {0.5, 0.2, 0.5}, 
        {0.1, 0.4, 0.1}, 
        {0.3, 0.6, 0.8}},
    layer2Weights[MAXSIZE][MAXSIZE] = {
        {0.3}, 
        {0.1}, 
        {0.7}}; 
    // Creating the ANN
    network ANN;
    ANN.networkSize = 3;
    ANN.layers[0].layerSize = 3;
    ANN.layers[1].layerSize = 3;
    ANN.layers[2].layerSize = 1;
    networkInitializer(&ANN); // Gives names to the layers and nodes.
    weightSetterLayerWise(&ANN, 1, layer1Weights);
    weightSetterLayerWise(&ANN, 2, layer2Weights);
    // Getting the input
    nodeinputs(&ANN.layers[0]); // Prompts user for the inputs
    // Solution Procedure
    nodeOutputsPrinter(ANN.layers[0]);
        // Layer 1
    netInputs(&ANN,1);
    outValues(&ANN.layers[1],1);
    nodeOutputsPrinter(ANN.layers[1]);
        // Layer 2
    netInputs(&ANN,2);
    outValues(&ANN.layers[2],1);
    nodeOutputsPrinter(ANN.layers[2]);
    // For terminal output's sake.
    printf("\n");
}
