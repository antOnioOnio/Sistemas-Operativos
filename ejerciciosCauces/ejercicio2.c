// Crea un programa en C de tal forma que realice el siguiente cauce linux: ls -l | wc -l
// Y además, como ese cauce escribirá en consola el número de líneas, decir si ese número es par o impar.

#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

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
    int numbytes;
    char valorLeido[5];

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

        close(FD2[0]); //Cerramos descriptor de lectura, solo vamos a escribir

        // enlazamos salida de uno con entrada
        dup2(FD1[0], STDIN_FILENO );
        close(FD1[0]); //No pasarlo dos veces

        dup2(FD2[1], STDOUT_FILENO);
        close(FD2[1]); //No pasarlo dos veces

        execlp("wc","wc", "-l", NULL);
   }
   // cerramos descriptores
    close(FD1[0]); 
    close(FD2[1]); 


    dup2(FD2[0], STDIN_FILENO);
    int numeroLeido;
    while (  (numbytes= read(FD2[0], &valorLeido, sizeof(FD2))) > 0   ){

        numeroLeido = atoi(valorLeido);
        if ( numeroLeido %2 ==0 ){
            printf("Numero de lineas %d y este valor es par \n", numeroLeido);
        }else {
            printf("Numero de lineas %d y este valor es impar \n", numeroLeido);
        }

    }
    // close(FD2[0]); //No pasarlo dos veces
    close(FD2[0]); // B-read not needed anymore



   return 0;
}