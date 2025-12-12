#ifndef LISTA_H
#define LISTA_H

#define FALSE 0
#define TRUE 1

typedef struct _ocorrencia_
{
    struct _ocorrencia_ *proximo;
    int linha;
} sublista;

typedef struct _no_
{
    struct _no_ *proximo;
    char *palavra;
    int quantidade;
    sublista *ocorrencias;
} no;

typedef struct
{
    no *primeiro;
    int tamanho;
} lista;

lista *criaLista();
void destroiLista(lista *lst);

#endif