#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

    //  Comprueba tu solución implementando un código para generar 20 procesos en cada caso, 
    //  en donde cada proceso imprima su PID y el del padre, PPID.

int main(int argc, char *argv[])
{
	pid_t p;
	int nprocs = 10, estado;
	if(setvbuf(stdout,NULL,_IONBF,0)) {
		perror("\nError en setvbuf");
	}
	printf("Proceso inicial: %d\t Padre: %d\n", getpid(), getppid());

	for (int i=1; i < nprocs; i++) {
		if ((p=fork()) == -1) {
			fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
			exit(-1);
			}
		if (p) {// el padre ejecuta break y no vuelve al bucle
            
           break;
        }
         
	}
		
	sleep(getpid()%3);

	// todos los procesos creados, incluyendo el inicial
	//aqui hacen su "trabajo siginificativo" en paralelo
	// ...
	// 
	printf("Proceso: %d\t Padre: %d\n", getpid(), getppid());

	wait(&estado);



}
