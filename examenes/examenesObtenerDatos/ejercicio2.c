#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <math.h>
#include <dirent.h>
#include <limits.h>


/**
 * Crea un progra,a en C llamado ejercicio2.c que realice lo siguiente
 * 
 *  a) [2 puntos] Crear un cauce, un proceso hijo y un proceso padre que se ejecuten de forma concurrente. 
 *      El metodo de comunicacion entre ellos es de libre eleccion, puede ser de hijo a padre o al reves. El que elijas 
 *      tendra que eejecutar el programa anterior ejercicio1.
 *  
 *  b)  El otro proceso leera del cauce las cadenas de caracteres que ejercicio1 ha enviado a la salida estandar
 *      una a una
 * 
 *  c) Utiliza en ese otro proceso la siguiente funcion <Obtener datos) que extrae de la cadena los tres datos que nos 
 *      interesan al almacenandolo en un array de cadena de caracteres:
 *      El nombre del archivo, sus permisos y su tamaño
 * 
 * 
 * */

char *nombres;
char *permisos;
char *tamanios;

void obtener_datos(char* buff){
    char bufferFinal[1024];
    
    char delim[] = "\n";
    char delim2[] = "|";

    char *ptr = strtok(buff, delim);

    while ( ptr !=NULL){
        char line[100];
        sprintf(line, ptr);
        printf("%s\n", line);

        ptr = strtok(NULL, delim);
    
        char *t = strtok(line, delim2);
        char line2[50];
        sprintf(line2, t);

        printf("\t%s\n", line2);

        while(t != NULL){
            t = strtok(NULL, delim2);
            sprintf(line2, t);
            printf("\t%s\n", line2);

        }



    } 


    // printf("FINAL BUFF\n%s\n", bufferFinal);
    
}



int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Uso correcto: \t ./quijote <directorio> \n");
        exit(-1);
    }


    int fd[2];
    int fdArchivo;
    pid_t pid, pid2;
    char line[300];
    char buff[1024];
    char letra;

    char *parametros[3];
    parametros[0] = "./ejercicio1";
    parametros[1] = argv[1];
    parametros[2] = NULL;

    //---------------------------------primer ejercicio--------------------------------------------------
    pipe(fd);

    if ((pid = fork()) == -1)
    {
        fprintf(stderr, " Could not create a child");
        exit(-1);
    }


    if (pid == 0)
    {

        close(fd[0]); //                    --> cerramos lectura

        dup2(fd[1], STDOUT_FILENO); //     --> duplicamos salida en la salida estandar
        close(fd[1]);

        execv("./ejercicio1", parametros); //-> ejecutamos programa
        exit(-1);


    } else  {

        close(fd[1]); //               -->Cerramos el descriptor de escritura, solo vamos a leer

        dup2(fd[0], STDIN_FILENO); //     -->Duplicamos en entrada estandar
        

        // leemos nuestro numero 
        sprintf(buff, "\n");
        while ((read(fd[0], &letra, sizeof(letra))) > 0)
        {
            if(letra != 10){    // salto de linea
                sprintf(buff + strlen(buff), "%c", letra);
            }else {
                sprintf(buff + strlen(buff), "\n");
            }
           
        }

        close(fd[0]);

        // printf("FICHERO\n%s", buff);

        obtener_datos(buff);


    }


    return 0;

}