#!/bin/bash

NUM_PROC=4
ARQUIVO_A="A.txt"
ARQUIVO_B="B.txt"
ARQUIVO_SAIDA="tempos_execucao.csv"

if [[ ! -f "$ARQUIVO_A" || ! -f "$ARQUIVO_B" ]]; then
    echo "Erro: Arquivos de entrada não encontrados."
    exit 1
fi

echo "Teste,Tamanho,Real,User,Sys" > "$ARQUIVO_SAIDA"

for i in {1..1500}; do
    ARQUIVO_C="C_saida_${i}.txt"
    MATRIX_SIZE=$(wc -l < "$ARQUIVO_A")

    { /usr/bin/time -o tempo.txt -f "%e %U %S" mpirun -np $NUM_PROC ./programa $MATRIX_SIZE $ARQUIVO_A $ARQUIVO_B; }

    read REAL_TIME USER_TIME SYS_TIME < tempo.txt

    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME" >> "$ARQUIVO_SAIDA"
    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME"
done

rm tempo.txt

echo "Testes concluídos. Resultados salvos em $ARQUIVO_SAIDA."
