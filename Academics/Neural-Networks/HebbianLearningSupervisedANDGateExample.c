#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

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

// Trainng Data
typedef struct {
    int InputVectorSize, OutputSize, DataSize, Epochs;
    float Inputs[MAXSIZE][MAXSIZE];
    float Outputs[MAXSIZE];
} trainingData;

void trainingDataSetter (trainingData *trainingDatas, float Inputs[MAXSIZE][MAXSIZE], float Outputs[MAXSIZE], int InputVectorSize, int DataSize, int Epochs) {
    trainingDatas->InputVectorSize = InputVectorSize;
    trainingDatas->DataSize = DataSize;
    trainingDatas->Epochs = Epochs;
    for (int i = 0; i < DataSize; i++)  {
        for (int j = 0; j < InputVectorSize; j++) {
            trainingDatas->Inputs[i][j] = Inputs[i][j];
        }
        trainingDatas->Outputs[i] = Outputs[i];
    }
}

void trainingDataPrinter (trainingData trainingDatas) {
    printf("\nTRAINING DATA: \n Meta:\n\tInput Vector Size: %d\n\tData Size: %d\n\tEphochs: %d\n\n Training Data: [", trainingDatas.InputVectorSize, trainingDatas.DataSize, trainingDatas.Epochs);
    for (int i = 0; i < trainingDatas.DataSize; i++)  {
        printf("(<");
        for (int j = 0; j < trainingDatas.InputVectorSize; j++) {
            printf("%.2f, ", trainingDatas.Inputs[i][j]);
        }
        printf("\b\b>, %.2f), \b",trainingDatas.Outputs[i]);
    }
    printf("\b]\b\n");
}

// for input layer only
void nodeInputs(layer *inputLayer) {
    printf("\nINPUTS:");
    float value;
    for(int i=0; i<inputLayer->layerSize-1;i++) {
        printf("\n\tEnter the value for the %c Input Node: ", inputLayer->nodes[i].name);
        scanf("%f", &value);
        inputLayer->nodes[i].outValue = inputLayer->nodes[i].netInput = value;
    }
}

