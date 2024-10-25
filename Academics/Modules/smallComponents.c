// Function to generate progress bar
void printProgressBar(int Value1, int MAX1, int Value2, int MAX2, int period) {
    printf("\r");   // Carriage return to go back to the start of the line
    int width = 20;
    int percentage1 = Value1*100/MAX1,percentage2 = Value2*100/MAX2;
    int pos1 = (percentage1 * width)/100; 
    int pos2 = (percentage2 * width)/100;
    printf("     STATUS: Stack: ["); 
    for (int i = 0; i < width; i++) {
        if (i < pos1)
            printf("#");
        else
            printf(" ");
    }
    printf("] %d", Value1); 
    printf("    Visited: ["); 
    for (int i = 0; i < width; i++) {
        if (i < pos2)
            printf("#");
        else
            printf(" ");
    }
    printf("] %d", Value2); 
    fflush(stdout); // Flush th1e output to ensure it prints immediately
    usleep(period);
}

// Function to swap two integers
void Swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}