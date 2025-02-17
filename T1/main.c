#include <stdio.h>
#include <stdlib.h>

void lerMatriz(FILE *arquivo, long long **matriz, int tamanho);
void multiplicarMatrizes(long long **A, long long **B, long long **C, int tamanho);
void imprimirMatriz(FILE *arquivo, long long **matriz, int tamanho);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <arquivoA> <arquivoB> <arquivoSaida>\n", argv[0]);
        return 1;
    }

    FILE *arquivoEntradaA = fopen(argv[1], "r");
    if (!arquivoEntradaA) {
        perror("Erro ao abrir o arquivo de entrada A");
        return 1;
    }

    FILE *arquivoEntradaB = fopen(argv[2], "r");
    if (!arquivoEntradaB) {
        perror("Erro ao abrir o arquivo de entrada B");
        fclose(arquivoEntradaA);
        return 1;
    }

    FILE *C = fopen(argv[3], "w");
    if (!C) {
        perror("Erro ao criar o arquivo de saída");
        fclose(arquivoEntradaA);
        fclose(arquivoEntradaB);
        return 1;
    }

    int tamanho;
    fscanf(arquivoEntradaA, "%d", &tamanho);

    long long **A = (long long **)malloc(tamanho * sizeof(long long *));
    long long **B = (long long **)malloc(tamanho * sizeof(long long *));
    long long **matrizC = (long long **)malloc(tamanho * sizeof(long long *));
    
    for (int i = 0; i < tamanho; i++) {
        A[i] = (long long *)malloc(tamanho * sizeof(long long));
        B[i] = (long long *)malloc(tamanho * sizeof(long long));
        matrizC[i] = (long long *)malloc(tamanho * sizeof(long long));
    }

    lerMatriz(arquivoEntradaA, A, tamanho);
    lerMatriz(arquivoEntradaB, B, tamanho);
    fclose(arquivoEntradaA);
    fclose(arquivoEntradaB);

    multiplicarMatrizes(A, B, matrizC, tamanho);
    imprimirMatriz(C, matrizC, tamanho);
    fclose(C);

    for (int i = 0; i < tamanho; i++) {
        free(A[i]);
        free(B[i]);
        free(matrizC[i]);
    }
    free(A);
    free(B);
    free(matrizC);

    return 0;
}

void lerMatriz(FILE *arquivo, long long **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo, "%lld", &matriz[i][j]);
        }
    }
}

void multiplicarMatrizes(long long **A, long long **B, long long **C, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            C[i][j] = 0;
            for (int k = 0; k < tamanho; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void imprimirMatriz(FILE *arquivo, long long **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fprintf(arquivo, "%lld ", matriz[i][j]);
        }
        fprintf(arquivo, "\n");
    }
}