void nodeInputsSetter(layer *inputLayer, float inputVector[MAXSIZE]) {
    for(int i=0; i<inputLayer->layerSize;i++) {
        inputLayer->nodes[i].outValue = inputLayer->nodes[i].netInput = inputVector[i];
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

// Set the weights to a fixed weight
void weightSetterLayerWise(network *ANN, int layer, float Weights[MAXSIZE][MAXSIZE]) {
    int rows = ANN->layers[layer-1].layerSize, columns = ANN->layers[layer].layerSize;
    for (int i=0; i < columns; i++) {
        for (int j=0; j < rows; j++) {
            ANN->layers[layer].weights[j][i] = Weights[j][i];
        }
    }
}

void weightPrinterLayerWise(network *ANN, int layer) {
    printf("\nWEIGHTS: [");
    int rows = ANN->layers[layer-1].layerSize, columns = ANN->layers[layer].layerSize;
    for (int i=0; i < columns; i++) {
        for (int j=0; j < rows; j++) {
            printf("%f, ", ANN->layers[layer].weights[j][i]);
        }
    }
    printf("\b\b]\b");
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

void binarysigmoid(layer *currentLayer) {
    // Uses a BinarySigmoid fuction to calculate the output
    for (int node = 0; node < currentLayer->layerSize; node++) {
        float netInputValue = currentLayer->nodes[node].netInput;
        float result = (1/(1+pow(e,-netInputValue)));
        currentLayer->nodes[node].outValue = result;
    }
}

void bipolarsigmoid(layer *currentLayer) {
    // Uses a BipolarSigmoid fuction to calculate the output
    for (int node = 0; node < currentLayer->layerSize; node++) {
        float netInputValue = currentLayer->nodes[node].netInput;
        float result = ((1-pow(e,-netInputValue))/(1+pow(e,-netInputValue)));
        currentLayer->nodes[node].outValue = result;
    }
}

void outValues(layer *currentLayer, int type) {
    switch (type)  {
        case 1:
            binarysigmoid(currentLayer);
            break;
        case 2:
            bipolarsigmoid(currentLayer);
            break;
        default:
            printf("\n[ERR] Activation Function not implemented yet.");
            break;
    }
}

void training(network *ANN, trainingData trainingDatas) {
    float inputVector[MAXSIZE];
    for (int i = 0; i < trainingDatas.Epochs; i++) {
        for (int j = 0; j < trainingDatas.DataSize; j++) {
            // input Vector mapping
            for (int k = 0; k < trainingDatas.InputVectorSize; k++) {
                inputVector[k] = trainingDatas.Inputs[j][k];
            }
            inputVector[trainingDatas.InputVectorSize] = 1; // Bias input
            
            // input Feeding
            nodeInputsSetter(&ANN->layers[0], inputVector);
            nodeOutputsPrinter(ANN->layers[0]);
            netInputs(ANN, 1);
            outValues(&ANN->layers[1], 1);
            
            // Weight update
            float newWeights[MAXSIZE][MAXSIZE];
            for (int node = 0; node < ANN->layers[0].layerSize - 1; node++) {
                newWeights[node][0] = ANN->layers[1].weights[node][0] + ANN->layers[0].nodes[node].outValue * trainingDatas.Outputs[j];
                printf("\nNew weights: %.2f + %.2f * %.2f = %.2f", ANN->layers[1].weights[node][0], ANN->layers[0].nodes[node].outValue, trainingDatas.Outputs[j], newWeights[node][0]);
            }
            newWeights[2][0] = ANN->layers[1].weights[2][0] + trainingDatas.Outputs[j]; //bias update
            printf("\nNew Bias: %.2f + %.2f = %.2f", ANN->layers[1].weights[2][0], trainingDatas.Outputs[j], newWeights[2][0]);
            weightSetterLayerWise(ANN, 1, newWeights);
            weightPrinterLayerWise(ANN, 1);
        }
    }
}


int main() {
    system("clear");
    int activationFunctionType = 2;
    // training Datas
    trainingData trainingDatas;
    int trainingDataSize=4, trainingInputSize=2, epochs=3;
    // AND GATE
    float trainingInput[MAXSIZE][MAXSIZE] = {{1, 1, 1}, {1, -1, 1}, {-1, 1, 1}, {-1, -1, 1}}; // Bias is 1
    float trainingOutput[MAXSIZE] = {1, -1, -1, -1};
    trainingDataSetter(&trainingDatas, trainingInput, trainingOutput, trainingInputSize, trainingDataSize,epochs);
    trainingDataPrinter(trainingDatas);

    // Creating the ANN
    network ANN;
    ANN.networkSize = 2;
    ANN.layers[0].layerSize = trainingInputSize+1; // For bias
    ANN.layers[1].layerSize = 1;
    networkInitializer(&ANN); // Gives names to the layers and nodes.

    // Setting Initial Weight of all to 0
    float InitialWeights[MAXSIZE][MAXSIZE];
    for (int i = 0; i < ANN.layers[0].layerSize; i++) {
        InitialWeights[i][0]=0;
    }
    weightSetterLayerWise(&ANN, 1, InitialWeights);

    // Train the Network
    training(&ANN, trainingDatas);

    // Testing the Model
    char prompt='y';
    while (prompt=='y') {
        system("clear");
        nodeInputs(&ANN.layers[0]); // Prompts user for the inputs
                // Layer 1
        netInputs(&ANN,1);
        outValues(&ANN.layers[1],2);
        nodeOutputsPrinter(ANN.layers[1]);
        // For terminal output's sake.
        printf("\n\n Test the program again? (y/n): "); 
        scanf(" %c", &prompt);
    };

}
