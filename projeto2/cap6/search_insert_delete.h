/*
    Aluno: Luiz Fabião Giserman
    Problema: Search-Insert-Delete - Little Book of Semaphores
    Disciplina - Sistemas Operacionais
    Unviersidade Federal do Rio de Janeiro
*/

#ifndef _SEARCH_INSERT_DELETE_
#define _SEARCH_INSERT_DELETE_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *Inserter(void *args);
void *Deleter(void *args);
void *Searcher(void *args);

void Insert();
void Delete();
void Search();

#endif
