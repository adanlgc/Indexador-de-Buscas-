#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 2000 // Maximo de caracteres aceitos em uma linha do .txt

// Estrutura principal para armazenar o texto completo, dividido em linhas
typedef struct
{
    int total_linhas; // Quantidade de linhas no arquivo .txt
    int capacidade;   // Inteiro usado para alocação dinamica de 'linhas' abaixo
    char **linhas;    // Ponteiro para o vetor de strings que armazena as linhas
} txt;

// Recebe o nome do arquivo e devolve uma estrutura txt com o texto armazenado
txt leTexto(char nome_arquivo[]);

#endif