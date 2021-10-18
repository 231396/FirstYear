#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void MemoryError(void* v)
{
    if (v == NULL){
        printf("Erro ao Allocar");
        exit(0);
    }
}

FILE* SafeFile(const char *fName, const char *mode)
{
    FILE *f = fopen(fName, mode);
    if (f == NULL){
        printf("Erro de Leitura do Arquivo %s", fName);
        exit(1);
    }
    return f;
}

void ErrorAluno()
{
    printf("Erro ao tentar juntar aluno, provavel erro nos arquivos");
    exit(2);
}

char* fgets_NoNewLine(int buffer)
{
    char *str, *temp;
    str = NewString(buffer);
    fgets(str, buffer, stdin);
    int tam = strlen(str);
    temp = NewString(tam);
    for (int i = 0; i < tam; i++)
        temp[i] = str[i];
    temp[tam-1] = '\0';
    free(str);
    return temp;
}

void PrintLine()
{
    printf("\n");
}

void PrintFree(char *str)
{
    printf("%s", str);
    free(str);
}

char* NewString(int tamanho)
{
    char *str = (char*) malloc(sizeof(char) * tamanho);
    MemoryError(str);
    return str;
}

char* CpyString(char *texto)
{
    int tam = strlen(texto)+1;
    char *str = NewString(tam);
    return strncpy(str, texto, tam);
}

// Tabela
void FreeTabela(tabela *t)
{
    if (t != NULL)
    {
        for (int i = 0; i < t->tamanho; i++)
        {
            free(t->ds[i].codigo);
            free(t->ds[i].nome);
            free(t->ds[i].preText);
        }
        free(t->ds);
        free(t);
    }
}

void FreeGrupo(grupo *g)
{
    if (g != NULL)
    {
        for (int i = 0; i < g->tamanho; i++)
        {
            free(g->as[i].u->nome);
            free(g->as[i].u->login);
            free(g->as[i].u->senha);
            free(g->as[i].u);
            free(g->as[i].c->ms);
            free(g->as[i].c);
        }
        free(g->as);
        free(g);
    }
}

tabela* NewTabela(int tamanho)
{
    tabela *t = malloc(sizeof(tabela));
    t->ds = malloc(sizeof(disciplina) * tamanho);
    t->tamanho = tamanho;
    return t;
}

grupo* NewGrupo(int tamanho, tabela *t)
{
    grupo *g = malloc(sizeof(grupo));
    MemoryError(g);
    g->as = NewAlunos(tamanho);
    g->tamanho = tamanho;
    g->t = t;
    return g;
}

materia* NewMaterias(int tamanho)
{
    materia *m = malloc(sizeof(materia) * tamanho);
    MemoryError(m);
    return m;
}

usuario* NewUsuarios(int tamanho)
{
    usuario *u = malloc(sizeof(usuario) * tamanho);
    MemoryError(u);
    return u;
}

curso* NewCursos(int tamanho)
{
    curso *c = malloc(sizeof(curso) * tamanho);
    MemoryError(c);
    return c;
}

aluno* NewAlunos(int tamanho)
{
    aluno *a = malloc(sizeof(aluno) * tamanho);
    MemoryError(a);
    return a;
}

aluno* NewAluno_Cadastro()
{
    aluno *a = NewAlunos(1);
    a->u = NewUsuarios(1);
    a->c = NewCursos(1);
    a->c->semestreAtual = 0;
    a->c->mTamanho = 0;
    a->c->ms = NULL;
    return a;
}
