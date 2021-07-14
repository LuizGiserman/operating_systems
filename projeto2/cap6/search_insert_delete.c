/*
    Aluno: Luiz Fabião Giserman
    Problema: Search-Insert-Delete - Little Book of Semaphores
    Disciplina - Sistemas Operacionais
    Unviersidade Federal do Rio de Janeiro
*/

#include "search_insert_delete.h"

int count_search = 0;
int count_insert = 0;

pthread_mutex_t mutex              = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  notSearching       = PTHREAD_COND_INITIALIZER;
pthread_cond_t  notInserting       = PTHREAD_COND_INITIALIZER;

void *Inserter(void *args)
{
    /*Como so podemos ter um insert por vez, damos o lock em tudo*/
    pthread_mutex_lock(&mutex);

    count_insert++;
    Insert();
    count_insert--;
    /*Avisa que nao esta mais inserindo*/
    pthread_cond_signal(&notInserting);

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *Deleter(void *args)
{
    /*Como so podemos ter um delete por vez, damos o lock em tudo*/
    pthread_mutex_lock (&mutex);
    /*So eh possivel deletar se nenhuma outra tarefa ocorrer, portanto, verificamos:*/
    while (count_insert > 0 || count_search > 0 )
    {   
        /*Esses dois whiles internos vao garantir cada condicao*/
        while (count_insert > 0)
        {
            pthread_cond_wait(&notInserting, &mutex);
        }
        while (count_search > 0)
        {
            pthread_cond_wait(&notSearching, &mutex);
        }
    }
    
    Delete();

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *Searcher(void *args)
{  
    /*Como podemos ter varios searchers de uma vez, damos lock apenas no contador*/
    pthread_mutex_lock(&mutex);
    count_search++;
    pthread_mutex_unlock(&mutex);
   
    Search();
    
    /*E aqui novamente*/
    pthread_mutex_lock(&mutex);
    count_search--;
    pthread_mutex_unlock(&mutex);
    
    pthread_cond_signal(&notSearching);
    
    return NULL;
}

/*Funcoes de uso da lista*/

void Insert()
{
    printf ("Inserting\n");
    sleep(1);
}

void Delete()
{
    printf ("Deleting\n");
    sleep(1);
}

void Search()
{
    printf ("Searching\n");
    sleep(1);
}

int main ()
{
    pthread_t ins, del, sear, s2, s3;
    for (int i =0; i < 100; i++)
    {
        pthread_create(&ins, NULL, Inserter, NULL);
        pthread_create(&sear, NULL, Searcher, NULL);
        pthread_create(&del, NULL, Deleter, NULL);
        pthread_create(&s2, NULL, Searcher, NULL);
        pthread_create(&s3, NULL, Searcher, NULL);

        pthread_join(ins, NULL);
        pthread_join(sear, NULL);
        pthread_join(del, NULL);
        pthread_join(s2, NULL);
        pthread_join(s3, NULL);
        
        printf ("Done %d\n", i+1);
    }

    
    
    return 0;
}