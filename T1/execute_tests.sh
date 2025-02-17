#!/bin/bash

# Definindo o nome do arquivo de saída para os tempos
OUTPUT_CSV="tempos_execucao.csv"

# Inicializa o arquivo CSV com um cabeçalho
echo "Teste,Tempo (segundos)" > $OUTPUT_CSV

# Executa 15 testes
for i in {1..15}
do
    # Nome dos arquivos de entrada e saída
    ARQUIVO_A="A.txt"
    ARQUIVO_B="B.txt"
    ARQUIVO_SAIDA="C_saida_${i}.txt"

    # Inicia a contagem do tempo
    START_TIME=$(date +%s.%N)

    # Executa o programa C com os arquivos fornecidos
    ./programa $ARQUIVO_A $ARQUIVO_B $ARQUIVO_SAIDA

    # Marca o tempo de execução
    END_TIME=$(date +%s.%N)
    EXEC_TIME=$(echo "$END_TIME - $START_TIME" | bc)

    # Grava o tempo no arquivo CSV
    echo "$i,$EXEC_TIME" >> $OUTPUT_CSV

    echo "Teste $i: $EXEC_TIME segundos"
done
