#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXSIZE 30
#define e 2.718281

//will be converted to classes after the basic is complete with structures and functions
// for a single node
typedef struct {
    char name;
    float outValue;
    float netInput;
} node; 

typedef struct {
    node nodes[MAXSIZE];
    int layerSize;
} layer;

typedef struct {
    layer layers[MAXSIZE];
    int networkSize;
} network;

//to mulltiply intputs with weights to get net Inputs for the whole layer, needs to be integrated, just an example
void matrixMultiply(float inputs[], float weights[][MAXSIZE], float netInputs[], int sizeA, int sizeB) {
    // Initialize the output matrix
    for (int i = 0; i < sizeB; i++) {
        netInputs[i] = 0;
        for (int j = 0; j < sizeA; j++) {
            netInputs[i] += inputs[j] * weights[j][i];
        }
    }
}

// all will be added as class methods
// for input layer only
void nodeinputs(layer *inputLayer) {
    printf("\nINPUTS:");
    for(int i=0; i<inputLayer->layerSize;i++) {
        printf("\n\tEnter the value for the %c Input Node: ", inputLayer->nodes[i].name);
        scanf("%f", &(inputLayer->nodes[i].netInput));
    }
}

void networkInitializer(network *ANN) {
    static int counter= 0 +'A';
    for (int i = 0; i < ANN->networkSize; i++) {
        for (int j = 0; j < ANN->layers[i].layerSize; j++) {
            ANN->layers[i].nodes[j].name=(char)counter;
            ANN->layers[i].nodes[j].netInput = 0;
            ANN->layers[i].nodes[j].outValue = 0;
            printf("\nSetting the name for layer %d node %d with value %c as %c", i, j, counter, ANN->layers[i].nodes[j].name);
            counter++;
        }
    }
}

void weightInputs(network ANN, int layer) {
    int rows = ANN.layers[layer-1].layerSize, columns = ANN.layers[layer].layerSize;
    float weights[rows][columns];
    for (int i=0; i < rows; i++) {
        for (int j=0; j < columns; j++) {
            printf("Enter the weight for %c and %c", ANN.layers[layer-1].nodes[i].name, ANN.layers[layer].nodes[j].name);
            scanf("%f", &weights[i][j]);
        }
    }
}

void nodeoutputs(layer currentLayer) {
    printf("\nOUTPUTS:");
    for(int i=0; i<currentLayer.layerSize;i++) {
        printf("\n\t For %c, netInputs: %.2f and outValue=%.2f.", currentLayer.nodes[i].name, currentLayer.nodes[i].netInput, currentLayer.nodes[i].outValue);
    }
}

float netInputs(float inputs[], float weights[], int size) {
    float result = 0;
    printf("Performing: ");
    for (int i=0; i< size; i++) {
        printf("%f *  %f + ", inputs[i], weights[i]);
       result += inputs[i] * weights[i];
    }
    return result;
}

float outValues(float netInputValue, int type) {
    switch (type)  {
        case 1:
            //binary sigmoid function
            return (1/(1+pow(e,-netInputValue)));
            break;
        default:
            printf("\n[ERR] Activation Function not implemented yet.");
            break;
    }
    return 0;
}

int main() {
    system("clear");
    int activationFuntionType = 1;

    network ANN;
    ANN.networkSize = 3;
    ANN.layers[0].layerSize = 3;
    ANN.layers[1].layerSize = 3;
    ANN.layers[2].layerSize = 1;
    networkInitializer(&ANN);
    nodeinputs(&ANN.layers[0]);
    nodeoutputs(ANN.layers[0]);

}
