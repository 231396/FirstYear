#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

#define read "r"
#define write "w"

#define tabelaDis "Disciplinas.txt"
#define tabelaPre "Prerequisitos.txt"
#define txtAlunos "Alunos.txt"
#define txtAluDis "AlunosDisciplinas.txt"
#define txtHist "RAdoAluno.txt"

int GetNumLines_File(char *fName)
{
    FILE *f = SafeFile(fName, read);

    char c;
    int count = 0;
    while(fscanf(f,"%c", &c) != EOF)
    {
        if (c == lineSeparator)
            count++;
        if (ferror(f) || feof(f))
            break;
    }
    fclose(f);
    return count;
}

tabela* LerDisciplinas()
{
    int tam = GetNumLines_File(tabelaDis);

    FILE *f1 = SafeFile(tabelaDis, read);
    FILE *f2 = SafeFile(tabelaPre, read);

    int cred;
    char cod[codText], temp[maxText];

    tabela *t = NewTabela(tam);

    for (int i = 0; i < tam; i++)
    {
        fscanf(f1, "%[^;];", cod);
        fscanf(f1, "%d;", &cred);
        fscanf(f1, "%[^\n]\n", temp);
        t->ds[i].creditos = cred;
        t->ds[i].codigo = CpyString(cod);
        t->ds[i].nome = CpyString(temp);
    }
    fclose(f1);

    for (int i = 0; i < tam; i++)
    {
        fscanf(f2, "%[^;];", cod);
        fscanf(f2, "%[^\n]\n", temp);
        t->ds[FindCodigo_inTabela(t, cod)].preText = CpyString(temp);
    }
    fclose(f2);
    return t;
}

grupo* LerGrupo(tabela *t)
{
    FILE *f1 = SafeFile(txtAlunos, read);
    FILE *f2 = SafeFile(txtAluDis, read);

    int numAlunos;
    fscanf(f1, "%d\n", &numAlunos);

    int num;
    float numF;
    char temp[maxText];

    usuario *us = NewUsuarios(numAlunos);
    for (int i = 0; i < numAlunos; i++)
    {
        fscanf(f1,"%d;", &num);
        us[i].RA = num;
        fscanf(f1,"%[^;];", temp);
        us[i].nome = CpyString(temp);
        fscanf(f1,"%[^;];", temp);
        us[i].login = CpyString(temp);
        fscanf(f1,"%[^\n]\n", temp);
        us[i].senha = CpyString(temp);
        //printUsuario(&us[i]);
    }
    fclose(f1);

    curso *cs = NewCursos(numAlunos);
    for (int i = 0; i < numAlunos; i++)
    {
        fscanf(f2,"%d;", &num);
        cs[i].RA = num;
        fscanf(f2,"%d;", &num);
        cs[i].semestreAtual = num;
        fscanf(f2,"%d:\n", &num);
        cs[i].mTamanho = num;

        cs[i].ms = NewMaterias(cs[i].mTamanho);
        for (int e = 0; e < cs[i].mTamanho; e++)
        {
            fscanf(f2,"%[^;];", temp);
            cs[i].ms[e].d = &t->ds[FindCodigo_inTabela(t, temp)];
            fscanf(f2, "%d;", &num);
            cs[i].ms[e].semestre = num;
            fscanf(f2, "%f;", &numF);
            cs[i].ms[e].nota = numF;
            fscanf(f2, "%f\n", &numF);
            cs[i].ms[e].faltas = numF;
            //ShowMateria(&cs[i].ms[e]);
        }
    }
    fclose(f2);

    grupo *g = NewGrupo(numAlunos, t);
    JoinAlunos(g, cs, us);

    return g;
}

void SaveGrupo(grupo *g)
{
    FILE *f1 = SafeFile(txtAlunos, write);
    FILE *f2 = SafeFile(txtAluDis, write);

    fprintf(f1,"%d\n", g->tamanho);
    for(int i = 0; i < g->tamanho; i++)
    {
        fprintf(f1, "%d;%s;", g->as[i].u->RA, g->as[i].u->nome);
        fprintf(f1, "%s;%s\n", g->as[i].u->login, g->as[i].u->senha);
    }
    fclose(f1);

    for (int i = 0; i < g->tamanho; i++)
    {
        curso *c = g->as[i].c;
        fprintf(f2, "%d;%d;%d:\n", c->RA, c->semestreAtual, c->mTamanho);
        for (int e = 0; e < c->mTamanho; e++)
        {
            fprintf(f2, "%s;", c->ms[e].d->codigo);
            fprintf(f2, "%d;", c->ms[e].semestre);
            fprintf(f2, "%.2f;", c->ms[e].nota);
            fprintf(f2, "%.2f\n",c->ms[e].faltas);
        }
    }
    fclose(f2);
}

grupo* RelerGrupo(grupo *g)
{
    grupo *ng = LerGrupo(g->t);
    FreeGrupo(g);
    return ng;
}

void Historico(grupo *g, int index)
{
    FILE *f = SafeFile(txtHist, "w");

    fprintf(f,"Faculdade de Tecnologia – UNICAMP\n\n");
    fprintf(f,"Relatorio de Matricula\n\n");
    fprintf(f,"RA: %d\n", g->as[index].u->RA);
    fprintf(f,"Nome Completo: %s\n", g->as[index].u->nome);

    float cr = 0;
    if (g->as[index].c->mTamanho > 0)
        cr = CoeficienteRendimento(g->as[index].c);
    int clas = ClassificarAluno(g, cr, index)+1;

    fprintf(f,"Coeficiente de Rendimento: %.2f\n", cr);
    fprintf(f,"Classificacao do aluno na turma: %d de %d\n", clas, g->tamanho);

    fprintf(f,"%-15s%-10s%-15s%s\n", "Disciplina", "Nota", "Faltas(%)", "Situacao");
    curso *c = g->as[index].c;
    for (int i = 1; i <= c->semestreAtual; i++)
        for(int e = 0; e < c->mTamanho; e++)
            if(c->ms[e].semestre == i)
            {
                fprintf(f,"%-15s", c->ms[e].d->codigo);

                if (c->ms[e].nota >= 0)
                    fprintf(f,"%-10.1f", c->ms[e].nota);
                else fprintf(f,"%-10c", '-');

                if (c->ms[e].faltas >= 0)
                    fprintf(f,"%-15.1f", c->ms[e].faltas);
                else fprintf(f,"%-15c", '-');

                fprintf(f,"%s\n", Situacao(c->ms[e].nota, c->ms[e].faltas));
            }
    fclose(f);
    printf("Arquivo gerado com sucesso\n");
}
