
// Estudie el comportamiento de este programa

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
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
		if (!p) break; // el hijo ejecuta break y no vuelve al bucle
	}/// for
		

	// todos los procesos creados, incluyendo el inicial
	//aqui hacen su "trabajo siginificativo" en paralelo
	// ... cada hijo espera un determinado tiempo....
	sleep(random()%3);

	printf("Proceso: %d\t Padre: %d\n", getpid(), getppid());

	while ( wait(&estado)!= -1); 
	// cuando wait falle termino
	// (dado que no hay más hijos)
}