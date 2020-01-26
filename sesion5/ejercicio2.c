#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

/**
 * Ejercicio 2. Escribe un programa en C llamado contador, tal que cada vez que reciba una
    señal que se pueda manejar, muestre por pantalla la señal y el número de veces que se ha
    recibido ese tipo de señal, y un mensaje inicial indicando las señales que no puede manejar. En el
    cuadro siguiente se muestra un ejemplo de ejecución del programa.
 */
static int SIGNAL[30];
void myHandler(int mysignal){
    
    if (mysignal != SIGKILL  && mysignal != SIGSTOP){
       SIGNAL[mysignal]++;

        printf("la signal %d se ha recibido %d veces \n", mysignal, SIGNAL[mysignal] );
    }else {
        printf("error\n");
    }

}

int main(int argc, char *argv[])
{

    struct sigaction sa;

    sa.sa_handler = myHandler;

    sigemptyset(&sa.sa_mask);

    // signal(SIGTERM, myHandler);

    sa.sa_flags = SA_RESTART;

    printf("the only signal I cant handle are %d, and %d \n", SIGKILL, SIGSTOP);

    for ( int i = 1 ; i < 29 ; i++){
        if ( i !=SIGKILL   && i != SIGSTOP){
             if (sigaction(i, &sa, NULL) < 0){
                printf("ERROR CON ---> %d \n",i );
                perror("\nError al intentar establecer el manejador de senal para SIGUSR1");
                exit(EXIT_FAILURE);
            }
        }
       
    }


    for ( ; ; ){}
}