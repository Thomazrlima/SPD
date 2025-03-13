#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

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
            fprintf(file, "%6.2f ", matrix[i * N + j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset, i, j, k;
    int N;
    double *a, *b, *c;
    MPI_Status status;

    if (argc != 4) {
        printf("Uso: %s <tamanho da matriz quadrada> <arquivo matriz A> <arquivo matriz B>\n", argv[0]);
        exit(1);
    }

    N = atoi(argv[1]);
    const char *file_a = argv[2];
    const char *file_b = argv[3];

    a = (double *)malloc(N * N * sizeof(double));
    b = (double *)malloc(N * N * sizeof(double));
    c = (double *)malloc(N * N * sizeof(double));

    read_matrix_from_file(file_a, a, N);
    read_matrix_from_file(file_b, b, N);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    if (numtasks < 2) {
        printf("É necessário pelo menos duas tarefas MPI. Finalizando...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    numworkers = numtasks - 1;

    if (taskid == MASTER) {
        for (i = 0; i < N * N; i++) c[i] = 0.0;

        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++) {
            MPI_Send(a, N * N, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
        }

        averow = N / numworkers;
        extra = N % numworkers;
        offset = 0;

        for (dest = 1; dest <= numworkers; dest++) {
            rows = (dest <= extra) ? averow + 1 : averow;
            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&b[0], N * N, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset += rows;
        }

        mtype = FROM_WORKER;
        for (i = 1; i <= numworkers; i++) {
            MPI_Recv(&offset, 1, MPI_INT, i, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, i, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset * N], rows * N, MPI_DOUBLE, i, mtype, MPI_COMM_WORLD, &status);
        }

        write_matrix_to_file("C.txt", c, N);
    }

    if (taskid > MASTER) {
        mtype = FROM_MASTER;
        MPI_Recv(a, N * N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

        double *b_part = (double *)malloc(N * N * sizeof(double));
        MPI_Recv(b_part, N * N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        double *c_part = (double *)malloc(rows * N * sizeof(double));

        for (i = 0; i < rows; i++) {
            for (k = 0; k < N; k++) {
                c_part[i * N + k] = 0.0;
                for (j = 0; j < N; j++) {
                    c_part[i * N + k] += a[(offset + i) * N + j] * b_part[j * N + k];
                }
            }
        }

        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(c_part, rows * N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);

        free(b_part);
        free(c_part);
    }

    free(a);
    free(b);
    free(c);
    MPI_Finalize();
}
