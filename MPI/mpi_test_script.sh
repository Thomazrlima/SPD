#!/bin/bash

NUM_PROC=4

tamanho=1024
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

    { time -f "%e %U %S" mpirun -np $NUM_PROC ./programa $MATRIX_SIZE $ARQUIVO_A $ARQUIVO_B $ARQUIVO_C; } 2> tempo.txt

    REAL_TIME=$(grep real tempo.txt | awk '{print $2}')
    USER_TIME=$(grep user tempo.txt | awk '{print $2}')
    SYS_TIME=$(grep sys tempo.txt | awk '{print $2}')

    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME" >> "$ARQUIVO_SAIDA"
    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME"
done

rm tempo.txt

echo "Testes concluídos. Resultados salvos em $ARQUIVO_SAIDA."
