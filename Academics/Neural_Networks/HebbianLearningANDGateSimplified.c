#include <stdio.h>

#define NUM_INPUTS 2
#define NUM_PATTERNS 4

// AND gate truth table in bipolar form
int inputs[NUM_PATTERNS][NUM_INPUTS] = {
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1}
};

int targets[NUM_PATTERNS] = {-1, -1, -1, 1}; // AND gate outputs

void trainHebbian(float weights[NUM_INPUTS], float *bias) {
    // Initialize weights and bias to zero
    for (int i = 0; i < NUM_INPUTS; i++) {
        weights[i] = 0.0;
    }
    *bias = 0.0;

    printf("Training Hebbian Network...\n");

    // Hebbian learning rule
    for (int i = 0; i < NUM_PATTERNS; i++) {
        printf("Input: [%d, %d], Target: %d\n", inputs[i][0], inputs[i][1], targets[i]);
        for (int j = 0; j < NUM_INPUTS; j++) {
            weights[j] += inputs[i][j] * targets[i];
        }
        *bias += targets[i];
        printf("Updated Weights: [%.1f, %.1f], Bias: %.1f\n", weights[0], weights[1], *bias);
    }
    printf("\nTraining Complete!\n");
}

void testHebbian(float weights[NUM_INPUTS], float bias) {
    printf("\nTesting Hebbian Network:\n");
    for (int i = 0; i < NUM_PATTERNS; i++) {
        // Calculate the weighted sum
        float weightedSum = bias;
        for (int j = 0; j < NUM_INPUTS; j++) {
            weightedSum += inputs[i][j] * weights[j];
        }

        // Output is 1 if weighted sum > 0, otherwise 0
        int output = (weightedSum > 0) ? 1 : -1;
        printf("Input: [%d, %d], Predicted: %d\n", inputs[i][0], inputs[i][1], output);
    }
}

int main() {
    float weights[NUM_INPUTS];
    float bias;

    trainHebbian(weights, &bias);
    testHebbian(weights, bias);

    return 0;
}
