#include "leitura.h"
#include <stdio.h>
#include <string.h>

// Recebe o nome do arquivo e devolve uma estrutura txt com o texto armazenado
txt leTexto(char nome_arquivo[])
{
    txt texto;
    texto.total_linhas = 0;
    texto.capacidade = 100;
    texto.linhas = malloc(texto.capacidade * sizeof(char *));
    FILE *arquivo = fopen(nome_arquivo, "r");

    if (!arquivo)
    {
        printf("Erro: nao foi possível abrir o arquivo\n");
        free(texto.linhas);

        exit(1);
    }

    char buffer[MAX_CHAR]; // Buffer para armazenar uma unica linha de texto

    while (fgets(buffer, MAX_CHAR, arquivo))
    {
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
                free(texto.linhas);
                fclose(arquivo);

                exit(1);
            }
            texto.linhas = aux;
        }
    }
    fclose(arquivo);

    return texto;
}

// OBS: 'texto.linhas' foi alocado dinamicamente e deve ser liberado no final do programa