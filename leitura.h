#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 2000

typedef struct
{
    int total_linhas;
    int capacidade;
    char **linhas;
} txt;

txt leTexto(char nome_arquivo[]);

#endif