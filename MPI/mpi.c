#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0               /* ID da tarefa mestre */
#define FROM_MASTER 1          /* tipo de mensagem */
#define FROM_WORKER 2          /* tipo de mensagem */

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
    int numtasks,              /* número de tarefas na partição */
        taskid,                /* identificador da tarefa */
        numworkers,            /* número de tarefas trabalhadoras */
        source,                /* ID da tarefa de origem */
        dest,                  /* ID da tarefa de destino */
        mtype,                 /* tipo de mensagem */
        rows,                  /* número de linhas da matriz A enviadas a cada trabalhador */
        averow, extra, offset, /* usados para determinar as linhas enviadas a cada trabalhador */
        i, j, k, rc;           /* misc */
    
    int N;                     /* Tamanho da matriz quadrada */
    double *a, *b, *c;         /* Matrizes A, B e C */
    MPI_Status status;

    /* Verificando os argumentos */
    if (argc != 4) {
        printf("Uso: %s <tamanho da matriz quadrada> <arquivo matriz A> <arquivo matriz B>\n", argv[0]);
        exit(1);
    }

    N = atoi(argv[1]);  /* Lê o tamanho da matriz quadrada */
    const char *file_a = argv[2];  /* Arquivo para matriz A */
    const char *file_b = argv[3];  /* Arquivo para matriz B */

    /* Aloca as matrizes dinamicamente */
    a = (double *)malloc(N * N * sizeof(double));
    b = (double *)malloc(N * N * sizeof(double));
    c = (double *)malloc(N * N * sizeof(double));

    /* Lê as matrizes dos arquivos */
    read_matrix_from_file(file_a, a, N);
    read_matrix_from_file(file_b, b, N);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    if (numtasks < 2) {
        printf("É necessário pelo menos duas tarefas MPI. Finalizando...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }

    numworkers = numtasks - 1;

    /**************************** tarefa mestre ************************************/
    if (taskid == MASTER) {
        printf("Iniciando multiplicação de matrizes com %d tarefas.\n", numtasks);

        /* Inicializa a matriz C com zeros */
        for (i = 0; i < N; i++) 
            for (j = 0; j < N; j++) 
                c[i * N + j] = 0.0;

        /* Enviar a matriz A para todos os trabalhadores */
        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++) {
            MPI_Send(a, N * N, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);  // Envia toda a matriz A
        }

        /* Distribuir a matriz B para os trabalhadores, uma parte de B por vez */
        averow = N / numworkers;
        extra = N % numworkers;
        offset = 0;

        for (dest = 1; dest <= numworkers; dest++) {
            rows = (dest <= extra) ? averow + 1 : averow; 

            /* Envia a parte correspondente de B */
            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&b[offset * N], rows * N, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);

            offset = offset + rows;
        }

        /* Receber os resultados das tarefas trabalhadoras */
        mtype = FROM_WORKER;
        for (i = 1; i <= numworkers; i++) {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset * N], rows * N, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
            printf("Resultados recebidos da tarefa %d\n", source);
        }

        /* Salvar o resultado em um arquivo C.txt */
        write_matrix_to_file("C.txt", c, N);
        printf("Resultado salvo em C.txt.\n");
    }

    /**************************** tarefa trabalhadora ************************************/
    if (taskid > MASTER) {
        mtype = FROM_MASTER;
        
        /* Receber a matriz A */
        MPI_Recv(a, N * N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        /* Receber a parte correspondente da matriz B */
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(b, rows * N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        /* Realizar a multiplicação de matrizes */
        for (k = 0; k < N; k++) 
        for (i = 0; i < rows; i++) {
            c[i * N + k] = 0.0;
            for (j = 0; j < N; j++) 
                c[i * N + k] += a[i * N + j] * b[j * N + k];  // Ajuste no acesso a b[j * N + k]
        }
    

        /* Enviar os resultados de volta para a tarefa mestre */
        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(c, rows * N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }

    /* Libera a memória alocada */
    free(a);
    free(b);
    free(c);

    MPI_Finalize();
}
