#include <stdio.h>
#include <unistd.h> // For usleep (optional)

void printProgressBar(int Value1, int MAX1, int Value2, int MAX2) {
    int width = 30;
    int percentage1 = Value1*100/MAX1,percentage2 = Value2*100/MAX2;
    int pos1 = (percentage1 * width)/100; 
    int pos2 = (percentage2 * width)/100;
    printf("STATUS: Queue: ["); 
    for (int i = 0; i < width; i++) {
        if (i < pos1)
            printf("#");
        else
            printf(" ");
    }
    printf("] %d%%", percentage2); 
    printf("    Visisted: ["); 
    for (int i = 0; i < width; i++) {
        if (i < pos2)
            printf("#");
        else
            printf(" ");
    }
    printf("] %d%%", percentage2); 
    fflush(stdout); // Flush th1e output to ensure it prints immediately
    printf("\r");   // Carriage return to go back to the start of the line
}

int main() {
    for (int i = 0; i <= 100; i++) {
        printProgressBar(i,100,i-3,100); // Call the progress bar function
        usleep(100000);        // Sleep for 0.1 second (optional, for demonstration)
    }
    
    printf("\n"); // Move to the next line after the progress completes
    return 0;
}
