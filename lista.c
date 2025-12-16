#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lista.h"

// Devolve um ponteiro para uma lista vazia
lista *criaLista()
{
    lista *lst = malloc(sizeof(lista));

    if (!lst)
    {
        printf("Erro: memoria insuficiente\n");

        exit(1);
    }
    lst->primeiro = NULL;
    lst->tamanho = 0;

    return lst;
}

// Recebe o ponteiro para uma lista e a destroi
void destroiLista(lista *lst)
{
    // Todos ponteiros auxiliares
    no_lista *p = lst->primeiro;
    no_lista *tmp;
    sublista *p2;
    sublista *tmp2;

    // Apaga tudo dentro da lista
    while (p)
    {
        tmp = p;
        p2 = p->ocorrencias;
        p = p->proximo;
        free(tmp->palavra);

        while (p2)
        {
            tmp2 = p2;
            p2 = p2->proximo;
            free(tmp2);
        }
        free(tmp);
    }
    // Apaga a lista
    free(lst);
}

/* Recebe um ponteiro para a lista, a palavra buscada, e um ponteiro para um ponteiro de
um no vazio. Retorna o numero de comparacoes para a busca e passa o endereco da palavra
encontrada (ou NULL), por meio de ponteiro, para o resultado, fora da funcao */
long long int buscaLista(lista *lst, char elemento[], no_lista **resultado)
{
    long long int cmp = 0;       // Contador de comparacoes
    no_lista *p = lst->primeiro; // Ponteiro auxiliar de no

    // Checa se a lista esta vazia
    if (!p)
    {
        *resultado = NULL;
        return 0;
    }

    // Converte a palavra para letras minusculas, padronizacao para operacoes
    for (int i = 0; elemento[i] != '\0'; i++)
        elemento[i] = tolower(elemento[i]);

    // Realiza comparacoes ate encontrar uma palavra que nao seja menor que a buscada
    while (p && strcmp(elemento, p->palavra) > 0)
    {
        p = p->proximo;
        cmp++;
    }

    // Checa se nao chegou no final da lista
    if (!p)
    {
        *resultado = NULL;
        return cmp;
    }
    // Palavra obtida e igual a buscada? Se sim passa para o resultado, se nao passa NULL
    *resultado = (strcmp(elemento, p->palavra) == 0) ? p : NULL;
    // Atualiza para a comparacao acima
    cmp++;

    return cmp;
}

/* Recebe um ponteiro para a lista, a string da palavra a ser inserida e o numero da
linha de onde ela veio. Se a palavra ja existir na lista, apenas adiciona a nova
ocorrencia no no da palavra antiga, se nao, a insere de modo a manter a ordem alfabetica.
Alem disso, padroniza a palavra para a sua versao em letras minusculas. Retorna o numero
comparacoes realizadas na operacao */
long long int insereLista(lista *lst, char elemento[], int n_linha)
{
    long long int cmp = 0; // Contador de comparacoes realizadas em insereLista
    int relacao = 0;       // Para armazenar o valor de strcmp
    // Ponteiros auxiliares
    no_lista *p = lst->primeiro;
    no_lista *anterior = NULL;
    // Aloca dinamicamente a nova ocorrencia
    sublista *nova_ocorrencia = malloc(sizeof(sublista));

    if (!nova_ocorrencia)
    {
        printf("Erro: memoria insuficiente\n");
        destroiLista(lst);

        exit(1);
    }
    nova_ocorrencia->linha = n_linha;
    nova_ocorrencia->proximo = NULL;

    // Transforma todas as letras da palavra em letras minusculas
    for (int i = 0; elemento[i] != '\0'; i++)
    {
        elemento[i] = tolower(elemento[i]);
    }

    // Aponta 'p' para o proximo do novo, e 'anterior' para o anterior ao novo
    while (p)
    {
        relacao = strcmp(elemento, p->palavra);
        cmp++;
        if (relacao <= 0)
            break;
        anterior = p;
        p = p->proximo;
    }

    // Verifica se a palavra a ser inserida ja existe, se sim adiciona a nova ocorrencia
    if (p && relacao == 0)
    {
        // Outro ponteiro auxiliar
        sublista *p2 = p->ocorrencias;

        while (p2->proximo)
        {
            p2 = p2->proximo;
        }
        p2->proximo = nova_ocorrencia;
        p->quantidade++; // Incrementa o contador de quantidade
    }
    // A palavra ainda nao existe na lista: cria um novo no para ela e a insere
    else
    {
        // Alocacao dinamica do novo no e sua palavra, que deve incluir um espaco para \0
        no_lista *novo = malloc(sizeof(no_lista));

        if (!novo)
        {
            printf("Erro: memoria insuficiente\n");
            destroiLista(lst);
            free(nova_ocorrencia);

            exit(1);
        }
        novo->palavra = malloc(strlen(elemento) + 1);

        if (!novo->palavra)
        {
            printf("Erro: memoria insuficiente\n");
            destroiLista(lst);
            free(nova_ocorrencia);
            free(novo);

            exit(1);
        }

        strcpy(novo->palavra, elemento);
        novo->proximo = p;
        novo->ocorrencias = nova_ocorrencia;
        novo->quantidade = 1;
        lst->tamanho++;

        // Faz as conexoes para os casos: no e o primeiro da lista ou ha um anterior
        if (anterior)
            anterior->proximo = novo;
        else
            lst->primeiro = novo;
    }

    return cmp;
}

/* Recebe a variavel txt que armazena o texto e um ponteiro para um ponteiro vazio de
lista ligada. Retorna o numero de comparacoes realizadas para a criacao do indice e
aponta o ponteiro de ponteiro de lista recebido para o indice final */
long long int paraLista(txt texto, lista **lst0)
{
    long long int cmp = 0;
    char *token;               // Ponteiro que recebe o endereco dos tokens
    lista *lst1 = criaLista(); // Ponteiro de lista ligada que sera retornado

    for (int i = 0; i < texto.total_linhas; i++)
    {
        // Recebe uma linha inteira
        char *buffer = malloc(strlen(texto.linhas[i]) + 1);

        if (!buffer)
        {
            printf("Erro: memoria insuficiente\n");
            destroiLista(lst1);

            exit(1);
        }
        strcpy(buffer, texto.linhas[i]);

        // Apaga cada sinal de pontuacao e o substitui por um espaco
        for (int j = 0; buffer[j] != '\0'; j++)
        {
            if (ispunct(buffer[j]) != 0)
            {
                buffer[j] = ' ';
            }
        }
        token = strtok(buffer, " \n\t\r");

        // Insere cada token na lista. n_linha = i + 1 pois a contagem e a partir de 1
        while (token)
        {
            cmp += insereLista(lst1, token, i + 1);
            token = strtok(NULL, " \n\t\r");
        }
        free(buffer);
    }
    *lst0 = lst1;

    return cmp;
}