/*
    Aluno: Luiz Fabião Giserman
    Problema: Barbershop - Little Book of Semaphores
    Disciplina - Sistemas Operacionais
    Unviersidade Federal do Rio de Janeiro
*/


#include "barber.h"

int num_cadeiras_vagas     = TOTAL_CADEIRAS;
unsigned char barber_busy  = 0;

pthread_mutex_t mutex              = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  barberAvailable    = PTHREAD_COND_INITIALIZER;
pthread_cond_t  cutHair            = PTHREAD_COND_INITIALIZER;

void *Costumer(void *args)
{
    pthread_mutex_lock(&mutex);

    if (num_cadeiras_vagas > 0) /*se eh possivel esperar ou pedir um corte*/
    {
        if (barber_busy && num_cadeiras_vagas <= TOTAL_CADEIRAS)
        {
            Sentar(); /*decrementa o num_cadeiras_vagas*/
            getHairCut(1); 
        }
        /*Nesse caso, o barbeiro esta livre e nao tem ninguem esperando.
        Por isso, o cliente pode ir diretamente ao barbeiro, sem se sentar.*/
        else if (num_cadeiras_vagas == TOTAL_CADEIRAS)
        {
            printf ("Cheguei. Salão vazio e barbeiro livre. Vou acorda-lo\n");
            getHairCut(0);
        }    

        pthread_mutex_unlock(&mutex);
    }
    else /*se o salao estiver cheio*/
    {
        pthread_mutex_unlock(&mutex);
        balk();
    }
    
    return NULL;
}

void *Barber(void *args)
{
    pthread_mutex_lock(&mutex);

    while (1)
    {
        /*Esperar um cliente querer cortar (acordar o barbeiro*/
        pthread_cond_wait(&cutHair, &mutex); 
        Cortar();
        /*Dizer que esta livre quando terminar de cortar*/
        barber_busy = 0;
        /*Sinalizar que esta livre, acordando as threads de clientes sentados*/
        pthread_cond_signal(&barberAvailable);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
    
}

/* 
void Cortar
return : void
descricao: Funcao simples de cortar o cabelo.
*/
void Cortar()
{
    printf ("Cortando o cabelo\n");
    sleep(1);
    printf ("Cabelo cortado\n");
}

/* 
void sentar
return : void
descricao: com o lock obtido pela funcao que a chama,
senta o cliente, diminui o numero de cadeiras e espera pelo
sinal do barbeiro.
*/
void Sentar()
{
    printf ("Sentando\n");
    num_cadeiras_vagas--;
    pthread_cond_wait(&barberAvailable, &mutex);

}

/* 
void getHairCut
params: int cat : 0 -> Cliente Sentou antes; 1 -> Cliente entrou e foi direto cortar o cabelo.
return : void
descricao: Checa a variavel (booleana (int)) para ver seo barbeiro esta livre.
Se o cliente estava sentado (cat 0), é preciso aumentar o num_cadeiras_vagas.
o primeiro cliente sinaliza pega a proxima vaga do barbeiro pela booleana e sinaliza que esta disponivel para cortar.
*/
void getHairCut(int cat)
{
    while (barber_busy);
    if(cat)
    {
        num_cadeiras_vagas++;
    }
    barber_busy = 1;
    pthread_cond_signal(&cutHair);
}

void balk ()
{
    printf ("Salão cheio. Vou sair\n");
    return;
}

int main ()
{
    pthread_t cli[10], barb;
    srand(time(NULL));
    
    pthread_create(&barb, NULL, Barber, NULL);
    for (int i = 0; i < 10; i++)
    {
        pthread_create(&cli[i], NULL, Costumer, NULL);
    }

    pthread_join(barb, NULL);

    return 0;

}