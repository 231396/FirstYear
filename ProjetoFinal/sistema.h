#ifndef SISTEMA_H
#define SISTEMA_H

#define sair 0
#define cadastrar 1
#define consultar 2
#define matricular 3
#define nota_falta 4
#define historico 5

#define codText  6
#define maxText 100

#define notaMin 5
#define faltasMax 25
#define notaParcial 3

#define maxCreditos 32
#define sairCod "XX000"
#define concluirCod "000XX"

#define lineSeparator '\n'

#define tela "\t%s\t\n"

//*xs = array / *x = single
typedef struct DISCIPLINA{
    int creditos;
    char *codigo, *nome;
    char *preText;
}disciplina;

typedef struct TABELA{
    disciplina *ds;
    int tamanho;
}tabela;

typedef struct MATERIA{
    disciplina *d;
    float nota, faltas;
    int semestre;
}materia;

typedef struct CURSO{
    int RA, semestreAtual, mTamanho;
    materia *ms;
}curso;

typedef struct USUARIO{
    int RA;
    char *nome, *login, *senha;
}usuario;

typedef struct ALUNO{
    usuario *u;
    curso *c;
}aluno;

typedef struct GRUPO{
    aluno *as; //array
    tabela *t;
    int tamanho;
}grupo;

// Base
//void Free(void **point);

void MemoryError(void *v);
FILE* SafeFile(const char *fName, const char *mode);
void ErrorAluno();

char* fgets_NoNewLine(int buffer);

void PrintLine();
void PrintFree(char *str);
char* NewString(int tamanho);
char* CpyString(char *texto);

void FreeTabela(tabela *t);
void FreeGrupo(grupo *g);

tabela* NewTabela(int tamanho);
grupo* NewGrupo(int tamanho, tabela *t);
materia* NewMaterias(int tamanho);
usuario* NewUsuarios(int tamanho);
curso* NewCursos(int tamanho);
aluno* NewAlunos(int tamanho);

aluno* NewAluno_Cadastro();

//Arquivos
int GetNumLines_File(char *fName);

tabela* LerDisciplinas();

grupo* LerGrupo(tabela *t);

void SaveGrupo(grupo *g);

grupo* RelerGrupo(grupo *g);

void Historico (grupo *g, int index);

//Funcoes
int FindCodigo_inTabela(tabela *t, char *cod);
int FindCodigo_inCurso(curso *c, char *cod);
int FindRA_inCurso (curso *cs, int RA, int numVezes);
int FindRA_inGrupo(grupo *g, int RA);

int FindLoginSenha_Index(grupo *g, char *lg, char *senha);

char* CodigoToNome(tabela *t, char* codDis);

void JoinAlunos(grupo *g, curso *cs, usuario *us);

void AddNewAluno(grupo *g, aluno *a);

void AddMateriasNovas(curso *c, materia *ms, int size_ms);

char* PreText_PrintTratamento(tabela *t, char *preText);

int VerifyDisciplina(float nota, float faltas);

int CumprePre(aluno *a, char *preText);

char* Situacao(float nota, float faltas);

float CoeficienteRendimento(curso *c);

int cmpFloat (const void *a, const void *b);

int ClassificarAluno(grupo *g, float CR, int idx);

//Show
void ShowDisciplina(disciplina *d);
void ShowTabela(tabela *t);
void ShowUsuario(usuario *u);
void ShowMateria(materia *m);
void ShowCurso(curso *c);
void ShowAluno(aluno *a);
void ShowGrupo(grupo *g);

#endif
