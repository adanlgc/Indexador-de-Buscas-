#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "leitura.h"

// Recebe o nome do arquivo e devolve uma estrutura txt com o texto armazenado
txt leTexto(char nome_arquivo[])
{
    txt texto;               // Inicializacao da estrutura principal
    texto.total_linhas = 0;  // Inicializacao do contador de linhas dessa estrutura
    texto.capacidade = 100;  // Capacidade inicial para o total de linhas
    int chunk_inicial = 128; // Capacidade inicial para o tamanho das linhas
    // Alocacao dinamica inicial da quantidade de linhas e inicializacao da leitura
    texto.linhas = malloc(texto.capacidade * sizeof(char *));
    FILE *arquivo = fopen(nome_arquivo, "r");

    if (!arquivo) // Checagem de seguranca: fopen falhou
    {
        printf("Erro: nao foi possível abrir o arquivo\n");
        free(texto.linhas);

        exit(1);
    }

    // Buffer para armazenar uma unica linha de texto
    char *buffer = malloc(chunk_inicial);

    while (fgets(buffer, chunk_inicial, arquivo))
    {
        int tamanho_atual = chunk_inicial; // Capacidade dessa linha
        int len = strlen(buffer);          // Tamanho da string em buffer

        // "tamanho_atual" nao foi suficiente para armazenar essa linha, dobra e realoca
        while (len == tamanho_atual - 1 && buffer[len - 1] != '\n' && !feof(arquivo))
        {
            tamanho_atual *= 2;
            char *aux = realloc(buffer, tamanho_atual);

            if (!aux) // Evita memory leak: se nao houver livre aux sera NULL
            {
                printf("Erro: Memoria insuficiente para ler o arquivo");
                free(buffer);
                free(texto.linhas);
                fclose(arquivo);

                exit(1);
            }
            buffer = aux;
            if (fgets(buffer + len, tamanho_atual - len, arquivo))
                len += strlen(buffer + len);
            else // Chegou no final do arquivo (ou houve erro)
                break;
        }
        // Remove o caractere de quebra de linha que fgets insere automaticamente
        buffer[strcspn(buffer, "\n")] = 0;
        texto.linhas[texto.total_linhas] = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(texto.linhas[texto.total_linhas], buffer);
        texto.total_linhas++;

        // Dobra a capacidade do array utilizando realloc
        if (texto.total_linhas == texto.capacidade)
        {
            texto.capacidade *= 2;
            char **aux = realloc(texto.linhas, texto.capacidade * sizeof(char *));

            if (!aux) // Evita memory leak: se nao houver livre aux sera NULL
            {
                printf("Erro: memoria insuficiente para ler o arquivo");
                free(buffer);
                free(texto.linhas);
                fclose(arquivo);

                exit(1);
            }
            texto.linhas = aux;
        }
    }
    fclose(arquivo);

    free(buffer);
    return texto;
}

// OBS: 'texto.linhas' foi alocado dinamicamente e deve ser liberado no final do programa