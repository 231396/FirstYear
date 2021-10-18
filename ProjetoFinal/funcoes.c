#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

int FindCodigo_inTabela(tabela *t, char *cod)
{
    for (int i = 0; i < t->tamanho; i++)
        if (strncmp(t->ds[i].codigo, cod, codText) == 0)
            return i;
    return -1;
}

int FindCodigo_inCurso(curso *c, char *cod)
{
    for (int i = 0; i < c->mTamanho; i++)
        if (strncmp(c->ms[i].d->codigo, cod, codText) == 0)
            return i;
    return -1;
}

int FindRA_inCurso(curso *cs, int RA, int numCursos)
{
    for (int i = 0; i < numCursos; i++)
        if (cs[i].RA == RA)
            return i;
    return -1;
}

int FindRA_inGrupo(grupo *g, int RA)
{
    for (int i = 0; i < g->tamanho; i++)
        if (g->as[i].u->RA == RA)
            return i;
    return -1;
}

int FindLoginSenha_Index(grupo *g, char *lg, char *senha)
{
    for(int i = 0; i < g->tamanho; i++)
        if(strcmp(g->as[i].u->login, lg) == 0 &&
           strcmp(g->as[i].u->senha, senha) == 0)
               return i;
    return -1;
}

char* CodigoToNome(tabela *t, char* codDis)
{
    char *str;

    if (codDis[0] == '-')
        str = CpyString("Sem Requisitos");
    else if(codDis[0] == 'A' && codDis[1] == 'A')
    {
        if (codDis[2] == '2')
            str = CpyString("Autorizacao da Coordenadoria");
        else if (codDis[2] == '4')
        {
            char chr[] = "CP requerido ";
            char temp[] = {codDis[3], codDis[4]};
            strncat(chr, temp, 2);
            strncat(chr, "%", 1);
            str = CpyString(chr);
        }
    }
    else
        str = CpyString(t->ds[FindCodigo_inTabela(t, codDis)].nome);

    return str;
}

void JoinAlunos(grupo *g, curso *cs, usuario *us)
{
    int idx;
    for (int i = 0; i < g->tamanho; i++)
    {
        idx = FindRA_inCurso(cs, us[i].RA, g->tamanho);
        if (idx == -1)
            ErrorAluno();
        g->as[i].u = &us[i];
        g->as[i].c = &cs[idx];
    }
}

void AddNewAluno(grupo *g, aluno *a)
{
    g->tamanho++;
    g->as = realloc(g->as, sizeof(aluno) * g->tamanho);
    g->as[g->tamanho-1] = *a;
    ShowAluno(&g->as[g->tamanho-1]);
}

void AddMateriasNovas(curso *c, materia *ms, int size_ms)
{
    int tam = c->mTamanho + size_ms;
    c->ms = realloc(c->ms, sizeof(materia) * tam);

    for(int i = c->mTamanho; i < tam; i++)
        c->ms[i] = ms[i-c->mTamanho];

    c->mTamanho = tam;
}

char* PreText_PrintTratamento(tabela *t, char *preText)
{
    if (preText[0] == '-')
        return CodigoToNome(t, preText);

    char str[maxText*2] = "", temp[codText];
    int tam = strlen(preText);
    for (int i = 0, j = 0; i < tam; i++)
    {
        if (preText[i] == '&')
            strcat(str ," e ");
        else if (preText[i] == '/')
            strcat(str ," ou ");
        else if (preText[i] == '*')
            strcat(str ,"[parcial]");
        else if (j < 5)
        {
            temp[j++] = preText[i];
            if (j == 5)
            {
                j = 0;
                strcat(str, temp);
                strcat(str, " (");
                strcat(str, CodigoToNome(t, temp));
                strcat(str, ")");
            }
        }
    }
    return CpyString(str);
}

int VerifyDisciplina(float nota, float faltas)
{
    if (nota == -1 || faltas == -1)
        return 0; //Nao concluio

    //True = bom
    int n = nota >= notaMin;
    int f = faltas < faltasMax;
    int p = nota >= notaParcial;

    if (f)
    {
        if (n)
            return 2; //Aprovado
        else if (p)
            return 1; //Aprovado Parcialmete Nota
        else
            return -1; //Reprovado Nota
    }
    else
    {
        if (n || p)
            return -2; //Reprovado Falta
        else
            return -3; //Reprovado Nota e Falta
    }
}

int CumprePre(aluno *a, char *preText)
{
    if (preText[0] == '-')
        return 1;

    char cod[codText];
    int tam = strlen(preText);

    int cumpre = 0;
    for (int i = 0, j = 0, parcial = 0; i < tam; i++)
    {
        if (preText[i] == '&' && !cumpre)
            i += 5;
        else if (preText[i] == '/' && cumpre)
            break;
        else if (preText[i] == '*')
            parcial = 1;
        else if (j < 5)
        {
            cod[j++] = preText[i];
            if (j == 5)
            {
                int temp = FindCodigo_inCurso(a->c, cod);
                if (temp > -1)
                {
                    int situacao = VerifyDisciplina(a->c->ms[temp].nota, a->c->ms[temp].faltas);
                    cumpre = (situacao == 2 || (situacao == 1 && parcial)) ? 1 : 0;
                }
                j = parcial = 0;
            }
        }
    }

    return cumpre;
}

char* Situacao(float nota, float faltas)
{
    switch (VerifyDisciplina(nota, faltas))
    {
        case -3:
            return "Reprovado por Nota e Faltas";
        case -2:
            return "Reprovado por Faltas";
        case -1:
            return "Reprovado por Nota";
        case 0:
            return "Disciplina nao concluida";
        case 1:
            return "Aprovado Parcialmente por Nota e Falta";
        case 2:
            return "Aprovado por Nota e Falta";
        default:
            return "Erro ao indentificar";
    }
}

float CoeficienteRendimento(curso *c)
{
    float top = 0;
    int bot = 0;

    for(int i = 0; i < c->mTamanho; i++)
        if (c->ms[i].nota != -1 && c->ms[i].faltas != -1)
        {
            top += c->ms[i].nota * c->ms[i].d->creditos;
            bot += c->ms[i].d->creditos;
        }
    if(bot > 0)
        return top/bot;
    return 0;
}

int cmpFloat (const void *a, const void *b)
{
  float af = *(float*) a;
  float bf = *(float*) b;
  return (af < bf) - (af > bf);
}

int ClassificarAluno(grupo *g, float CR, int idx)
{
    float arr[g->tamanho];
    for (int i = 0; i < g->tamanho; i++)
        if (i == idx)
            arr[idx] = CR;
        else
            arr[i] = CoeficienteRendimento(g->as[i].c);

    qsort(arr, g->tamanho, sizeof(float), cmpFloat);

    int pos = -1;
    for (int i = 0; i < g->tamanho; i++)
        if(arr[i] == CR)
            pos = i;

    if(pos == -1)
       printf("Float Error\n");

    return pos;
}
