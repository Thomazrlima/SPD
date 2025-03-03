#!/bin/bash

OUTPUT_CSV="tempos_execucao.csv"

if [ ! -f "$OUTPUT_CSV" ]; then
    echo "Teste,Tamanho,Real,User,Sys" > "$OUTPUT_CSV"
fi

for i in {1..1500}
do
    ARQUIVO_A="A.txt"
    ARQUIVO_B="B.txt"

    MATRIX_SIZE=$(wc -l < "$ARQUIVO_A")

    { /usr/bin/time -o tempo.txt -f "%e %U %S" ./programa $MATRIX_SIZE $ARQUIVO_A $ARQUIVO_B; } 2>/dev/null

    read REAL_TIME USER_TIME SYS_TIME < tempo.txt

    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME" >> "$OUTPUT_CSV"
    echo "$i,$MATRIX_SIZE,$REAL_TIME,$USER_TIME,$SYS_TIME"

    rm tempo.txt
done
