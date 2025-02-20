#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 2048

int main() {
    int **matriz = (int **)malloc(SIZE * sizeof(int *));
    for (int i = 0; i < SIZE; i++) {
        matriz[i] = (int *)malloc(SIZE * sizeof(int));
    }
    srand(time(NULL));

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matriz[i][j] = rand() % 1001;
        }
    }

    FILE *file = fopen("B.txt", "w");
    if (file == NULL) {
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", matriz[i][j]);
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i < SIZE; i++) {
        free(matriz[i]);
    }
    free(matriz);

    fclose(file);
    return 0;
}
