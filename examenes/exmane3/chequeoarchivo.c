#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <math.h>
#include <dirent.h>


int main(int argc, char *argv[]){

    if( argc != 2){
        printf("Correcto uso del programa--> ./Maestro <ArchivoRegular>\n");
        exit(-1);
    }

    char *archivoregular = argv[1];
    int fd;
    struct stat atributos;

    if (stat(archivoregular, &atributos)<0){
        perror("\nError en lstat");
        exit(EXIT_FAILURE);
    }

    if ( !S_ISREG(atributos.st_mode)){
        printf("%s no es un archivo regular\n", archivoregular);
        exit(-1);
    }
    long int tamanioInicial = atributos.st_size;
    long int tamanioNuevo, diferencia;

   // printf("tamanio inicial %ld \n", tamanioInicial);

    while (1){

        if (stat(archivoregular, &atributos)<0){
            perror("\nError en lstat");
            exit(EXIT_FAILURE);
        }
        tamanioNuevo = atributos.st_size;
        if ( tamanioNuevo != tamanioInicial){
            if ( tamanioNuevo != 0){

                printf("%10ld\n",tamanioNuevo - tamanioInicial );
                tamanioInicial = tamanioNuevo;
            }
           // printf( "tamaño inicial es %ld y tamaño actual es %ld \n" , tamanioInicial , tamanioNuevo);
        }
    }
    return 0;
}
    