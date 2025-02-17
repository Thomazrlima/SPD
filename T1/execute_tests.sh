#!/bin/bash

OUTPUT_CSV="tempos_execucao.csv"
echo "Teste,Tamanho,Real,User,Sys" > $OUTPUT_CSV

for i in {1..15}
do
    ARQUIVO_A="A.txt"
    ARQUIVO_B="B.txt"
    ARQUIVO_C="C_saida_${i}.txt"

    MATRIX_SIZE=$(wc -l < "$ARQUIVO_A")

    { time ./programa $MATRIX_SIZE $ARQUIVO_A $ARQUIVO_B $ARQUIVO_C; } 2> tempo.txt

    REAL_TIME=$(grep real tempo.txt | awk '{print $2}')
    USER_TIME=$(grep user tempo.txt | awk '{print $2}')
    SYS_TIME=$(grep sys tempo.txt | awk '{print $2}')

    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME" >> $OUTPUT_CSV
    echo "Teste $i: Tamanho=$MATRIX_SIZE, Real=$REAL_TIME, User=$USER_TIME, Sys=$SYS_TIME"
done

rm tempo.txt
