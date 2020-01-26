//Examen 04 - Ejercicio 02

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>



int procesos = 0 ;

void manejador(int sig){
	 int estado ;
	 wait(&estado); // a NULL si me da igual como acabe. wait para no tener hijos zombi
	 if(estado != 0){
	 	 printf("Finalizado con error "); // muestra mensaje de error al llegar el hijo
	 }
	 procesos ++ ;
}

int main(int argc, char * argv[]){


	 struct sigaction sa;
	 sa.sa_handler = manejador ; // cuando llegue una seÃ±al se ejecuta el manejador.
	 if ((sigaction(SIGCHLD, &sa, NULL)) < 0){perror("error en sigaction"),exit(1);}//seÃ±al hijo

	 int numero_argumentos = argc -1;
	 
	 if(argc < 2) {perror("uso: maestro <directorio>, minimo un argumento"),exit(1);}

	 pid_t pid ;
	 int fd[2];
	 DIR * d;
	 struct dirent * ent;
	 int contador = 0 ;
	 char dato[500];
	 
	 if((pipe(fd)) < 0) perror(" error al abrir cauce"),exit(1);
	 
	 for(int i=1 ; i <= numero_argumentos ; i ++){
		  if((pid=fork())<0){perror("error al crear el hijo"),exit(-1);}
		  
		  if (pid == 0){
			   close(fd[0]);
			   if((d=opendir(argv[i])) == NULL){perror("error en opendir"),exit(-1);}
			   if(dup2(fd[1], STDOUT_FILENO)<0)perror("error en dup2"),exit(1);
			   execlp("./contar","contar","archivo.txt", argv[i], NULL);
		  }
	 }
	 

	 close(fd[1]);

	 if(dup2(fd[0], STDIN_FILENO)<0)perror("error en dup2"),exit(1);

	 while((read(fd[0],&dato, sizeof(dato))) > 0){
	 	contador += atoi(dato);
	 }
	 while(procesos < numero_argumentos){
	  	sleep(1);
	 }
	 printf("%d \n\n", contador);
	 
	 return 0 ; 
}
