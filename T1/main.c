#include <stdio.h>
#include <stdlib.h>

void lerMatriz(FILE *arquivo, int **matriz, int tamanho);
void multiplicarMatrizes(int **A, int **B, int **C, int tamanho);
void imprimirMatriz(FILE *arquivo, int **matriz, int tamanho);

int main(int argc, char *argv[]) {
    FILE *arquivoEntrada;

    if (argc < 2) {
        arquivoEntrada = fopen("C:/PENTES/SPD/T1/entrada.txt", "r");
        if (!arquivoEntrada) {
            perror("Erro ao abrir o arquivo de entrada");
            return 1;
        }
    }

    FILE *arquivoSaida = fopen("C:/PENTES/SPD/T1/arquivo_saida.txt", "w");
    if (!arquivoSaida) {
        perror("Erro ao criar o arquivo de saída");
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

    multiplicarMatrizes(A, B, C, tamanho);

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
