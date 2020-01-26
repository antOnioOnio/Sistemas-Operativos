#include <sys/types.h>
#include <unistd.h>		
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>	
#include <string.h>

// Ejercicio 7. Escribe un programa que acepte como argumentos el nombre de un programa, sus
// argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto programa deberá ejecutar el
// programa pasado como primer argumento en foreground si no se especifica la cadena “bg” y en
// background en caso contrario. Si el programa tiene argumentos hay que ejecutarlo con éstos.



int main ( int argc, char *argv[]){
 
    pid_t pid;
    bool bg = false;
    int numeroParametros = argc;

    char *orden;
    char *parametros[10];
    int estado;

    if(numeroParametros<2){
		printf("Modo de uso: %s <programa> [opciones] [bg]\n\n", argv[0]);
		exit(1);
    }
    orden = argv[1];
    
    for ( int i = 1; i < numeroParametros; i++){
        parametros[i-1] = argv[i];
    }

    

    if( strcmp(argv[argc-1], "bg") == 0){
        bg = true;
        numeroParametros--;
    }

    printf("Numeros parametros--> %d \n", numeroParametros);

  

    printf("(ORDEN-->) %s \t\n", orden );


    for ( int i = 0 ; i < argc-1 ; i++){
        printf("(PARAMETROS-->) %s \t\n", parametros[i] );
    }
    parametros[argc-1] = NULL;

    if (bg){
      
        if  ((pid =fork()) < 0){
            perror("\n Error en el fork \n");
            exit(-1);
        }

        else if (pid == 0){
            printf("----Ejecutando en background----\n");
            if( (execv(orden, parametros)<0)) {
                    perror("Error en el execl\n");
                    exit(-1);
            }

        }else{
            exit(0); //--->padre se va fuera
        }

    }else{
        printf("----Ejecutando en foreground----\n");
        if( (execv(orden, parametros)<0)) {
            perror("Error en el execl\n");
            exit(-1);
        }

    }

    exit(0);
}