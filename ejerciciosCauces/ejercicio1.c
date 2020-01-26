
// Crea un programa en C de tal forma que realice el siguiente cauce linux: ls -l | tail -N, 
// donde N será un número entero que se le pasa como entrada a dicho programa. El comando ls -l
// lista los ficheros/directorios y sus permisos del directorio actual; y tail -N muestra las
// últimas N líneas de una entrada. Como sabéis, el carácter barra vertical «|» realiza un pipe (cauce).


#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

 int main(int argc, char *argv[])
{
    int FD1[2];
    int FD2[2];
    pid_t pid_A, pid_B;
    char parametroNumero[50];


    // se van a crear dos cauces, enlazando entradas de uno con salidas de otro
    if( argc < 2){
        printf("Correcto uso del programa--> ./Ejercicio1 <Numero>\n");
        exit(-1);
    }
   
    // creamos primer pipe 
    pipe(FD1);

    if( !(pid_A = fork()) ) {

        close(FD1[0]); // Cerramos descriptor de lectura, solo vamos a escribir
 
        dup2(FD1[1], STDOUT_FILENO);
        close(FD1[1]); //No pasarlo dos veces

        execlp("ls","ls","-l",NULL);
    }

    close(FD1[1]); // Cerramos descriptor de escritura, ya hemos enviado

    pipe(FD2); // Creamos segundo cauce

    if( !(pid_B = fork()) ) {
        // concatenamos nuestro argumento
        sprintf(parametroNumero, "-%s", argv[1]);

        close(FD2[0]); //Cerramos descriptor de lectura, solo vamos a escribir

        // enlazamos salida de uno con entrada
        dup2(FD1[0], STDIN_FILENO );
        close(FD1[0]); //No pasarlo dos veces

        dup2(FD2[1], STDOUT_FILENO);
        close(FD2[1]); //No pasarlo dos veces

        execlp("tail","tail", parametroNumero, NULL);
   }
   // cerramos descriptores
    close(FD1[0]); 
    close(FD2[1]); 


    dup2(FD2[0], STDIN_FILENO);
    close(FD2[0]); //No pasarlo dos veces

    execlp("cut","cut","-c", "1-10", NULL);

    close(FD2[0]); // B-read not needed anymore



   return 0;
}