// Ejercicio 5. Implementa una modificación sobre el anterior programa en la 
// que el proceso padre espera primero a los hijos creados en orden impar (1o,3o,5o) y 
// después a los hijos pares (2o y 4o).



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){

    int estado, hijos = 5;
    int PIDs[4];
    pid_t pid, hijo;
	int nprocs = 5; 


    // creamos los 5 hijos
    for ( int i = 0 ; i < nprocs+1 ; i++){
        if( (PIDs[i]=fork())<0) {
            perror("\nError en el fork" ) ;
            exit(-1);
        }
        if(PIDs[i]==0) {  //proceso hijo ejecutando el programa
            printf("Soy el hijo %d\n", getpid());
            exit(0);
        }

    }

    for(int i=0; i<5; i = i +2){ //proceso padre ejecutando el programa 
        waitpid(PIDs[i],&estado, 0);
        printf("Acaba de finalizar mi hijo con PID = %d y estado %d\n", PIDs[i], estado);
        printf("Solo me quedan %d hijos vivos, este es el %do hijo.\n", --hijos, i+1);
    }


    for(int i=1; i<4; i = i +2){ //proceso padre ejecutando el programa 
        waitpid(PIDs[i],&estado, 0 );
        printf("Acaba de finalizar mi hijo con PID = %d y estado %d\n", PIDs[i], estado);
        printf("Solo me quedan %d hijos vivos, este es el %do hijo.\n", --hijos, i+1);
    }


}