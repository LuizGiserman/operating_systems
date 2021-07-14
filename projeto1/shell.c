#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h> /*access*/
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h> 

#define BUFFER_SIZE     20
#define OK              0
#define PATH_SIZE       5
#define ERRO_ARGUMENTO  1
#define EOS             '\0'
int state = 0;

void SignalHandler (int sigNumber)
{

    if (sigNumber == SIGUSR1)
    {
        printf ("Aperte ENTER para reiniciar o shell\n");
        state = -1;
    }
}

/*unsigned int verificaComando
 verifica se o comando pedido existe no path
*/
unsigned int verificaComando(char *path, char *executable)
{

    char * completePath = malloc (sizeof(char) * (PATH_SIZE + BUFFER_SIZE));
  
    strcpy(completePath, path);
    strcat(completePath, executable);

    if ((access(completePath, F_OK) != -1) && (access(completePath, X_OK) != -1))
    {
        free(completePath);
		return OK;
	}
    
    free(completePath);
    printf ("Programa inexisteste. Tente novamente.\n\n");
    return 1;
}

int main ()
{
    char *validation;
    char command[BUFFER_SIZE + 1];
    char numArguments[BUFFER_SIZE + 1];
    int nArguments;
    char **arguments;
    char path[] = {"/bin/"};
    int i = 0;
    char *checkString = NULL;
    
    
    if (signal(SIGUSR1, SignalHandler) == SIG_ERR)
    {
        printf("Erro tratando SIGUSR1\n");
    }

    state = 0;
    while(1)
    {
        switch(state)
        {
            case 0: /*Pegando o nome do programa*/

                printf ("Qual comando quer executar?\n");
        
                fgets(command, BUFFER_SIZE + 1, stdin);
                /*Verificacao do fgets*/
                if ( (checkString = strchr(command, '\n')) != NULL)
                {
                    *checkString = EOS;
                }
                else
                {
                    printf ("\nErro : Programa deve ter no max %d caracteres.\nTente novamente\n\n", BUFFER_SIZE);
                    scanf("%*[^\n]");scanf("%*c");//limpar o buffer
                    state--;
                }
                /*fim verificacao*/
                
                if (checkString != NULL)
                {
                    if(verificaComando(path, command) != OK)
                    {
                        state--;
                    }
                }
                checkString = NULL;

                break;

            case 1: /*Pegando o numero de argumentos*/
                printf("Quantos argumentos voce quer digitar?\n");

                fgets(numArguments, BUFFER_SIZE + 1, stdin);
                /*verificando o input*/
                 if ( (checkString = strchr(numArguments, '\n')) != NULL)
                {
                    *checkString = EOS;
                }
                else
                {
                    printf ("\nErro : Numero deve ter no max %d caracteres.\nTente novamente\n\n", BUFFER_SIZE);
                    exit(ERRO_ARGUMENTO);
                }
                nArguments = (int) strtol(numArguments, &validation, 10);
                if (*validation != EOS)
                {
                    printf ("Numero Invalido\nTerminando o programa...\n\n");
                    exit(ERRO_ARGUMENTO);
                }

                /*allocando o vetor de strings para os argumentos*/
                arguments = (char**) malloc(sizeof(char*) * nArguments + 1 + 1);
                arguments[0] = (char*) malloc(sizeof(char) * BUFFER_SIZE + 1);
                strcpy(arguments[0], command); 

                break;
            
            case 2: /*pegando os argumentos*/
                for (int index = 1; index <= nArguments; index++)
                {   
                    printf ("Insira o argumento %d\n", index);
                    arguments[index] = (char*) malloc(sizeof(char) * BUFFER_SIZE + 1);
                    fgets(arguments[index], BUFFER_SIZE + 1, stdin);
                    /*checa cada argumento (fgets)*/
                    if ( (checkString = strchr(arguments[index], '\n')) != NULL)
                    {
                        *checkString = EOS;
                    }
                    else
                    {
                        printf ("Argumento deve ter no max %d caracteres.\n", BUFFER_SIZE);
                        printf ("Programa terminando...\n");
                        exit(ERRO_ARGUMENTO);
                    }
                    /*fim da checagem*/

                    /*Aqui precisamos verificar essa variável para poder interromper
                      entre qualquer argumento*/
                    if (state == -1)
                    {
                        break;
                    }
                }
                arguments[nArguments+1] = NULL;
                break;
        }
        state++;

        /*Se passou por tudo, saia do loop*/
        if (state == 3)
        {
            break;
        }

    }
    
    /*A partir desse momento, nao aceitamos mais interrupcoes*/
    
    strcat(path, command);
    pid_t child_pid = fork();
	if (child_pid == 0) {
		execv(path, arguments);
		return 0;
	} 
	else {
		wait(NULL);

        /*Liberando a memoria allocada*/
        while (arguments[i] != NULL)
        {
            free(arguments[i]);
            i++;
        }
        free (arguments);

		printf("\nTarefa Concluida\n");
		return OK;
	}

}



