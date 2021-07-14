/*
    Aluno: Luiz Fabião Giserman
    Problema: Search-Insert-Delete - Little Book of Semaphores
    Disciplina - Sistemas Operacionais
    Unviersidade Federal do Rio de Janeiro
*/

#include "sushi.h"

int num_lugares = 5;

pthread_mutex_t mutex              = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  vazio              = PTHREAD_COND_INITIALIZER;

void *Manager (void *args)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        
        /*Espera o restaurante ficar lotado*/
        while (num_lugares > 0)
        {
            /*so recebe o signal do cliente se o restaurante estiver lotado*/
            pthread_cond_wait(&vazio, &mutex);
        }

        /*Libera os lugares*/
        num_lugares = 5;
        printf ("Todos os 5 saiem do restaurante\n");
        
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *Cliente (void *args)
{
    int saiOuFica;

    /*Como podemos ter muitos clientes, damos o lock so para verificar
      e atualizar o contador de clientes no restaurante*/
    pthread_mutex_lock(&mutex);

    /*Se o restuarante nao esta lotado, o cliente senta*/ 
    if (num_lugares > 0)
    {
        Sentar();
    }
    pthread_mutex_unlock(&mutex);

    /*Damos esse lock separado para que dois clientes possam estar em estados diferentes
      da thread Cliente*/
    pthread_mutex_lock(&mutex);
    
    /*50% de chance de sair logo apos sentar e 50% de chance de ficar*/
    saiOuFica = rand() % 2;
    
    if (saiOuFica == 1 && num_lugares < 5 && num_lugares > 0)
    {
        Levantar();
    }
    else /*cliente fica ate que o manager peca para sair*/
    {   
        // pthread_cond_wait(&vazio, &mutex);
    }

    /*aqui damos o singal, mas existe uma verificacao no manager para atende-lo*/
    pthread_cond_signal(&vazio);

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void Sentar()
{

    num_lugares--;
    printf ("Sentando\n");
}

void Levantar()
{
    num_lugares++;
    printf ("Levantando\n");
    
}

int main ()
{
    pthread_t cli[100], manager;
    srand(time(NULL));
    
    pthread_create(&manager, NULL, Manager, NULL);
    for (int i = 0; i < 100; i++)
    {
        pthread_create(&cli[i], NULL, Cliente, NULL);
    }

    pthread_join(manager, NULL);

    return 0;

}
