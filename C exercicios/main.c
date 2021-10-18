#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct ARRAY{
    int *matriz;
    int inicio;
    int fim;
}array;

typedef struct DATFILE{
    char *nome;
    int *matriz;
    int tamanho;
}datFile;

typedef struct POPULA{
    int vezes;
    int *matriz;
    datFile *dat;
}popula;


void ThrError(const char* menssage)
{
    printf("%s\n", menssage);
    exit(5);
}

int cmpFunc (const void *x, const void *y)
{
    int p = *(int*)x;
    int q = *(int*)y;

    return (p - q);
}

void SalvaMatriz_File (char* nome_arquivo, int row, int col, int matriz[row][col])
{
    FILE* file = fopen (nome_arquivo, "w");

    if (file == NULL)
    {
        printf("\nErro ao criar arquivo \"%s\"\n", nome_arquivo);
        exit (1);
    }

    for (int i = 0; i < row; i++)
    {
        for (int e = 0; e < col; e++)
            fprintf(file, "%-5d", matriz[i][e]);
            //printf("%d", matriz[i][e]);
        fprintf(file, "\n");
        //printf("\n");
    }

    fclose (file);
}

int Dats_maiorElementos(int tamanho, datFile *dat)
{
    int temp = dat[0].tamanho;
    for (int i = 1; i < tamanho; i++)
        if (temp < dat[i].tamanho)
            temp = dat[i].tamanho;

    return temp;
}

void* Ler_DatFile (void* dat)
{
    datFile *df = (datFile*) dat;

    FILE* file = fopen (df->nome, "r");

    if (file == NULL)
    {
        printf("\nArquivo %s nao encontrado\n", df->nome);
        exit(1);
    }

    int contador = 0;
    while (!feof (file))
    {
        int temp;
        fscanf (file, "%d", &temp);
        contador++;
    }

    df->matriz = (int*) calloc(contador, sizeof(int));
    df->tamanho = contador;

    rewind(file);
    for(int i = 0; i < df->tamanho; i++)
        fscanf (file, "%d", &df->matriz[i]);

    fclose (file);

    //qsort(df->matriz, df->tamanho, sizeof(int), cmpFunc);

    pthread_exit("Thread Concluida");
    return NULL;
}

void* Popula_Matriz(void* pop)
{
    popula *p = (popula*)pop;

    for (int e = 0; e < p->vezes; e++)
        if (e < p->dat->tamanho)
            p->matriz[e] = p->dat->matriz[e];
        else
            p->matriz[e] = 0;

    pthread_exit("Thread Concluida");
    return NULL;
}

void* SortArray (void* v)
{
    int i;
    array arr = *(array*) v;

    int tamanho = arr.fim - arr.inicio;
    int tempVetor[tamanho];

    for (i = 0; i < tamanho; i++)
    {
        tempVetor[i] = arr.matriz[i+arr.inicio];
    }

    qsort(tempVetor, tamanho, sizeof(int), cmpFunc);

    for (i = 0; i < tamanho; i++)
    {
        arr.matriz[i+arr.inicio] = tempVetor[i];
    }

    pthread_exit("Thread Concluida");
    return NULL;
}

int main(int argc, char *argv[])
{
    //for (int z = 1; z <= 16; z++){
    clock_t inicio = clock();

    int i, e, k;
    int thrUsadas;

    int num_threads = atoi(argv[1]);

    if (num_threads % 2 != 0)
        num_threads++;

    int num_arquivos = argc-4;

    pthread_t threads[num_threads];
    datFile dat[num_arquivos];

    //Pega o nome dos arquivos .dat
    for (i = 2, e = 0; i < argc-2; i++, e++)
        dat[e].nome = argv[i];

    //Usaremos as thread para ler cada arquivo
    for(i = 0; i < num_arquivos;)
    {
        thrUsadas = 0;
        for (e = 0; e < num_threads && i != num_arquivos; e++)
        {
            if(pthread_create(&threads[e], NULL, Ler_DatFile, (void*)&dat[i++]))
                ThrError("Erro ao criar thread");
            thrUsadas++;
        }
        for (e = 0; e < thrUsadas; e++)
            if(pthread_join(threads[e], NULL))
                ThrError("Erro ao juntar thread");
    }

    //Descobre a o arquivo com maior numero de elementos
    int maior = Dats_maiorElementos(num_arquivos, dat);

    //Realiza um qsort usando varias threads
    array arr;
    for(i = 0; i < num_arquivos; i++)
    {
        arr.matriz = dat[i].matriz;

        int atual = 0;
        for (k = 0; atual < dat[i].tamanho; k++)
        {
            arr.inicio = 0;
            arr.fim = 0;
            atual = 50 * (k+1);
            if (atual > dat[i].tamanho)
                atual = dat[i].tamanho;

            while(arr.fim < dat[i].tamanho)
            {
                thrUsadas = 0;
                for (e = 0; e < num_threads && arr.fim != dat[i].tamanho; e++)
                {
                    arr.inicio += atual;
                    arr.fim = arr.inicio + atual;
                    if (arr.fim > dat[i].tamanho)
                        arr.fim = dat[i].tamanho;

                    if(pthread_create(&threads[e], NULL, SortArray, (void*)&arr))
                        ThrError("Erro ao criar thread");
                    thrUsadas++;
                }
                for (e = 0; e < thrUsadas; e++)
                    if(pthread_join(threads[e], NULL))
                        ThrError("Erro ao juntar thread");
            }

        }
    }

    //Matrix Final e a Preenche
    int matriz[num_arquivos][maior];

    popula p[num_arquivos];
    for (i = 0; i < num_arquivos; i++)
    {
        p[i].vezes = maior;
        p[i].matriz = matriz[i];
        p[i].dat = &dat[i];
    }

    for (i = 0; i < num_arquivos;)
    {
        thrUsadas = 0;
        for (e = 0; e < num_threads && i != num_arquivos; e++)
        {
            if(pthread_create(&threads[e], NULL, Popula_Matriz, (void*)&p[i++]))
                ThrError("Erro ao criar thread");
            thrUsadas++;
        }
        for (e = 0; e < thrUsadas; e++)
            if(pthread_join(threads[e], NULL))
                ThrError("Erro ao juntar thread");
    }

    SalvaMatriz_File(argv[argc-1], num_arquivos, maior, matriz);

    clock_t fim = clock();
    printf("Tempo de Execucao de %2d threads: %lf\n", num_threads, (double)(fim - inicio) / CLOCKS_PER_SEC);
    //}
}
