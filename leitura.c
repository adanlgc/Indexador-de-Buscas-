#include "leitura.h"
#include <stdio.h>
#include <string.h>

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

    char buffer[MAX_CHAR];

    while (fgets(buffer, MAX_CHAR, arquivo))
    {
        buffer[strcspn(buffer, "\n")] = 0;
        texto.linhas[texto.total_linhas] = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(texto.linhas[texto.total_linhas], buffer);
        texto.total_linhas++;

        if (texto.total_linhas == texto.capacidade)
        {
            texto.capacidade *= 2;
            char **aux = realloc(texto.linhas, texto.capacidade * sizeof(char *));

            if (!aux)
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