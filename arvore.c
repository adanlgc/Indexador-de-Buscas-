#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "arvore.h"

#define TRUE 1
#define FALSE 0

// Devolve um ponteiro para uma arvore vazia
arvore *criaArvore()
{
    arvore *arv = malloc(sizeof(arvore));

    if (!arv)
    {
        printf("Erro: memoria insuficiente\n");

        exit(1);
    }
    arv->raiz = NULL;
    arv->tamanho = 0;
    return arv;
}

// Recebe o ponteiro para a raiz de uma arvore e destroi todos os seus descendentes
void destroiArvoreREC(no_arvore *raiz)
{
    if (!raiz) // Recursao chegou ao fim, o no atual nao existe
        return;
    // Ponteiros auxiliares
    no_arvore *esq = raiz->esq;
    no_arvore *dir = raiz->dir;
    sublista *p = raiz->ocorrencias;
    sublista *tmp;

    free(raiz->palavra);

    while (p)
    {
        tmp = p;
        p = p->proximo;
        free(tmp);
    }
    // Chama a funcao recursivamente para apagar os seus dois filhos
    destroiArvoreREC(esq);
    destroiArvoreREC(dir);
    free(raiz);
}

// Recebe o ponteiro para uma lista e a destroi utilizando destroiArvoreREC
void destroiArvore(arvore *arv)
{
    // Apaga todos os nos da arvore, comecando pelas folhas e subindo ate a raiz
    destroiArvoreREC(arv->raiz);
    // Apaga a estrutura geral
    free(arv);
}

/* Recebe um ponteiro para um no de arvore, a palavra buscada, e um ponteiro para um
ponteiro de um no vazio. Recursivamente busca pela palavra buscada nos nos inferiores,
aponta resultado para ela e retorna o numero de comparacoes realizadas. Para ser usado
pela funcao abaixo */
int buscaArvoreREC(no_arvore *no, char elemento[], no_arvore **resultado)
{
    if (no)
    {
        // Valor de comparacao entre a palavra buscada e a palavra do no atual
        int relacao = strcmp(elemento, no->palavra);

        if (relacao == 0)
        {
            *resultado = no;

            // Palavra encontrada, comparacao final feita
            return 1;
        }
        if (relacao < 0)
            // Palavra menor que a atual, conta uma comparacao e busca no no esquerdo
            return 1 + buscaArvoreREC(no->esq, elemento, resultado);
        else
            // Palavra maior que a atual, conta uma comparacao e busca no no direito
            return 1 + buscaArvoreREC(no->dir, elemento, resultado);
    }
    // No atual nao existe e a palavra nao foi encontrada
    *resultado = NULL;

    return 0;
}

/* Recebe um ponteiro para a arvore, a palavra buscada, e um ponteiro para um ponteiro de
um no vazio. Retorna o numero de comparacoes para a busca e passa o endereco da palavra
encontrada (ou NULL), por meio de ponteiro, para o resultado, fora da funcao */
int buscaArvore(arvore *arv, char elemento[], no_arvore **resultado)
{
    // Converte a palavra para letras minusculas, padronizacao para operacoes
    for (int i = 0; elemento[i] != '\0'; i++)
        elemento[i] = tolower(elemento[i]);
    return buscaArvoreREC(arv->raiz, elemento, resultado);
}

// Funcao auxiliar que retorna o maior entre dois numeros
int max(int a, int b)
{
    return a > b ? a : b;
}

// Recebe um ponteiro para um no da arvore e retorna o seu balanco
int balanco(no_arvore *no)
{
    if (no->esq && no->dir)
        return (no->dir->h) - (no->esq->h);
    if (no->esq)
        return -1 * (no->esq->h + 1);
    if (no->dir)
        return no->dir->h + 1;
    return 0;
}

/* Recebe um ponteiro para um no da arvore cujas alturas dos descendentes ja estao
atualizadas e atualiza o no atual */
void atualizaAltura(no_arvore *no)
{
    if (no->esq && no->dir)
        no->h = max(no->esq->h, no->dir->h) + 1;
    else if (no->dir)
        no->h = no->dir->h + 1;
    else if (no->esq)
        no->h = no->esq->h + 1;
    else
        no->h = 0;
}

// Recebe um ponteiro para um no e o rotaciona em L
no_arvore *rotacaoL(no_arvore *p)
{
    no_arvore *v;
    no_arvore *u = p->esq;

    if (balanco(u) == -1)
    {
        // Rotaciona em LL
        p->esq = u->dir;
        u->dir = p;
        p->h -= 2;
        return u;
    }
    else if (balanco(u) == 1)
    {
        // Rotaciona em LR
        v = u->dir;
        u->dir = v->esq;
        v->esq = u;
        p->esq = v->dir;
        v->dir = p;
        p->h -= 2;
        u->h -= 1;
        v->h += 1;

        return v;
    }

    return NULL;
}

// Recebe um ponteiro para um no e o rotaciona em R
no_arvore *rotacaoR(no_arvore *p)
{
    no_arvore *v;
    no_arvore *u = p->dir;

    if (balanco(u) == 1)
    {
        // Rotaciona em RR
        p->dir = u->esq;
        u->esq = p;
        p->h -= 2;
        return u;
    }
    else if (balanco(u) == -1)
    {
        // Rotaciona em RL
        v = u->esq;
        u->esq = v->dir;
        v->dir = u;
        p->dir = v->esq;
        v->esq = p;
        p->h -= 2;
        u->h -= 1;
        v->h += 1;

        return v;
    }

    return NULL;
}

