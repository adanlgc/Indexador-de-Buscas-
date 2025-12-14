#ifndef LISTA_H
#define LISTA_H

// Uma lista simplificada para armazenar as linhas das ocorrencias da palavra buscada
typedef struct _ocorrencia_
{
    struct _ocorrencia_ *proximo;
    int linha; // O numero da linha onde essa ocorrencia esta
} sublista;

// No principal para a lista principal, contem a sublista
typedef struct _no_
{
    struct _no_ *proximo;
    char *palavra;         // String com a palavra em letras minusculas
    int quantidade;        // Quantidade de ocorrencias para esta palavra
    sublista *ocorrencias; // "Mini-lista" que armazena as linhas das ocorrencias
} no;

// Lista ligada principal: ordenada em ordem alfabetica
typedef struct
{
    no *primeiro;
    int tamanho; // Contador para o total de palavras unicas indexadas
} lista;

// Devolve uma lista vazia
lista *criaLista();

// Recebe o ponteiro para uma lista e a destroi
void destroiLista(lista *lst);

/* Recebe um ponteiro para a lista, a palavra buscada, e um ponteiro para um ponteiro de
um no. Retorna o numero de comparacoes para a busca e passa o endereco da palavra
encontrada (ou NULL), por meio de ponteiro, para o resultado, fora da funcao */
int busca(lista *lst, char elemento[], no **resultado);

/* Recebe um ponteiro para a lista, a string da palavra a ser inserida e o numero da
linha de onde ela veio. Se a palavra ja existir na lista, apenas adiciona a nova
ocorrencia no no da palavra antiga, se nao, a insere de modo a manter a ordem alfabetica.
Alem disso, padroniza a palavra para a sua versao em letras minusculas */
void insere(lista *lst, char elemento[], int n_linha);

/* Recebe a variavel txt que armazena o texto e devolve um ponteiro para uma lista ligada
criada nela mesma (o criaLista ja e usado dentro desta funcao) */
lista *paraLista(txt texto);

#endif