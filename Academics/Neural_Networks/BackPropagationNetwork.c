#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_INPUTS 10
#define MAX_HIDDEN 10
#define MAX_OUTPUTS 10
#define LEARNING_RATE 0.5
#define EPOCHS 10000
#define THRESHOLD 0.01

// Sigmoid activation function and its derivative
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoidDerivative(double x) {
    return x * (1.0 - x);
}

// Neural Network Structure
typedef struct {
    int numInputs;
    int numHidden;
    int numOutputs;

    double inputLayer[MAX_INPUTS];
    double hiddenLayer[MAX_HIDDEN];
    double outputLayer[MAX_OUTPUTS];

    double weightsInputHidden[MAX_INPUTS][MAX_HIDDEN];
    double weightsHiddenOutput[MAX_HIDDEN][MAX_OUTPUTS];

    double biasHidden[MAX_HIDDEN];
    double biasOutput[MAX_OUTPUTS];
} NeuralNetwork;

// Initialize the neural network with random weights and biases
void initializeNetwork(NeuralNetwork *nn, int numInputs, int numHidden, int numOutputs) {
    nn->numInputs = numInputs;
    nn->numHidden = numHidden;
    nn->numOutputs = numOutputs;

    srand(time(NULL));

    for (int i = 0; i < numInputs; i++) {
        for (int j = 0; j < numHidden; j++) {
            nn->weightsInputHidden[i][j] = ((double)rand() / RAND_MAX) * 2 - 1; // Random values between -1 and 1
        }
    }

    for (int i = 0; i < numHidden; i++) {
        for (int j = 0; j < numOutputs; j++) {
            nn->weightsHiddenOutput[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }

    for (int i = 0; i < numHidden; i++) {
        nn->biasHidden[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }

    for (int i = 0; i < numOutputs; i++) {
        nn->biasOutput[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

// Forward pass
void forwardPass(NeuralNetwork *nn, double inputs[]) {
    // Input to Hidden Layer
    for (int i = 0; i < nn->numHidden; i++) {
        double activation = nn->biasHidden[i];
        for (int j = 0; j < nn->numInputs; j++) {
            activation += inputs[j] * nn->weightsInputHidden[j][i];
        }
        nn->hiddenLayer[i] = sigmoid(activation);
    }

    // Hidden to Output Layer
    for (int i = 0; i < nn->numOutputs; i++) {
        double activation = nn->biasOutput[i];
        for (int j = 0; j < nn->numHidden; j++) {
            activation += nn->hiddenLayer[j] * nn->weightsHiddenOutput[j][i];
        }
        nn->outputLayer[i] = sigmoid(activation);
    }
}

// Backward pass
void backwardPass(NeuralNetwork *nn, double inputs[], double targets[]) {
    double outputErrors[MAX_OUTPUTS];
    double hiddenErrors[MAX_HIDDEN];

    // Calculate output layer errors
    for (int i = 0; i < nn->numOutputs; i++) {
        outputErrors[i] = (targets[i] - nn->outputLayer[i]) * sigmoidDerivative(nn->outputLayer[i]);
    }

    // Calculate hidden layer errors
    for (int i = 0; i < nn->numHidden; i++) {
        hiddenErrors[i] = 0.0;
        for (int j = 0; j < nn->numOutputs; j++) {
            hiddenErrors[i] += outputErrors[j] * nn->weightsHiddenOutput[i][j];
        }
        hiddenErrors[i] *= sigmoidDerivative(nn->hiddenLayer[i]);
    }

    // Update weights from Hidden to Output Layer
    for (int i = 0; i < nn->numHidden; i++) {
        for (int j = 0; j < nn->numOutputs; j++) {
            nn->weightsHiddenOutput[i][j] += LEARNING_RATE * outputErrors[j] * nn->hiddenLayer[i];
        }
    }

    // Update biases for Output Layer
    for (int i = 0; i < nn->numOutputs; i++) {
        nn->biasOutput[i] += LEARNING_RATE * outputErrors[i];
    }

    // Update weights from Input to Hidden Layer
    for (int i = 0; i < nn->numInputs; i++) {
        for (int j = 0; j < nn->numHidden; j++) {
            nn->weightsInputHidden[i][j] += LEARNING_RATE * hiddenErrors[j] * inputs[i];
        }
    }

    // Update biases for Hidden Layer
    for (int i = 0; i < nn->numHidden; i++) {
        nn->biasHidden[i] += LEARNING_RATE * hiddenErrors[i];
    }
}

// Train the neural network
void train(NeuralNetwork *nn, double inputs[][MAX_INPUTS], double targets[][MAX_OUTPUTS], int numSamples) {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double totalError = 0.0;

        for (int sample = 0; sample < numSamples; sample++) {
            forwardPass(nn, inputs[sample]);
            backwardPass(nn, inputs[sample], targets[sample]);

            // Calculate total error
            for (int i = 0; i < nn->numOutputs; i++) {
                double error = targets[sample][i] - nn->outputLayer[i];
                totalError += error * error;
            }
        }

        totalError /= numSamples;

        if (epoch % 1000 == 0) {
            printf("Epoch %d, Error: %.6f\n", epoch, totalError);
        }

        if (totalError < THRESHOLD) {
            printf("Training complete at epoch %d, Error: %.6f\n", epoch, totalError);
            break;
        }
    }
}

// Test the neural network
void test(NeuralNetwork *nn, double inputs[][MAX_INPUTS], double targets[][MAX_OUTPUTS], int numSamples) {
    printf("\nTesting Neural Network:\n");
    for (int sample = 0; sample < numSamples; sample++) {
        forwardPass(nn, inputs[sample]);
        printf("Input: [");
        for (int i = 0; i < nn->numInputs; i++) {
            printf("%.1f ", inputs[sample][i]);
        }
        printf("] Output: [");
        for (int i = 0; i < nn->numOutputs; i++) {
            printf("%.3f ", nn->outputLayer[i]);
        }
        printf("] Target: [");
        for (int i = 0; i < nn->numOutputs; i++) {
            printf("%.1f ", targets[sample][i]);
        }
        printf("]\n");
    }
}

int main() {
    NeuralNetwork nn;
    const int numberOfSamples=4;
    initializeNetwork(&nn, 2, 2, 1); // 2 inputs, 2 hidden neurons, 1 output

    // Training data for XOR problem
    double inputs[numberOfSamples][MAX_INPUTS] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double targets[numberOfSamples][MAX_OUTPUTS] = {{0}, {1}, {1}, {0}};

    printf("Training Neural Network...\n");
    train(&nn, inputs, targets, numberOfSamples);

    test(&nn, inputs, targets, numberOfSamples);

    return 0;
}
