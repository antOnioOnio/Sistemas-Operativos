#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <math.h>


int main(int argc, char *argv[]){


    if( argc != 3){
        printf("Correcto uso del programa--> ./Maestro <LIMITE_INFERIOR><LIMITESUPERIOR>\n");
        exit(-1);
    }

    // Declaracion de variables


    int inicioPrimerEsclavo, finPrimerEsclavo, inicioSegundoEsclavo, finSegundoEsclavo;
    pid_t PidEsclavo1, PidEsclavo2;
    int fd1[2] , fd2[2];
    int numBytes, valLeidoEsclavo1, valLeidoEsclavo2;
    int estado1, estado2;
    char ini[10];
    char fin[10];
    char *parametros[4];

    // convertimos los argumentos a nuestras variables
    inicioPrimerEsclavo = atoi(argv[1]);
    finSegundoEsclavo = atoi(argv[2]);

    finPrimerEsclavo = (inicioPrimerEsclavo+finSegundoEsclavo) / 2 ; 
    inicioSegundoEsclavo = finPrimerEsclavo + 1;
   

    printf( "inicioPrimerEsclavo %d \n ", inicioPrimerEsclavo);
    printf( "finPrimerEsclavo %d\n ", finPrimerEsclavo);
    printf( "inicioSegundoEsclavo %d\n ", inicioSegundoEsclavo);
    printf( "finSegundoEsclavo %d\n ", finSegundoEsclavo);

    parametros[0] = "ejercicio5Esclavo";
    parametros[3] = NULL;


    //---------------- Creamos los dos cauces------------------

    pipe(fd1); 
    pipe(fd2);


    // ----------------------Creacion del primer hijo---------------------

    if ( (PidEsclavo1 = fork()) < 0 ){
        perror("fork");
        exit(EXIT_FAILURE);
    }


    if (PidEsclavo1 == 0 ){ 
        // -----------Primer hijo ejecutando--------------------

        close(fd1[0]);  // -----Cerramos el descriptor de lectura, solo vamos a escribir
        
        // definimos los parametros que pasamos en el execv
        sprintf(ini, "%d",  inicioPrimerEsclavo);
        sprintf(fin, "%d", finPrimerEsclavo ) ;
        parametros[1] = ini;
        parametros[2] = fin;
        
        printf("\n [HIJO1] --> ENVIO PRIMER PARAMETRO %s \n [HIJO1] --> ENVIO SEGUNDO PARAMETRO %s \n", ini, fin);

        dup2(fd1[1],STDOUT_FILENO);  // ---------------------------------------------------PREGUNTAR
	    execv("./ejercicio5Esclavo", parametros);
        exit(-1);

    }
    
    dup2(fd1[0],STDIN_FILENO);
    close(fd1[1]);  // ---Cerramos el descriptor de escritura, solo vamos a leer

    while (  (numBytes= read(fd1[0], &valLeidoEsclavo1, sizeof(int))) > 0   ){

        printf("\n El esclavo 1 dice que : %d es primo \n", valLeidoEsclavo1);

    }
    close(fd1[0]);



    // ----------------------Creacion del segundo hijo---------------------

    if ( (PidEsclavo2 = fork()) < 0 ){
        perror("fork");
        exit(EXIT_FAILURE);
    }


    if (PidEsclavo2 == 0 ){ 
        // -----------Segundo hijo ejecutando--------------------

        close(fd2[0]);  // -----Cerramos el descriptor de lectura, solo vamos a escribir
        
        // definimos los parametros que pasamos en el execv
        sprintf(ini, "%d",  inicioSegundoEsclavo);
        sprintf(fin, "%d", finSegundoEsclavo ) ;
        parametros[1] = ini;
        parametros[2] = fin;
        

        printf("\n [HIJO2] --> ENVIO PRIMER PARAMETRO %s \n [HIJO2] --> ENVIO SEGUNDO PARAMETRO %s \n", ini, fin);

        dup2(fd2[1],STDOUT_FILENO);  // ---------------------------------------------------PREGUNTAR
	    execv("./ejercicio5Esclavo", parametros);
        exit(-1);

    }
    
    dup2(fd2[0],STDIN_FILENO);
    close(fd2[1]);  // ---Cerramos el descriptor de escritura, solo vamos a leer

    while (  (numBytes= read(fd2[0], &valLeidoEsclavo2, sizeof(int))) > 0   ){

        printf("\n El esclavo 2 dice que : %d es primo \n", valLeidoEsclavo2);

    }
    close(fd2[0]);

    return EXIT_SUCCESS;

}
