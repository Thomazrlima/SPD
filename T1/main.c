#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void lerMatriz(FILE *arquivo, long long **matriz, int tamanho);
void multiplicarMatrizes(long long **A, long long **B, long long **C, long long **D, int tamanho);
void imprimirMatriz(FILE *arquivo, long long **matriz, int tamanho);

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
        perror("Erro ao criar o arquivo de saída");
        fclose(arquivoEntrada);
        return 1;
    }

    int tamanho;
    fscanf(arquivoEntrada, "%d", &tamanho);

    long long **A = (long long **)malloc(tamanho * sizeof(long long *));
    long long **B = (long long **)malloc(tamanho * sizeof(long long *));
    long long **C = (long long **)malloc(tamanho * sizeof(long long *));
    long long **D = (long long **)malloc(tamanho * sizeof(long long *));

    for (int i = 0; i < tamanho; i++) {
        A[i] = (long long *)malloc(tamanho * sizeof(long long));
        B[i] = (long long *)malloc(tamanho * sizeof(long long));
        C[i] = (long long *)malloc(tamanho * sizeof(long long));
        D[i] = (long long *)malloc(tamanho * sizeof(long long));
    }

    lerMatriz(arquivoEntrada, A, tamanho);
    lerMatriz(arquivoEntrada, B, tamanho);
    lerMatriz(arquivoEntrada, C, tamanho);
    fclose(arquivoEntrada);

    double start = getTime();
    multiplicarMatrizes(A, B, C, D, tamanho);
    double end = getTime();

    imprimirMatriz(arquivoSaida, D, tamanho);
    fclose(arquivoSaida);

    for (int i = 0; i < tamanho; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
        free(D[i]);
    }
    free(A);
    free(B);
    free(C);
    free(D);

    printf("Tempo de execucao: %.9f segundos\n", end - start);

    return 0;
}

void lerMatriz(FILE *arquivo, long long **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo, "%lld", &matriz[i][j]);
        }
    }
}

void multiplicarMatrizes(long long **A, long long **B, long long **C, long long **D, int tamanho) {
    long long **temp = (long long **)malloc(tamanho * sizeof(long long *));
    for (int i = 0; i < tamanho; i++) {
        temp[i] = (long long *)malloc(tamanho * sizeof(long long));
    }

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < tamanho; k++) {
                temp[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            D[i][j] = 0;
            for (int k = 0; k < tamanho; k++) {
                D[i][j] += temp[i][k] * C[k][j];
            }
        }
    }

    for (int i = 0; i < tamanho; i++) {
        free(temp[i]);
    }
    free(temp);
}

void imprimirMatriz(FILE *arquivo, long long **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fprintf(arquivo, "%lld ", matriz[i][j]);
        }
        fprintf(arquivo, "\n");
    }
}
