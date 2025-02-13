#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void lerMatriz(FILE *arquivo, int **matriz, int tamanho);
void multiplicarMatrizes(int **A, int **B, int **C, int tamanho);
void imprimirMatriz(FILE *arquivo, int **matriz, int tamanho);

double getTime() {
    LARGE_INTEGER frequency, start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    return (double)start.QuadPart / frequency.QuadPart;
}

int main(int argc, char *argv[]) {
    FILE *arquivoEntrada = fopen("C:/PENTES/SPD/T1/entrada.txt", "r");
    if (!arquivoEntrada) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    FILE *arquivoSaida = fopen("C:/PENTES/SPD/T1/arquivo_saida.txt", "w");
    if (!arquivoSaida) {
        perror("Erro ao criar o arquivo de saida");
        fclose(arquivoEntrada);
        return 1;
    }

    int tamanho;
    fscanf(arquivoEntrada, "%d", &tamanho);

    int **A = (int **)malloc(tamanho * sizeof(int *));
    int **B = (int **)malloc(tamanho * sizeof(int *));
    int **C = (int **)malloc(tamanho * sizeof(int *));
    for (int i = 0; i < tamanho; i++) {
        A[i] = (int *)malloc(tamanho * sizeof(int));
        B[i] = (int *)malloc(tamanho * sizeof(int));
        C[i] = (int *)malloc(tamanho * sizeof(int));
    }

    lerMatriz(arquivoEntrada, A, tamanho);
    lerMatriz(arquivoEntrada, B, tamanho);
    fclose(arquivoEntrada);

    double start = getTime();
    multiplicarMatrizes(A, B, C, tamanho);
    double end = getTime();

    imprimirMatriz(arquivoSaida, C, tamanho);
    fclose(arquivoSaida);

    for (int i = 0; i < tamanho; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    printf("Tempo de execucao: %.9f segundos\n", end - start);
    return 0;
}

void lerMatriz(FILE *arquivo, int **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo, "%d", &matriz[i][j]);
        }
    }
}

void multiplicarMatrizes(int **A, int **B, int **C, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            C[i][j] = 0;
            for (int k = 0; k < tamanho; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void imprimirMatriz(FILE *arquivo, int **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fprintf(arquivo, "%d ", matriz[i][j]);
        }
        fprintf(arquivo, "\n");
    }
}
