#!/bin/bash

OUTPUT_CSV="tempos_execucao.csv"

if [ ! -f "$OUTPUT_CSV" ]; then
    echo "Teste,Tamanho,Real,User,Sys" > "$OUTPUT_CSV"
fi

export OMP_NUM_THREADS=4

for i in {1..32}
do
    ARQUIVO_A="A.txt"
    ARQUIVO_B="B.txt"

    MATRIX_SIZE=$(wc -l < "$ARQUIVO_A")
    
    if [ ! -f "$ARQUIVO_A" ]; then
        echo "Erro: O arquivo $ARQUIVO_A não foi encontrado."
        exit 1
    fi
    
    if [ ! -f "$ARQUIVO_B" ]; then
        echo "Erro: O arquivo $ARQUIVO_B não foi encontrado."
        exit 1
    fi

    /usr/bin/time -o tempo.txt -f "%e %U %S" ./matriz_openmp $MATRIX_SIZE $ARQUIVO_A $ARQUIVO_B

    if [ $? -ne 0 ]; then
        echo "Erro: Não foi possível executar ./matriz_openmp"
        continue
    fi

    read REAL_TIME USER_TIME SYS_TIME < tempo.txt

    if [ -z "$REAL_TIME" ] || [ -z "$USER_TIME" ] || [ -z "$SYS_TIME" ]; then
        echo "Erro ao ler os tempos de execução."
        continue
    fi

    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME" >> "$OUTPUT_CSV"
    
    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME"

    rm tempo.txt
done
