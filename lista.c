#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lista.h"

lista *criaLista()
{
    lista *lst = (lista *)malloc(sizeof(lista));
    lst->primeiro = NULL;

    return lst;
}

void destroiLista(lista *lst)
{
    no *p = lst->primeiro;
    no *tmp;
    sublista *p2;
    sublista *tmp2;

    while (p)
    {
        tmp = p;
        tmp2;
        p2 = p->ocorrencias;
        p = p->proximo;
        free(tmp->palavra);

        while (p2->proximo)
        {
            tmp2 = p2;
            p2 = p2->proximo;
            free(tmp2);
        }
        free(p2);
        free(tmp);
    }
    free(lst);
}

int busca(lista *lst, char elemento[], no **resultado)
{
    int cmp = 0;
    no *p = lst->primeiro;
    int i = 0;

    while (elemento[i])
    {
        elemento[i] = tolower(elemento[i]);
        i++;
    }

    while (p && strcmp(elemento, p->proximo->palavra) >= 0)
    {
        p = p->proximo;
        cmp++;
    }
    *resultado = (strcmp(elemento, p->palavra) == 0) ? p : NULL;

    return cmp;
}

int insere(lista *lst, char elemento[], int n_linha)
{
    no *p;
    no *p2;
    no *anterior;
    anterior = NULL;
    p = lst->primeiro;
    int i = 0;

    while (elemento[i])
    {
        elemento[i] = tolower(elemento[i]);
        i++;
    }

    while (p && strcmp(elemento, p->palavra) < 0)
    {
        anterior = p;
        p = p->proximo;
    }

    if (anterior && strcmp(elemento, anterior->palavra) == 0)
    {
        p2 = anterior->ocorrencias;
        while (p2->proximo)
        {
            p2 = p2->proximo;
        }
        sublista *nova_ocorrencia = (sublista *)malloc(sizeof(sublista));
        nova_ocorrencia->linha = n_linha;
        p2->proximo = nova_ocorrencia;
        anterior->quantidade++;

        return TRUE;
    }

    else
    {
        no *novo = (no *)malloc(sizeof(no));
        novo->palavra = (char *)malloc(strlen(elemento) + 1);
        strcpy(novo->palavra, elemento);
        novo->ocorrencias = (sublista *)malloc(sizeof(sublista));
        novo->ocorrencias->linha = n_linha;
        novo->ocorrencias->proximo = NULL;
        novo->quantidade = 1;
        novo->proximo = p;

        if (anterior)
            anterior->proximo = novo;
        else
            lst->primeiro = novo;
    }

    return TRUE;
}