/* Recebe um ponteiro para uma arvore, outros para os nos da raiz, do pai (ou NULL) e do
novo no a ser inserido, e o insere de modo a manter a ordenacao. Funcao auxiliar, retorna
o numero de comparacoes realizadas na operacao */
int insereArvoreREC(arvore *arv, no_arvore *raiz, no_arvore *pai, no_arvore *novo)
{
    // Contador de comparacoes realizadas em insereArvoreREC
    int cmp = 0;
    // Armazena o no rotacionado
    no_arvore *rot;
    // Ponteiros auxiliares
    sublista *p;
    sublista *tmp;
    // Valor de comparacao entre a palavra buscada e a palavra do no atual
    int relacao = strcmp(novo->palavra, raiz->palavra);
    cmp++;

    // Palavra e diferente da atual
    if (relacao != 0)
    {
        // Palavra e menor que a atual
        if (relacao < 0)
        {
            /* Repete a recorrencia ate encontrar um no vazio para inseri-la, e atualiza
            os nos subjacentes */
            if (raiz->esq)
            {
                cmp += insereArvoreREC(arv, raiz->esq, raiz, novo);
                atualizaAltura(raiz);

                // No desbalanceado, realiza rotacao em L
                if (abs(balanco(raiz)) >= 2)
                {
                    rot = rotacaoL(raiz);

                    if (pai)
                    {
                        if (pai->esq == raiz)
                            pai->esq = rot;
                        if (pai->dir == raiz)
                            pai->dir = rot;
                    }
                    else
                        arv->raiz = rot;
                }

                return cmp;
            }
            else
            {
                // Conecta o novo no e atualiza a altura do pai
                raiz->esq = novo;
                if (!raiz->dir)
                    raiz->h = 1;
            }
        }
        else
        {
            /* Repete a recorrencia ate encontrar um no vazio para inseri-la, e atualiza
            os nos subjacentes */
            if (raiz->dir)
            {
                cmp += insereArvoreREC(arv, raiz->dir, raiz, novo);
                atualizaAltura(raiz);

                // No desbalanceado, realiza rotacao em R
                if (abs(balanco(raiz)) >= 2)
                {
                    rot = rotacaoR(raiz);

                    if (pai)
                    {
                        if (pai->esq == raiz)
                            pai->esq = rot;
                        if (pai->dir == raiz)
                            pai->dir = rot;
                    }
                    else
                        arv->raiz = rot;
                }

                return cmp;
            }
            else
            {
                // Conecta o novo no e atualiza a altura do pai
                raiz->dir = novo;
                if (!raiz->esq)
                    raiz->h = 1;
            }
        }
        // Atualiza o contador de tamanho da arvore
        arv->tamanho++;

        return cmp;
    }
    // Palavra ja existe na arvore, atualiza a lista de ocorrencias
    raiz->quantidade++;
    p = raiz->ocorrencias;

    while (p)
    {
        tmp = p;
        p = p->proximo;
    }
    tmp->proximo = novo->ocorrencias;
    // Apaga as alocacoes dinamicas para o novo no que nao sera mais necessario
    free(novo->palavra);
    free(novo);

    return cmp;
}

/* Utiliza a funcao auxiliar insereArvoreREC para inserir um par palavra-linha na
arvore. Recebe um ponteiro para arvore, uma string com a palavra e um inteiro com o
numero da linha, retorna o numero de comparacoes realizadas na operacao */
int insereArvore(arvore *arv, char elemento[], int n_linha)
{
    // Converte a palavra para letras minusculas, padronizacao para operacoes
    for (int i = 0; elemento[i] != '\0'; i++)
        elemento[i] = tolower(elemento[i]);
    // Aloca dinamicamente um espaco para o novo no e o inicializa
    no_arvore *novo = malloc(sizeof(no_arvore));

    if (!novo)
    {
        printf("Erro: memoria insuficiente\n");
        destroiArvore(arv);

        exit(1);
    }
    novo->palavra = malloc(strlen(elemento) + 1);

    if (!novo->palavra)
    {
        printf("Erro: memoria insuficiente\n");
        destroiArvore(arv);
        free(novo);

        exit(1);
    }
    novo->quantidade = 1;
    novo->ocorrencias = malloc(sizeof(sublista));

    if (!novo->ocorrencias)
    {
        printf("Erro: memoria insuficiente\n");
        destroiArvore(arv);
        free(novo);
        free(novo->palavra);

        exit(1);
    }
    novo->ocorrencias->linha = n_linha;
    novo->ocorrencias->proximo = NULL;
    strcpy(novo->palavra, elemento);
    novo->esq = novo->dir = NULL;
    novo->h = 0;

    // Arvore nao esta vazia, inicializa recursao de insercao
    if (arv->raiz)
        return insereArvoreREC(arv, arv->raiz, NULL, novo);
    // Arvore vazia, novo no vira a raiz
    arv->raiz = novo;

    return 0;
}

/* Recebe a variavel txt que armazena e um ponteiro para um ponteiro vazio de arvore AVL.
Retorna o numero de comparacoes realizadas para a criacao do indice e aponta o ponteiro
de ponteiro de arvore recebido para o indice final */
int paraArvore(txt texto, arvore **arv0)
{
    int cmp = 0;                 // Contador de comparacoes realizadas em paraArvore
    char *token;                 // Ponteiro que recebe o endereco dos tokens
    arvore *arv1 = criaArvore(); // Passa um endereco de arvore vazia para o ponteiro

    for (int i = 0; i < texto.total_linhas; i++)
    {
        // Recebe uma linha inteira
        char *buffer = malloc(strlen(texto.linhas[i]) + 1);

        if (!buffer)
        {
            printf("Erro: memoria insuficiente\n");
            destroiArvore(arv1);

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
            cmp += insereArvore(arv1, token, i + 1);
            token = strtok(NULL, " \n\t\r");
        }
        free(buffer);
    }
    *arv0 = arv1;

    return cmp;
}