#define FALSE 0
#define TRUE 1

#define INDEFINIDO -1
#define ESQUERDO 0
#define DIREITO 1

#define MAX_CHAR 2000

typedef struct
{
    int total_linhas;
    int capacidade;
    char **linhas;
} txt;

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