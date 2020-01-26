/*

    [2 puntos] Crear un cauce, un proceso hijo y un proceso padre que se ejecuten de forma concurrente.
    El metodo de comunicacion entre ellos es de libre elccion. El que elijas tendrá que ejecutar el programa 
    anterior tam_quijote.c

    [1 punto] El otro proceso leera del cauce un entero que indica el tamaño del fichero <quijote.txt> que 
    <tam_quijote.x> envia por la salida estandar. Y tendrá que guardar ese mismo numero en un fichero llamado 
    resultado.txt. siguiendo este formato

        Tamaño quijote.xt: NUMERO

    [2 puntos] Usa otro cauce para ejecutar el cpomando siguiente para contar el numero de palabras del quijote.

        wc -w quijote/quijote.txt

    [2 puntos] Guarda el numero de palabras justo al final del archivo anterior añadiendo dos saltos de linea a resultado.txt
    y poniendo Palabras: NUMERO

*/

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

    if (argc != 2)
    {
        printf("Uso correcto: \t ./quijote <directorio> \n");
        exit(-1);
    }

    int fd[2];
    int fd1[2];
    int fdArchivo;
    pid_t pid, pid2;
    char datoLeido[400];
    char line2[200];
    char line3[200];
    char line[300];

    char *parametros[3];
    parametros[0] = "./tam_quijote";
    parametros[1] = argv[1];
    parametros[2] = NULL;
    int tamanioTotal;
    int estado, estado2;

    int savedSTDOUT = dup(1);
    int savedSTDIN = dup(2);

    
    //---------------------------------primer ejercicio--------------------------------------------------
    pipe(fd);
    pipe(fd1);


    if ((pid = fork()) == -1)
    {
        fprintf(stderr, " Could not create a child");
        exit(-1);
    }


    if ((pid2 = fork()) == -1)
    {
        fprintf(stderr, " Could not create a child");
        exit(-1);
    }

    

    if (pid == 0)
    {

        close(fd[0]); //                    --> cerramos lectura

        dup2(fd[1], STDOUT_FILENO); //     --> duplicamos salida en la salida estandar
        close(fd[1]);

        execv("./tam_quijote", parametros); //-> ejecutamos programa
        exit(-1);

    }else if (pid2 == 0) {

        close(fd1[0]); //                    --> cerramos lectura

        dup2(fd1[1], STDOUT_FILENO); //     --> duplicamos salida en la salida estandar
        close(fd1[1]);

        execlp("wc", "wc", "-w", "quijote.txt", NULL);
        exit(-1);

    } else  {

        close(fd[1]); //               -->Cerramos el descriptor de escritura, solo vamos a leer

        dup2(fd[0], STDIN_FILENO); //     -->Duplicamos en entrada estandar
        

        // leemos nuestro numero 
        while ((read(fd[0], &datoLeido, sizeof(datoLeido))) > 0)
        {

            tamanioTotal += atoi(datoLeido);
        }

        close(fd[0]);


        // abrimos o creamos nuestro archivo
        if ((fdArchivo = open("resultado.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR)) < 0)
        {
            printf("\nError %d en open", errno);
            perror("\nError en open");
            exit(-1);
        }


        // escribimos en nuestro archivo


        sprintf(line, "TAMAÑO ARCHIVO %d\n", tamanioTotal); 
        char a;
        int i = 1;
        int contador = 0;
        while (i != -1)
        {
            a = line[contador];
            if (a != 10) // nueva linea
            {
                if (write(fdArchivo, &a, sizeof(a)) != sizeof(a))
                {
                    printf("\nError %d en write\n", errno);
                    perror("\nError en write");
                    exit(-1);
                }
            }
            else
            {
                i = -1;
            }
            contador++;
        }

        

        // ceramos nuestro descriptor de escritura del wc
        close(fd1[1]);


        // escribimos en nuestro archivo abierto dos saltos de linea
        
        write(fdArchivo, "\n\nPalabras: ", sizeof("\n\nPalabras: ")-1);


        // leemos nuestro datos
        int stop = 0; //----> Lo usamos para parar en el bucle
        char b;

    
        // vamos metiendo char a char hasta que nos de un espacio, cuando nos de un espacio cerramos el tinglao
        while ((read(fd1[0], &b, sizeof(b))) > 0  && stop != 1 )
        {   
                if ( b != 32 ){ // caracter 32 es el espacio   
                    write(fdArchivo, &b, sizeof(b));
                }else {
                    stop = 1;
                    close(fdArchivo);
                }
        
        }

        //Cerramos el cauce al completo
        close(fd1[0]);


    }


    return 0;

}