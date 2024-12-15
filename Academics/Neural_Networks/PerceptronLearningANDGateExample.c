#include <stdio.h>
#include <stdlib.h>

#define LEARNING_RATE 0.1
#define EPOCHS 10

// Activation function: Step function
int stepFunction(float value) {
    return value >= 0 ? 1 : 0;
}

// Train perceptron for the AND gate
void trainPerceptron() {
    // AND gate truth table
    int inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    int outputs[4] = {0, 0, 0, 1};

    float weights[2] = {0.0, 0.0}; // Initialize weights to 0
    float bias = 0.0;              // Initialize bias to 0

    printf("Training Perceptron for AND Gate...\n");

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        printf("\nEpoch %d:\n", epoch + 1);
        for (int i = 0; i < 4; i++) {
            // Calculate weighted sum
            float weightedSum = inputs[i][0] * weights[0] +
                                inputs[i][1] * weights[1] +
                                bias;

            // Apply activation function
            int predicted = stepFunction(weightedSum);

            // Calculate error
            int error = outputs[i] - predicted;

            // Update weights and bias
            weights[0] += LEARNING_RATE * error * inputs[i][0];
            weights[1] += LEARNING_RATE * error * inputs[i][1];
            bias += LEARNING_RATE * error;

            printf("Input: [%d, %d], Target: %d, Predicted: %d, Error: %d\n",
                   inputs[i][0], inputs[i][1], outputs[i], predicted, error);
            printf("Updated Weights: [%.2f, %.2f], Bias: %.2f\n",
                   weights[0], weights[1], bias);
        }
    }

    printf("\nTraining Complete!\n");
    printf("Final Weights: [%.2f, %.2f], Bias: %.2f\n", weights[0], weights[1], bias);

    // Automated Testing of  perceptron
    printf("\nTesting Perceptron:\n");
    for (int i = 0; i < 4; i++) {
        float weightedSum = inputs[i][0] * weights[0] +
                            inputs[i][1] * weights[1] +
                            bias;
        int predicted = stepFunction(weightedSum);
        printf("Input: [%d, %d], Predicted: %d\n", inputs[i][0], inputs[i][1], predicted);
    }
}

int main() {
    trainPerceptron();
    return 0;
}
