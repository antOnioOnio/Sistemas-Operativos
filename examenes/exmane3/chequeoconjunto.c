#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define ARCHIVO_FIFO "datos"


int main(int argc, char *argv[]){
  
    char dato[12];
    dato[11]='\0';
    int fd, fdl;
    pid_t pid;
    //char *parametros[3];
    int leidos;

    if(setvbuf(stdout,NULL,_IONBF,0)){
        perror("\nError en setbuf");
    }

    if( argc != 2){
        printf("Correcto uso del programa--> ./Maestro <ArchivoRegular><ArchivoRegular>\n");
        exit(-1);
    }


    // creacion de cauce con nombre FIFO
    umask(0);
    mkfifo(ARCHIVO_FIFO,0666);


    //Abrir el cauce para lectura-escritura
    if((fd=open(ARCHIVO_FIFO, O_RDWR,S_IRWXU)<0)){
        perror("Error en open");
        exit(-1);
    }
    //Abrir el cauce para escritura

    if ( ( pid = fork() ) ==-1 ){
        perror("Error al crear el hijo");
        exit(-1);
    }


    if ( !pid){
        //close(fd[0]);
        //printf("hijo para %s\n",argv[1]);

        dup2(fd,STDOUT_FILENO);
        execlp("./chequeoarchivo","chequeoarchivo",argv[1],NULL);
        // perror("Error en execlp");
        // exit(-1);

    }

    //close(fd[1]);
    //dup2(fd,STDIN_FILENO);

    dup2(fd, STDIN_FILENO);

    printf("Lanzado hijo para %s\n",argv[1]);


    while( (leidos = read(fd,dato,11)) !=0){
        printf("Diferencia detectada: %s\n",dato);
        fflush(stdout); 
        write(1, dato, leidos );
    }

 
    //close(fd[0]);

    //printf("despues --->%s\n", dato);


    return 0;


    }

    



