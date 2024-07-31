#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXSIZE 30
#define e 2.7182818284590452353602874713527

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
    char layerName[50];
    float weights[MAXSIZE][MAXSIZE];
} layer;

typedef struct {
    layer layers[MAXSIZE];
    int networkSize;
} network;

// all will be added as class methods
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
    static int counter= 0 +'A';
    for (int i = 0; i < ANN->networkSize; i++) {
        if (i == 0 ) {
            strcpy(ANN->layers[i].layerName, "Input Layer");
        } else if (i == ANN->networkSize-1) {
            strcpy(ANN->layers[i].layerName, "Output Layer");
        } else {
            strcpy(ANN->layers[i].layerName, "Hidden Layer");
        }
        for (int j = 0; j < ANN->layers[i].layerSize; j++) {
            ANN->layers[i].nodes[j].name=(char)counter;
            ANN->layers[i].nodes[j].netInput = 0;
            ANN->layers[i].nodes[j].outValue = 0;
            //printf("\n\tSetting the name for layer %d node %d as %c", i, j, ANN->layers[i].nodes[j].name);
            counter++;
        }
    }
}

//whole network imported because it uses previous layer too 
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

void nodeOutputsPrinter(layer currentLayer) {
    printf("\n%s OUTPUTS:", currentLayer.layerName);
    for(int i=0; i<currentLayer.layerSize;i++) {
        printf("\n\t For %c, netInputs: %.2f and outValue=%.2f.", currentLayer.nodes[i].name, currentLayer.nodes[i].netInput, currentLayer.nodes[i].outValue);
    }
}

void netInputs(network *ANN, int layer) {
    //printf("\nNETINPUTS: ");
    for (int node=0; node<ANN->layers[layer].layerSize; node++) {
        //printf("\n\tPerforming: ");
        float result = 0;
        for (int i=0; i< ANN->layers[layer-1].layerSize; i++) {
            //printf("%f *  %f + ", ANN->layers[layer-1].nodes[i].outValue, ANN->layers[layer].weights[i][node]);
            result += ANN->layers[layer-1].nodes[i].outValue * ANN->layers[layer].weights[i][node];
        }
        printf("\b\b \b\n");
        ANN->layers[layer].nodes[node].netInput = result;
    }
}

void sigmoid(layer *currentLayer) {
    //printf("\nSIGMOID: ");
    for (int node = 0; node < currentLayer->layerSize; node++) {
        float netInputValue = currentLayer->nodes[node].netInput;
        float result = (1/(1+pow(e,-netInputValue)));
        currentLayer->nodes[node].outValue = result;
        //printf("\n\tHere net input value taken is %f and the result is %f",netInputValue, result);
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
    int activationFuntionType = 1;

    network ANN;
    ANN.networkSize = 3;
    ANN.layers[0].layerSize = 3;
    ANN.layers[1].layerSize = 3;
    ANN.layers[2].layerSize = 1;
    networkInitializer(&ANN);
    nodeinputs(&ANN.layers[0]);
    nodeOutputsPrinter(ANN.layers[0]);
    weightInputsLayerWise(&ANN, 1);
    netInputs(&ANN,1);
    outValues(&ANN.layers[1],1);
    nodeOutputsPrinter(ANN.layers[1]);
    weightInputsLayerWise(&ANN, 2);
    netInputs(&ANN,2);
    outValues(&ANN.layers[2],1);
    nodeOutputsPrinter(ANN.layers[2]);

}
