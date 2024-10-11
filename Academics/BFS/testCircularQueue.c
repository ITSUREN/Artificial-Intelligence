#include <stdio.h>
#include <stdlib.h>
#define MAXNODES 4


typedef struct{
    int value;
} queueNode;
 

int Front = 0, Rear = -1;

int isEmpty(queueNode Queue[]) {
    if (Front > Rear) {
        Front = 0;Rear = -1;
        printf("\n>>> Queue Empty");
        return 1;
    }
    return 0;
}

int isFull(queueNode Queue[]) {
    if ((Rear - Front +1) == MAXNODES) {
        printf("\n>>> Queue Full");
        return 1;
    }
    return 0;
}

int enQueue(queueNode Queue[], int value) {
    if (!isFull(Queue)) {
        ++Rear;
        int Front_conv = Front%MAXNODES, Rear_Conv = Rear%MAXNODES;
        Queue[Rear_Conv].value=value;
        return 1;
    }
    return 0;
}

int deQueue(queueNode Queue[])  {
    if(!isEmpty(Queue)) {
        ++Front;
        int Front_conv = Front%MAXNODES, Rear_Conv = Rear%MAXNODES;
        return 1;
    }
    return 0;
}

int menu(queueNode Queue[],int *choice) {
    int value;
    system("clear");
    printf("\nSTAT: %d/%d", Rear -Front+1, MAXNODES);
    printf("\nMENU:\n\t1. enQueue\n\t2. deQueue\n\t3. Exit\n => ");
    scanf("%d", choice);
    switch(*choice) {
        case 1: 
            printf("\nEnter the Value to enqueue: ");
            scanf("%d", &value);
            enQueue(Queue,value);
            break;
        case 2:
            deQueue(Queue);
    }
}
int main() {
    system("clear");
    queueNode Queue[MAXNODES];
    int choice=0;
    do {
        menu(Queue,&choice);
    } while (choice!=3);

}