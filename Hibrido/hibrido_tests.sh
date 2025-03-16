#!/bin/bash

OUTPUT_CSV="tempos_execucao.csv"

if [ ! -f "$OUTPUT_CSV" ]; then
    echo "Teste,Tamanho,Num_Proc,Num_Threads,Real,User,Sys" > "$OUTPUT_CSV"
fi

export OMP_NUM_THREADS=4

for NUM_PROC in 2 4 8
do
    for i in {1..32}
    do
        ARQUIVO_A="A.txt"
        ARQUIVO_B="B.txt"

        if [ ! -f "$ARQUIVO_A" ] || [ ! -f "$ARQUIVO_B" ]; then
            echo "Erro: Arquivo(s) de matriz não encontrado(s)."
            exit 1
        fi

        MATRIX_SIZE=$(wc -l < "$ARQUIVO_A")

        /usr/bin/time -o tempo.txt -f "%e %U %S" mpirun -np $NUM_PROC ./matriz_hibrida $MATRIX_SIZE $ARQUIVO_A $ARQUIVO_B

        if [ $? -ne 0 ]; then
            echo "Erro: Falha ao executar mpirun"
            continue
        fi

        read REAL_TIME USER_TIME SYS_TIME < tempo.txt

        if [ -z "$REAL_TIME" ] || [ -z "$USER_TIME" ] || [ -z "$SYS_TIME" ]; then
            echo "Erro ao ler os tempos de execução."
            continue
        fi

        echo "$i,$MATRIX_SIZE,$NUM_PROC,$OMP_NUM_THREADS,$REAL_TIME,$USER_TIME,$SYS_TIME" >> "$OUTPUT_CSV"
        
        echo "$i,$MATRIX_SIZE,$NUM_PROC,$OMP_NUM_THREADS,$REAL_TIME,$USER_TIME,$SYS_TIME"

        rm tempo.txt
    done
done
