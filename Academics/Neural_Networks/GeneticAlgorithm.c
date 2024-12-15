#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define POP_SIZE 100 // Population size
#define GENES 5      // Number of genes in a chromosome
#define MAX_GEN 5  // Maximum number of generations
#define MUT_RATE 0.01 // Mutation rate

// Chromosome structure
typedef struct {
    int genes[GENES];
    double fitness;
} Chromosome;

// Randomly initialize the genes of a chromosome
void initializeChromosome(Chromosome *chromo) {
    for (int i = 0; i < GENES; i++) {
        chromo->genes[i] = rand() % 2; // Binary genes: 0 or 1
    }
    chromo->fitness = 0.0;
}

// Fitness function: maximize the number of 1s in a chromosome
void evaluateFitness(Chromosome *chromo) {
    chromo->fitness = 0.0;
    for (int i = 0; i < GENES; i++) {
        chromo->fitness += chromo->genes[i];
    }
}

// Perform single-point crossover between two parents
void crossover(Chromosome *parent1, Chromosome *parent2, Chromosome *offspring1, Chromosome *offspring2) {
    int point = rand() % GENES; // Random crossover point
    for (int i = 0; i < GENES; i++) {
        if (i < point) {
            offspring1->genes[i] = parent1->genes[i];
            offspring2->genes[i] = parent2->genes[i];
        } else {
            offspring1->genes[i] = parent2->genes[i];
            offspring2->genes[i] = parent1->genes[i];
        }
    }
}

// Perform mutation on a chromosome
void mutate(Chromosome *chromo) {
    for (int i = 0; i < GENES; i++) {
        if ((double)rand() / RAND_MAX < MUT_RATE) {
            chromo->genes[i] = 1 - chromo->genes[i]; // Flip the gene
        }
    }
}

// Select a parent using roulette wheel selection
Chromosome* selectParent(Chromosome population[]) {
    double totalFitness = 0.0;
    for (int i = 0; i < POP_SIZE; i++) {
        totalFitness += population[i].fitness;
    }

    double threshold = ((double)rand() / RAND_MAX) * totalFitness;
    double sum = 0.0;
    for (int i = 0; i < POP_SIZE; i++) {
        sum += population[i].fitness;
        if (sum >= threshold) {
            return &population[i];
        }
    }
    return &population[POP_SIZE - 1]; // Fallback
}

int main() {
    srand(time(NULL));

    Chromosome population[POP_SIZE];
    Chromosome newPopulation[POP_SIZE];

    // Initialize population
    for (int i = 0; i < POP_SIZE; i++) {
        initializeChromosome(&population[i]);
        evaluateFitness(&population[i]);
    }

    for (int generation = 0; generation < MAX_GEN; generation++) {
        printf("Generation %d:\n", generation);

        // Print population fitness
        for (int i = 0; i < POP_SIZE; i++) {
            printf("\tChromosome %d: Fitness = %.1f\n", i, population[i].fitness);
        }

        // Create next generation
        for (int i = 0; i < POP_SIZE; i += 2) {
            Chromosome *parent1 = selectParent(population);
            Chromosome *parent2 = selectParent(population);

            crossover(parent1, parent2, &newPopulation[i], &newPopulation[i + 1]);

            mutate(&newPopulation[i]);
            mutate(&newPopulation[i + 1]);

            evaluateFitness(&newPopulation[i]);
            evaluateFitness(&newPopulation[i + 1]);
        }

        // Replace old population with new population
        for (int i = 0; i < POP_SIZE; i++) {
            population[i] = newPopulation[i];
        }
    }

    // Find the best solution
    Chromosome best = population[0];
    for (int i = 1; i < POP_SIZE; i++) {
        if (population[i].fitness > best.fitness) {
            best = population[i];
        }
    }

    printf("\nBest solution:\n");
    printf("Genes: ");
    for (int i = 0; i < GENES; i++) {
        printf("%d ", best.genes[i]);
    }
    printf("\nFitness: %.1f\n", best.fitness);

    return 0;
}
