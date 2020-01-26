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


int main(int argc, char *argv[])
{

    int fd[2];
    pid_t pid, pid2;
    char result[400];
    char buff[10];
    char letra;
    int fdArchivo;

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

        execlp("cut", "cut", "-d", ";", "-f", "6", "titanic.csv", NULL); //-> ejecutamos programa
        exit(-1);


    } else  {

        close(fd[1]); //               -->Cerramos el descriptor de escritura, solo vamos a leer

        dup2(fd[0], STDIN_FILENO); //     -->Duplicamos en entrada estandar
        
        // leemos nuestro numero 
        sprintf(buff, "\n");
        int edad_mas_joven = 100;
        int edad_mas_viejo = 0;
        int edad_leida;
        int menores_cinco=0;
        double media;
        int total = 0; 
        int suma = 0;

        while ((read(fd[0], &letra, sizeof(letra))) > 0)
        {

            if(letra != 10){    // salto de lin ea
                sprintf(buff + strlen(buff), "%c", letra);

            }else {
                total++; 
                edad_leida = atoi(buff);
                suma = suma+edad_leida;

                if ( edad_leida > edad_mas_viejo){
                    edad_mas_viejo = edad_leida;
                }
                if ( edad_leida < edad_mas_joven){
                    edad_mas_joven = edad_leida;
                }
                if ( edad_leida <5){
                    menores_cinco++;
                }
                media = suma/ total;

                sprintf(buff, "");
 
            }
           
        }

        // abrimos o creamos nuestro archivo
        if ((fdArchivo = open("resultado_titanic.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR)) < 0)
        {
            printf("\nError %d en open", errno);
            perror("\nError en open");
            exit(-1);
        }

        sprintf(result, "Total de personas %d\n", total);
        sprintf(result + strlen(result), "Media Edad %f\n", media);
        sprintf(result + strlen(result),"Mas Joven %d\n", edad_mas_joven);
        sprintf(result + strlen(result),"Mas viejo %d\n", edad_mas_viejo);
        sprintf(result + strlen(result),"Menores de 5 años %d\n", menores_cinco);

        write(fdArchivo,result, strlen(result) );


        close(fd[0]);



    }


    return 0;

}