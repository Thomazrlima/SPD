#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void lerMatriz(FILE *arquivo, double **matriz, int tamanho);
void multiplicarMatrizes(double **A, double **B, double **C, int tamanho);
void imprimirMatriz(double **matriz, int tamanho);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <tamanho> <arquivoA> <arquivoB>\n", argv[0]);
        return 1;
    }

    int tamanho = atoi(argv[1]);
    if (tamanho <= 0) {
        printf("Erro: O tamanho da matriz deve ser um número inteiro positivo.\n");
        return 1;
    }

    FILE *arquivoEntradaA = fopen(argv[2], "r");
    if (!arquivoEntradaA) {
        perror("Erro ao abrir o arquivo de entrada A");
        return 1;
    }

    FILE *arquivoEntradaB = fopen(argv[3], "r");
    if (!arquivoEntradaB) {
        perror("Erro ao abrir o arquivo de entrada B");
        fclose(arquivoEntradaA);
        return 1;
    }

    double **A = (double **)malloc(tamanho * sizeof(double *));
    double **B = (double **)malloc(tamanho * sizeof(double *));
    double **C = (double **)malloc(tamanho * sizeof(double *));
    
    for (int i = 0; i < tamanho; i++) {
        A[i] = (double *)malloc(tamanho * sizeof(double));
        B[i] = (double *)malloc(tamanho * sizeof(double));
        C[i] = (double *)malloc(tamanho * sizeof(double));
    }

    lerMatriz(arquivoEntradaA, A, tamanho);
    lerMatriz(arquivoEntradaB, B, tamanho);
    fclose(arquivoEntradaA);
    fclose(arquivoEntradaB);

    multiplicarMatrizes(A, B, C, tamanho);
    imprimirMatriz(C, tamanho);

    for (int i = 0; i < tamanho; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}

void lerMatriz(FILE *arquivo, double **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo, "%lf", &matriz[i][j]);
        }
    }
}

void multiplicarMatrizes(double **A, double **B, double **C, int tamanho) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < tamanho; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void imprimirMatriz(double **matriz, int tamanho) {
    FILE *arquivoSaida = fopen("C.txt", "w");
    if (!arquivoSaida) {
        perror("Erro ao criar o arquivo de saída");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fprintf(arquivoSaida, "%.2lf ", matriz[i][j]);
        }
        fprintf(arquivoSaida, "\n");
    }

    fclose(arquivoSaida);
}