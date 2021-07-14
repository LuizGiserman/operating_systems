/*
    Aluno: Luiz Fabião Giserman
    Problema: Barbershop - Little Book of Semaphores
    Disciplina - Sistemas Operacionais
    Unviersidade Federal do Rio de Janeiro
*/

#ifndef _BARBER_
#define _BARBER_

#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define TOTAL_CADEIRAS      5

void *Costumer (void *args);
void *Barber (void *args);

void Cortar();
void Sentar();
void getHairCut(int cat);
void balk();

#endif
