#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>


// Ejercicio 1. Implementa un programa en C que tenga como argumento un número entero. 
// Este programa debe crear un proceso hijo que se encargará de comprobar si dicho número 
// es un número par o impar e informará al usuario con un mensaje que se enviará por la salida estándar. 
// A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o no 
// usando igualmente la salida estándar.

int main(int argc, char** argv) {


    if(argc != 2){
        printf("mete un numero no ? ");
        exit(-1);
    } 

    pid_t id_proceso;

    int numero ;
    // check the difference between those two maderfaquers
    //numero = atoi(argv[1]);
    numero = strtol(argv[1],NULL,10);
    printf("numero-->%d \n", numero);
    id_proceso = fork();

    if ( id_proceso < 0){
        perror("Error en el fork \n");
        exit(EXIT_FAILURE);
    }else if ( id_proceso == 0){ // ---> proceso hijo
        printf("hola que tal, soy el proceso hijo, mi pid es--> %d, el de mi padre es -->%d \n", getpid(), getppid());
        printf("Pidt a pelo de hijo%d-> \n", id_proceso );
        if (numero % 2 == 0){
            printf("pues el numero es par \n");
            exit(EXIT_SUCCESS);

        }else{
            printf("pues el numero es impar\n");
            exit(EXIT_SUCCESS);
        }

    }else{ //---> proceso padre
        printf("hola que tal, soy el proceso padre, mi pid es--> %d, el de mi padre es -->%d \n", getpid(), getppid());
        printf("Pidt a pelo de padre%d-> \n", id_proceso );

            if (numero % 4 == 0){
            printf("pues el numero es divisible por 4\n");
            exit(EXIT_SUCCESS);

        }else{
            printf("pues el numero no es divisible por 4\n");
            exit(EXIT_SUCCESS);

        }

    }



}