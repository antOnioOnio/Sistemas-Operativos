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
	if ( fcntl (fd, F_GETLK, &cerrojo) <0)
	{
		perror("Error en fcntl");
		exit(EXIT_FAILURE);
	}
	if ( cerrojo.l_type == F_UNLCK) 
		printf("No hay actualmente ningún bloqueo sobre %s\n",argv[1]);
	else	
		printf ("El proceso de pid %d tiene actualmente puesto un bloqueo sobre %s\n", cerrojo.l_pid, argv[1]);
	
}
