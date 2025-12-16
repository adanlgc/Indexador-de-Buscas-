#ifndef ARVORE_H
#define ARVORE_H

#include "leitura.h"
#include "lista.h" // Importar o tipo sublista

// No principal para a arvore principal, contem a sublista
typedef struct _no_arvore_
{
    struct _no_arvore_ *esq;
    struct _no_arvore_ *dir;
    int h;                 // Altura do no
    char *palavra;         // String com a palavra em letras minusculas
    int quantidade;        // Quantidade de ocorrencias para esta palavra
    sublista *ocorrencias; // "Mini-lista" que armazena as linhas das ocorrencias
} no_arvore;

// Arvore AVL: ordenada em ordem alfabetica
typedef struct
{
    no_arvore *raiz;
    int tamanho; // Contador para o total de palavras unicas indexadas
} arvore;

// Devolve um ponteiro para uma arvore vazia
arvore *criaArvore();

// Recebe o ponteiro para a raiz de uma arvore e destroi todos os seus descendentes
void destroiArvoreREC(no_arvore *raiz);

// Recebe o ponteiro para uma lista e a destroi utilizando destroiArvoreREC
void destroiArvore(arvore *arv);

/* Recebe um ponteiro para um no de arvore, a palavra buscada, e um ponteiro para um
ponteiro de um no vazio. Recursivamente busca pela palavra buscada nos nos inferiores,
aponta resultado para ela e retorna o numero de comparacoes realizadas. Para ser usado
pela funcao abaixo */
long long int buscaArvoreREC(no_arvore *no, char elemento[], no_arvore **resultado);

/* Recebe um ponteiro para a arvore, a palavra buscada, e um ponteiro para um ponteiro de
um no vazio. Retorna o numero de comparacoes para a busca e passa o endereco da palavra
encontrada (ou NULL), por meio de ponteiro, para o resultado, fora da funcao */
long long int buscaArvore(arvore *arv, char elemento[], no_arvore **resultado);

// Funcao auxiliar que retorna o maior entre dois numeros
int max(int a, int b);

// Recebe um ponteiro para um no da arvore e retorna o seu balanco
int balanco(no_arvore *no);

/* Recebe um ponteiro para um no da arvore cujas alturas dos descendentes ja estao
atualizadas e atualiza o no atual */
void atualizaAltura(no_arvore *no);

// Recebe um ponteiro para um no e o rotaciona em L
no_arvore *rotacaoL(no_arvore *p);

// Recebe um ponteiro para um no e o rotaciona em R
no_arvore *rotacaoR(no_arvore *p);

/* Recebe um ponteiro para uma arvore, outros para os nos da raiz, do pai (ou NULL) e do
novo no a ser inserido, e o insere de modo a manter a ordenacao. Funcao auxiliar, retorna
o numero de comparacoes realizadas na operacao */
long long int insereArvoreREC(arvore *arv, no_arvore *raiz, no_arvore *pai, no_arvore *novo);

/* Utiliza a funcao auxiliar insereArvoreREC para inserir um par palavra-linha na
arvore. Recebe um ponteiro para arvore, uma string com a palavra e um inteiro com o
numero da linha, retorna o numero de comparacoes realizadas na operacao */
long long int insereArvore(arvore *arv, char elemento[], int n_linha);

/* Recebe a variavel txt que armazena e um ponteiro para um ponteiro vazio de arvore AVL.
Retorna o numero de comparacoes realizadas para a criacao do indice e aponta o ponteiro
de ponteiro de arvore recebido para o indice final */
long long int paraArvore(txt texto, arvore **arv);

#endif