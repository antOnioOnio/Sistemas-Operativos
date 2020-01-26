/*
	Sistemas Operativos - Prueba Modulo 2
	Curso 2º A - Grupo 1
	Apellidos : Barrilao Bensrhir
	Nombre:	Angel
	
	

*/


#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>



int main(int argc, char *argv[])
{
char * usuario=getenv("USER");
char ruta[255]="/etc/mtab";
char recibido[255];
int fd[2];
pid_t PID;

pipe(fd); // Llamada al sistema para crear un pipe

if ( (PID= fork())<0) {
	perror("\Error en fork");
	exit(-1);
}
if (PID == 0) { // ls
	//Cerrar el descriptor de lectura de cauce en el proceso hijo
	close(fd[0]);

	//Duplicar el descriptor de escritura en cauce en el descriptor
	//correspondiente a la salida estandar (stdout), cerrado previamente en
	//la misma operación
	dup2(fd[1],STDOUT_FILENO);
	execlp("grep","grep",usuario,ruta,NULL);
}
else { // sort. Proceso padre porque PID != 0.
	//Cerrar el descriptor de escritura en cauce situado en el proceso padre
	close(fd[1]);

	//Duplicar el descriptor de lectura de cauce en el descriptor
	//correspondiente a la entrada estándar (stdin), cerrado previamente en
	//la misma operación
	dup2(fd[0],STDIN_FILENO);
	execlp("cut","cut","-d ","-f",argv[1],NULL);
}

read(fd[0],recibido,sizeof(recibido));

printf("\n %s", recibido);



return(0);
}

