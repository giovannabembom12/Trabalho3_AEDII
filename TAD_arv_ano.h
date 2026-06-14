#ifndef TAD_ARV_ANO_H
#define TAD_ARV_ANO_H

#include <stdio.h>
#include "registro.h"

// TAD: Árvore Binária de Pesquisa indexada pelo campo 'ano' (atributo não-chave, com valores repetidos).
typedef struct noReg {
    registro reg;   
    struct noReg *prox;
} noReg;

typedef struct arvAno {
    int ano;  
    noReg *lista; // registros com este ano
    struct arvAno *esq;
    struct arvAno *dir;
} arvAno;

typedef struct resLista {
    registro reg;
    struct resLista *prox;
} resLista;

#define OP_MAIOR 0  /* >  */
#define OP_MAIOR_IGUAL 1  /* >= */
#define OP_MENOR 2  /* <  */
#define OP_MENOR_IGUAL 3  /* <= */

arvAno *insereArvAno(arvAno *raiz, const registro *reg); 
arvAno *construirIndiceAno(FILE *arq);
void podaArvAno(arvAno *raiz);
resLista *buscarRangeAno(const arvAno *raiz, int limiar, int op);
void liberaResLista(resLista *lista);
int contarResLista(const resLista *lista);
void exibirIndiceAno(const arvAno *raiz);

#endif 