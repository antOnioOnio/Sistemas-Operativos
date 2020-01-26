
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se
    identificará en la salida estándar, mostrando un mensaje del tipo Soy el hijo PID. 
    El proceso padre simplemente tendrá que esperar la finalización de todos sus hijos
    y cada vez que detecte la finalización de uno de sus hijos escribirá en la salida 
    estándar un mensaje del tipo:
    Acaba de finalizar mi hijo con <PID>
    Sólo me quedan <NUM_HIJOS> hijos vivos
*/


int main(int argc, char * argv[]){
    int estado;
    pid_t pid, hijo;
	int nprocs = 5; 


    // creamos los 5 hijos
    for ( int i = 0 ; i < nprocs ; i++){
        if ((hijo=fork()) == -1) {
			fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
			exit(-1);
			}
		if (hijo == 0) {// el padre ejecuta break y no vuelve al bucle
            printf("(HIJO) PID = %i\n", getpid());
            exit(0);
        } 

    }

    // wait for them 
    for ( int i = 5 ; i>0 ; i--){
        pid = wait(&estado);

        printf("(PAPA) mi hijo --> %d ya ha terminado \n", pid);
        printf("(PAPA) me quedan %d \n", i);
    }
    
}

