#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

int main (int argc, char * argv[])
{
char nada[10];

	struct flock cerrojo;
	int fd;
	if ( argc ==1 ){
			printf("Debe dar un argumento\n");
			exit(-1);
	}

	if ( (fd=open(argv[1], O_RDWR))  <0 )  {
		perror("Error en open");
		exit(EXIT_FAILURE);
	}

	cerrojo.l_type=F_WRLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;

	if ( fcntl (fd, F_SETLK, &cerrojo) <0)
	{
		printf ("Pid: %d - Error al poner bloqueo\n",getpid());
	}
	else
	{
		printf ("Pid: %d - Bloqueo puesto con exito\n",getpid());
		printf ("Pulse algo y finalizara el programa\n");
		scanf("%s",nada);
	}
}
