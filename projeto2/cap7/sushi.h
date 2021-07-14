/*
    Aluno: Luiz Fabião Giserman
    Problema: Search-Insert-Delete - Little Book of Semaphores
    Disciplina - Sistemas Operacionais
    Unviersidade Federal do Rio de Janeiro
*/

#ifndef _SUSHI_
#define _SUSHI_

#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


void *Cliente(void *args);
void *Manager(void *args);

void Sentar();
void Levantar();
#endif
