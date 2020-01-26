/*Del proyector de Mª Angustias */

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define ARCHIVO_FIFO "datos"

int main (int argc,char *argv[]){


char cad[12];
cad[11]='\0';

int fd, leidos;
pid_t pid;

if(setvbuf(stdout,NULL,_IONBF,0)){
	perror("\nError en setbuf");
}

//Comprobacion de argumentos
if(argc !=3){
	printf("Uso: %s <archivo1> <archivo2> \n",argv[0]);
	exit(-1);
}

//Crear el cauce con nombre (FIFO) si no existe 
umask(0);
mkfifo(ARCHIVO_FIFO,0666);

//Abrir el cauce para lectura-escritura
if((fd=open(ARCHIVO_FIFO, O_RDWR,S_IRWXU)<0)){
	perror("Error en open");
	exit(-1);
}

//Crear hijo 1
if((pid=fork())==-1){
	perror("Error al crear el hijo");
	exit(-1);
}


if(!pid){//Hijo
	dup2(fd,STDOUT_FILENO);
	execlp("./chequeoarchivo","chequeoarchivo",argv[1],NULL);
	perror("Error en execlp");
	exit(-1);
}

printf("Lanzado hijo para %s\n",argv[1]);

//Crear hijo 2
if((pid=fork())==-1){
	perror("Error al crear el hijo");
	exit(-1);
}

if(!pid){//Hijo
	dup2(fd,STDOUT_FILENO);
	execlp("./chequeoarchivo","chequeoarchivo",argv[2],NULL);
	perror("Error en execlp");
	exit(-1);
}

printf("Lanzado hijo para %s\n",argv[2]);

printf("Esperando diferencias ...\n\n");

while(leidos =read(fd,cad,11)!=0){
	printf("Diferencia detectada: %s\n",cad);
}

return 0;


}
