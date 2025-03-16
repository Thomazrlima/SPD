#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

void read_matrix_from_file(const char *filename, double *matrix, int N) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(file, "%lf", &matrix[i * N + j]);
        }
    }
    fclose(file);
}

void write_matrix_to_file(const char *filename, double *matrix, int N) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo %s para escrita\n", filename);
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%.2lf ", matrix[i * N + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset;
    int N;
    double *a, *b, *c;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    if (argc != 4) {
        if (taskid == MASTER) {
            printf("Uso: %s <tamanho> <arquivoA> <arquivoB>\n", argv[0]);
        }
        MPI_Finalize();
        exit(1);
    }

    N = atoi(argv[1]);
    const char *file_a = argv[2];
    const char *file_b = argv[3];

    a = (double *)malloc(N * N * sizeof(double));
    b = (double *)malloc(N * N * sizeof(double));
    c = (double *)malloc(N * N * sizeof(double));

    if (taskid == MASTER) {
        read_matrix_from_file(file_a, a, N);
        read_matrix_from_file(file_b, b, N);

        numworkers = numtasks - 1;
        averow = N / numworkers;
        extra = N % numworkers;
        offset = 0;

        for (dest = 1; dest <= numworkers; dest++) {
            rows = (dest <= extra) ? averow + 1 : averow;
            MPI_Send(&offset, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&a[offset * N], rows * N, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(b, N * N, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
            offset += rows;
        }

        for (source = 1; source <= numworkers; source++) {
            MPI_Recv(&offset, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset * N], rows * N, MPI_DOUBLE, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        }

        write_matrix_to_file("C.txt", c, N);
    } else {
        MPI_Recv(&offset, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&a[offset * N], rows * N, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(b, N * N, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);

        double *c_part = (double *)malloc(rows * N * sizeof(double));
        
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < N; j++) {
                c_part[i * N + j] = 0.0;
                for (int k = 0; k < N; k++) {
                    c_part[i * N + j] += a[(offset + i) * N + k] * b[k * N + j];
                }
            }
        }

        MPI_Send(&offset, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(c_part, rows * N, MPI_DOUBLE, MASTER, FROM_WORKER, MPI_COMM_WORLD);

        free(c_part);
    }

    free(a);
    free(b);
    free(c);
    MPI_Finalize();
    return 0;
}
