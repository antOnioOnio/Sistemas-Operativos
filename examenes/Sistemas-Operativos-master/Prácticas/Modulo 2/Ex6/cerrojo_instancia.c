//programa que se asegura de que es la unica instancia ejecutandose del archivo ejecutable.


#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<sys/wait.h>
#include<string.h>

int main(){ 

	struct flock cerrojo;
	int fd ;
	
	if((fd=open("bloqueo",O_WRONLY|O_CREAT,0666))<0){perror("error al abrir el archivo"),exit(1);}
	
	if (fcntl(fd, F_GETLK)){perror("fcntl error, ya hay una instancia del ejecutable"), exit(1);}
	
	if(cerrojo.l_type != F_UNLCK){exit(20);}

	cerrojo.l_type= F_WRLCK;//F_WRLCK escritura,F_RDLCK lectura, F_UNLCK 
	cerrojo.l_whence= SEEK_SET;
	cerrojo.l_start= 0;
	cerrojo.l_len = 0;
	//Si vamos a bloquearlo y ya lo esta, entonces
	if (fcntl(fd, F_SETLKW, &cerrojo) < 0 ){perror("error fnctl"), exit (1);}

	sleep(30);
	
	cerrojo.l_type= F_UNLCK;
	if (fcntl(fd, F_SETLKW, &cerrojo) < 0 ){perror("error fnctl"), exit (1);}

	printf("fin comprobacion");
	
	exit(0);
}
