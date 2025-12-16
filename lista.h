#ifndef LISTA_H
#define LISTA_H

#include "leitura.h"

// Uma lista simplificada para armazenar as linhas das ocorrencias da palavra buscada
typedef struct _ocorrencia_
{
    struct _ocorrencia_ *proximo;
    int linha; // O numero da linha onde essa ocorrencia esta
} sublista;

// No principal para a lista principal, contem a sublista
typedef struct _no_lista_
{
    struct _no_lista_ *proximo;
    char *palavra;         // String com a palavra em letras minusculas
    int quantidade;        // Quantidade de ocorrencias para esta palavra
    sublista *ocorrencias; // "Mini-lista" que armazena as linhas das ocorrencias
} no_lista;

// Lista ligada principal: ordenada em ordem alfabetica
typedef struct
{
    no_lista *primeiro;
    int tamanho; // Contador para o total de palavras unicas indexadas
} lista;

// Devolve um ponteiro para uma lista vazia
lista *criaLista();

// Recebe o ponteiro para uma lista e a destroi
void destroiLista(lista *lst);

/* Recebe um ponteiro para a lista, a palavra buscada, e um ponteiro para um ponteiro de
um no vazio. Retorna o numero de comparacoes para a busca e passa o endereco da palavra
encontrada (ou NULL), por meio de ponteiro, para o resultado, fora da funcao */
long long int buscaLista(lista *lst, char elemento[], no_lista **resultado);

/* Recebe um ponteiro para a lista, a string da palavra a ser inserida e o numero da
linha de onde ela veio. Se a palavra ja existir na lista, apenas adiciona a nova
ocorrencia no no da palavra antiga, se nao, a insere de modo a manter a ordem alfabetica.
Alem disso, padroniza a palavra para a sua versao em letras minusculas. Retorna o numero
comparacoes realizadas na operacao */
long long int insereLista(lista *lst, char elemento[], int n_linha);

/* Recebe a variavel txt que armazena o texto e um ponteiro para um ponteiro vazio de
lista ligada. Retorna o numero de comparacoes realizadas para a criacao do indice e
aponta o ponteiro de ponteiro de lista recebido para o indice final */
long long int paraLista(txt texto, lista **lst0);

#endif