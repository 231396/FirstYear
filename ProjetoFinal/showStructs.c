#include <stdio.h>
#include "sistema.h"

void ShowDisciplina(disciplina *d)
{
    printf("creditos: %d | pre: %s\ncod: %s | nome: %s\n",
           d->creditos, d->preText, d->codigo, d->nome);
}


void ShowTabela(tabela *t)
{
    printf("%d\n", t->tamanho);
    for (int i = 0; i < t->tamanho; i++)
        ShowDisciplina(&t->ds[i]);
}

void ShowUsuario(usuario *u)
{
    printf("RA %d | Nome %s | Login %s | Senha %s\n", u->RA, u->nome, u->login, u->senha);
}

void ShowMateria(materia *m)
{
    printf("Sem %d | N %f | F %f \n",m->semestre, m->nota, m->faltas);
    ShowDisciplina(m->d);
}

void ShowCurso(curso *c)
{
    printf("RA %d | SemA %d | mT %d\n", c->RA, c->semestreAtual, c->mTamanho);
    for(int i = 0; i < c->mTamanho; i++)
        ShowMateria(&c->ms[i]);
}

void ShowAluno(aluno *a)
{
    ShowUsuario(a->u);
    ShowCurso(a->c);
}

void ShowGrupo(grupo *g)
{
    printf("NumAlunos %d\n", g->tamanho);
    for(int i = 0; i < g->tamanho; i++)
    {
        ShowAluno(&g->as[i]);
        PrintLine();
    }
}
