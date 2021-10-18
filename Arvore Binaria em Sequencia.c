#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct SEQUENCIA
{
    int v[1000];
    int top;
}sequencia;

typedef struct NO{
   int valor;
   struct NO *esq;
   struct NO *dir;
}no;

typedef struct TREE{
   no *raiz;
}tree;

sequencia* NewSequencia()
{
    sequencia *s = malloc(sizeof(sequencia));
    s->top = 0;
    return s;
}

no* NovoNo(int valor)
{
    no *n = malloc(sizeof(no));
    n->valor = valor;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

tree* NovaTree()
{
    tree *t = malloc(sizeof(tree));
    t->raiz = NULL;
    return t;
}

void AddSequencia(sequencia *s, int valor)
{
    s->v[s->top++] = valor;
}

void ShowSequencia(sequencia *s)
{
    for(int i = 0; i < s->top; i++)
    {
        printf("%d ", s->v[i]);
        if (i < s->top-1)
            printf(",");
    }
    printf("\n");
}

sequencia* RandSequencia(int size, int max, unsigned int randType)
{
    sequencia *s = NewSequencia();
    srand(randType);
    for (int i = 0; i < size; i++)
    {
        int rngValue = rand() % max;
        AddSequencia(s, rngValue);
    }
    return s;
}

void AddNode(no *n, int valor)
{
    if (valor < n->valor)
    {
        if (n->esq == NULL)
            n->esq = NovoNo(valor);
        else
            AddNode(n->esq, valor);
    }
    else //if (valor > no->valor)
    {
        if (n->dir == NULL)
            n->dir = NovoNo(valor);
        else
            AddNode(n->dir, valor);
    }
    //else return;
}

void AddTree(tree *t, int valor)
{
    if(t->raiz == NULL)
        t->raiz = NovoNo(valor);
    else
        AddNode(t->raiz, valor);
}

void ImprimeXMLRec(no *n, FILE* f)
{
    fprintf(f, "%d\n", n->valor);
    if (n->esq != NULL)
    {
        fprintf(f, "<ESQ>\n");
        ImprimeXMLRec(n->esq, f);
        fprintf(f, "</ESQ>\n");
    }
    if (n->dir != NULL)
    {
        fprintf(f, "<DIR>\n");
        ImprimeXMLRec(n->dir, f);
        fprintf(f, "</DIR>\n");
    }
}

void ImprimeXML(no *n, char *fName)
{
    FILE *f = fopen(fName, "w");

    fprintf(f, "<NO>\n");
	if(n != NULL)
		ImprimeXMLRec(n,f);
    fprintf(f, "</NO>\n");

    fclose(f);
}

int Busca(no *n, int valor)
{
    if (n != NULL)
    {
        if (valor < n->valor)
            return Busca(n->esq, valor);
        else if (valor > n->valor)
            return Busca(n->dir, valor);
        else
            return 1;
    }
    return 0;
}

int Size(no *n)
{
    if (n != NULL)
        return 1+Size(n->dir)+Size(n->esq);
    return 0;
}

int Altura(no *n)
{
    if (n == NULL)
        return 0;

    int hEsq = Altura(n->esq);
    int hDir = Altura(n->dir);

    return (hEsq < hDir ? hDir : hEsq)+1;
}

void EmOrdem(no *n, sequencia *s)
{
    if (n != NULL)
    {
        EmOrdem(n->esq, s);
        AddSequencia(s, n->valor);
        EmOrdem(n->dir, s);
    }
}

void OrdenaSeq(sequencia *s)
{
    tree *t = NovaTree();

    for (int i = 0; i < s->top; i++)
        AddTree(t, s->v[i]);
    s->top = 0;

    EmOrdem(t->raiz, s);
}

int MinTree(no *n)
{
    if(n != NULL)
    {
        if (n->esq == NULL)
            return n->valor;
        else
            return MinTree(n->esq);
    }
    return 0;
}

int MaxTree(no *n)
{
    if(n != NULL)
    {
        if (n->dir == NULL)
            return n->valor;
        else
            return MaxTree(n->dir);
    }
    return 0;
}

int main(void)
{
    sequencia *s = RandSequencia(5, 10, 22);
    ShowSequencia(s);

    tree *t = NovaTree();
    for (int i = 0; i < s->top; i++)
        AddTree(t, s->v[i]);

    printf("%d / %d / %d\n", Busca(t->raiz, 5), Size(t->raiz), Altura(t->raiz));

    OrdenaSeq(s);
    ShowSequencia(s);

    //ImprimeXML(t->raiz, "XML.xml");

    printf("%d / %d", MinTree(t->raiz), MaxTree(t->raiz));
}
