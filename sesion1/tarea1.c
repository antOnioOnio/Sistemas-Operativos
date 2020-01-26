/*
tarea1.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant''
Probar tras la ejecución del programa: $>cat archivo y $> od -c archivo
*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	
/* Primitive system data types for abstraction\
of implementation-dependent data types.
POSIX Standard: 2.6 Primitive System Data Types
<sys/types.h>
*/

#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

char buf1[]="abcdefghij";
char buf2[]="ABCDEFGHIJ";

int main(int argc, char *argv[])
{
int fd;
// creamos el archivo sino existe, lo creamos con permisos de escritura 
// con O_TRUNC, si existe el fichero y tiene habilitada la escritura, lo sobreescribe a tamaño 0
if( (fd=open("archivo",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
	printf("\nError %d en open",errno);
	perror("\nError en open");
	exit(EXIT_FAILURE);
}

// escribimos en el descriptor de archivos el bufer1, da error si no escribimos 10
if(write(fd,buf1,10) != 10) {
	perror("\nError en primer write");
	exit(EXIT_FAILURE);
}
// colocamos el puntero con lseek en 40, con lo cual empezaremos despues a escribir a 
if(lseek(fd,40,SEEK_SET) < 0) {
	perror("\nError en lseek");
	exit(EXIT_FAILURE);
}

// escribimos el ultimo buffer 
if(write(fd,buf2,10) != 10) {
	perror("\nError en segundo write");
	exit(EXIT_FAILURE);
}

return EXIT_SUCCESS;
}
