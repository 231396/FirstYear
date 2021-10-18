#ifndef ARVORE_H
#define ARVORE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Sequencia {
   int v[1000];
   int top;
}Sequencia;


typedef struct No{
   int valor;
   struct No * esq;
   struct No * dir;
}No;



typedef struct Tree{
   No * no;
}Tree;

/** construtor da sequencia */
Sequencia * newSequencia();

/** insere valor na sequencia */
void insereSeq(Sequencia *s, int valor);


/** imprime sequencia */
void imprimeS(Sequencia *s);

/** cria uma sequencia aleatória com size
elementos */
Sequencia * randomSeq(int size,int max);


/** construtor da árvore */
Tree * newTree();

/** construtor do nó */
No * newNo(int valor);


/** insere um valor na árvore */
void insereT(Tree * t, int valor);

/** insere na árvore binária de busca.
Elementos menores à esquerda. Maiores à direita. 
Assume-se que no não vale NULL*/
void insereN(No * no, int valor);

/** imprime a árvore no formato xml */
void imprimeXml(No * no);


/** faz a busca de um valor na árvore */
int busca(No *no, int valor);

/** numero de nós da árvore */
int size(No * no);

/** altura da árvore */
int altura(No *no);


/** ordena sequencia usando árvore */
void ordena(Sequencia *s);

/** percorre os nós da árvore em ordem, salvando
na sequencia */
void emOrdem(No * no, Sequencia * s);










#endif