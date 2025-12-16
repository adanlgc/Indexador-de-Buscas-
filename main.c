#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "leitura.h"
#include "lista.h"
#include "arvore.h"

#define TRUE 1
#define FALSE 0
#define LISTA 0
#define ARVORE 1

/* Funcao auxiliar que le a entrada de um usuario e retorna um ponteiro para uma string.
OBS: alocacao dinamica e usada e a string armazenada precisa ser liberada no final */
char *leComando()
{
    // Tamanho inicial da string que armazena o comando
    int tam_cmd = 16;
    char *comando = malloc(tam_cmd);
    // Variaveis auxiliares
    int c;
    char *aux;

    // Laco que repete indefinidamente e mantem o indice 'i' da string atualizado
    for (int i = 0; 1; i++)
    {
        // E necessario mais espaco: realoca o dobro do tamanho original
        if (i == tam_cmd - 1)
        {
            tam_cmd *= 2;
            aux = realloc(comando, tam_cmd);
            if (aux)
                comando = aux;
            else
            {
                printf("Erro: memoria insuficiente para a leitura do comando\n");
                return NULL;
            }
        }
        c = getc(stdin);

        // Final da entrada, substitui o caractere pelo \0 que sera usado em strtok
        if (c == '\n' || c == EOF)
        {
            comando[i] = '\0';
            break;
        }
        comando[i] = c;
    }

    return comando;
}

int main(int argc, char *argv[])
{
    // Checagem de seguranca: usuario esqueceu algum parametro
    if (argc < 3)
    {
        printf("Erro: arquivo faltando\n");
        exit(1);
    }
    // Modo de busca: 0 para lista ligada, 1 para arvore AVL
    int modo;

    if (strcmp(argv[2], "lista") == 0)
        modo = LISTA;
    else if (strcmp(argv[2], "arvore") == 0)
        modo = ARVORE;
    // Usuario digitou errado o modo de busca
    else
    {
        printf("Erro: modo invalido\n");
        exit(1);
    }

    /* Ja inclui todas as checagens de seguranca.
    OBS: texto deve ser liberado antes de sair da funcao */
    txt texto = leTexto(argv[1]);

    if (modo == LISTA)
    {
        lista *lst;
        int cmp = paraLista(texto, &lst);
        printf("Arquivo: '%s'\n", argv[1]);
        printf("Tipo de indice: 'lista'\n");
        printf("Numero de linhas no arquivo: %d\n", texto.total_linhas);
        printf("Total de palavras unicas indexadas: %d\n", lst->tamanho);
        printf("Numero de comparacoes realizadas para a construcao do indice: %d", cmp);
        printf("\n> ");

        char *comando = leComando();

        // Realloc falhou/falta de espaco: apaga tudo e sai do programa
        if (!comando)
        {
            destroiLista(lst);
            destroiTexto(&texto);
            free(comando);
            exit(1);
        }
        char *token = strtok(comando, " ");
        no_lista *resultado; // Para armazenar o no da busca

        // Usuario nao digitou nada ou houve erro, abre o campo novamente
        while (!token)
        {
            printf("\n> ");
            free(comando);
            comando = leComando();

            if (!comando)
            {
                destroiLista(lst);
                destroiTexto(&texto);
                free(comando);
                exit(1);
            }
            token = strtok(comando, " ");
        }

        // Repete a interacao ate o comando ser "fim"
        while (strcmp(token, "fim") != 0)
        {
            // Usuario iniciou uma busca
            if (strcmp(token, "busca") == 0)
            {
                token = strtok(NULL, " ");

                // Usuario nao digitou a palavra
                if (!token)
                    printf("Opcao invalida!\n");
                else
                {
                    cmp = buscaLista(lst, token, &resultado);

                    // Palavra digitada nao foi encontrada
                    if (!resultado)
                    {
                        printf("Palavra '%s' nao encontrada.\n", token);
                    }
                    // Palavra digitada foi encontrada
                    else
                    {
                        printf("Existem %d ocorrencias da palavra", resultado->quantidade);
                        printf("'%s' na(s) seguinte(s) linha(s):\n", resultado->palavra);
                        sublista *p = resultado->ocorrencias;

                        // Imprime todas as ocorrencias da palavra
                        while (p)
                        {
                            printf("%05d: %s\n", p->linha, texto.linhas[p->linha - 1]);
                            p = p->proximo;
                        }
                    }
                    printf("Numero de comparacoes: %d\n", cmp);
                }
            }
            // Usuario digitou um comando errado
            else
            {
                printf("Opcao invalida!\n");
            }
            printf("> ");
            // Segunda leitura: pode ser "fim", por isso e lido antes do fim do laco
            free(comando);
            comando = leComando();

            if (!comando)
            {
                destroiLista(lst);
                destroiTexto(&texto);
                free(comando);
                exit(1);
            }
            token = strtok(comando, " ");

            // Usuario nao digitou nada ou houve erro, abre o campo novamente
            while (!token)
            {
                printf("\n> ");
                free(comando);
                comando = leComando();

                if (!comando)
                {
                    destroiLista(lst);
                    destroiTexto(&texto);
                    free(comando);
                    exit(1);
                }
                token = strtok(comando, " ");
            }
        }
        // Usuario encerrou o programa, apaga tudo e sai
        free(comando);
        destroiTexto(&texto);
        destroiLista(lst);

        return 0;
    }
}