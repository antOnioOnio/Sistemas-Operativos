// 2.[7 pts] Implementa un progrma en C llamado maestro que tenga como argumentos
// nombres de directorios (d1,d2 ... dn)y que realiza las siguientes acciones:

// a)Por cada argumento (al menos debe haber uno), crea un proceso hijo que se encargará
// de ejecutar el programa contar.

// b) Cada hijo usará como argumento del programa contar que va a ejecutar el nombre del
// directorio por el cual se creo y el nombre de un archivo que se llamra <pid del hijo>.Directorios.

// c)El proceso padre crea un unico archivo de tipo cauce (con o sin nombre) que le
// comunicará con sus procesos hijos. De este cauce leerá números que recibira de sus
// hijos (que habran cambiado su salida estandar por la escritura en dicho cauce).
// El padre, ademas, ira sumando los numeros leidos.

// d)Cuando todos los procesos hijos finalicen, el proceso padre imprimira en pantalla el 
// siguiente mensaje "la suma de los bloques de todos los archivos de tipo directorio de todos 
// los directorios dados como argumentos es  <numero>" y finalizará.
// Los procesos hijos no deben quedar zombies.


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

    if( argc < 2){
        printf("Correcto uso del programa--> ./Maestro <directorio><directorio>...\n");
        exit(-1);
    }


    int estado;
    pid_t pid, hijo;
    int fd1[2];
    int numBytes, valLeido;
    char *parametros[4];
    parametros[0] = "./contar";
    parametros[1] = "salida.txt";
    parametros[3] = NULL;
    char dato[500];
    DIR *d;
    int tamanioTotal=0;

    pipe(fd1); 
    // creamos los  hijos

     for ( int i = 0 ; i <argc ; i++){

        if ((hijo=fork()) == -1) {
			fprintf(stderr, "Could not create child");
			exit(-1);
			}
		if (hijo == 0) {// el padre ejecuta break y no vuelve al bucle
            close(fd1[0]); // -----Cerramos el descriptor de lectura, solo vamos a escribir
            parametros[2] = argv[i+1];
            // printf("parametro 1--> %s ", parametros[2] );

            if((d=opendir(parametros[2])) == NULL){
                perror("error en opendir"),exit(-1);
            }
            dup2(fd1[1],STDOUT_FILENO); 
            execv("./contar", parametros);
            exit(-1);
        } 

     }


    close(fd1[1]);  // ---Cerramos el descriptor de escritura, solo vamos a leer
    dup2(fd1[0],STDIN_FILENO);


    while((read(fd1[0],&dato, sizeof(dato))) > 0){

        // printf("Val--> %s  \n", dato);
        tamanioTotal += atoi(dato);

    }
    close(fd1[0]);
    
    printf("tamanio total: \t %d \n", tamanioTotal);


 
    return 0;

}