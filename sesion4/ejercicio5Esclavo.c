#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <math.h>


int esPrimo( int numero){

    int limite =sqrt(numero);
    int numeroEsPrimo = 1;
    
 
    for (int i = 2; i <= limite && numeroEsPrimo; i++){

        if(numero%i == 0){
            numeroEsPrimo = 0;
        }

    }

    return numeroEsPrimo;


}


int main(int argc, char *argv[]){

    int inicio, fin;

    if( argc != 3){
        printf("asi no se usa este programa...\n");
        exit(-1);
    }

    inicio = atoi(argv[1]);
    fin = atoi(argv[2]);

    for(int i = inicio ; i <fin ; i++){
        // printf("pues \t  i-->%d ", i );
        if (esPrimo(i)){
        
            write(STDOUT_FILENO, &i, sizeof(int));
            // printf("PRIMO---> %d \n", i)
        }

    }

    return 0;

}