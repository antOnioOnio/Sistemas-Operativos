/*
tarea7.c
Programa ilustrativo del uso de pipes y la redirección de entrada y
salida est‡ndar: "mils rutadirectorio | wc"
*/

#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, char *argv[]) 
{
int fd[2];
pid_t PID;
if(setvbuf(stdout,NULL,_IONBF,0)) {
///////no hay buffering en la salida estandar
   perror("\nError en setvbuf");
}

pipe(fd); // Llamada al sistema para crear un pipe

if ( (PID= fork())<0) {
	perror("fork");
	exit(EXIT_SUCCESS);
}	
if(PID == 0) {	// ------------ hijo
	close(fd[0]);//cierro lo que no uso
	dup2( fd[1], STDOUT_FILENO );
      execlp("./mils","mils",argv[1], NULL);
	perror("Error en exec\n");
	exit(EXIT_FAILURE);

}
else {			// ------------ padre
	close(fd[1]); //cierro lo que no uso	
	dup2( fd[0],  STDIN_FILENO  );
	execlp("wc","wc", NULL);
	perror("Error en wd\n");
	exit(EXIT_FAILURE);
}

}


