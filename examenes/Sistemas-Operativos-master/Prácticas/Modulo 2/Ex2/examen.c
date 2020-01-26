#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv) { 
	//validacion de parametros 
	if(!(argc==2)){
		perror("introduce correctamente el numero de parametros ");
		exit(-1);
	}

	pid_t proceso;
	int fd[2], fderror;
	
	struct stat st;

	//crea cauce
	if(pipe(fd)<0){
		perror("error al crear el cauce");
		exit(-1);
	}

	//crea hijo
	if((proceso=fork())<0){
		perror("error al crear el hijo");
		exit(-1);
	}
	
	//si es hijo
	if(proceso==0){
		//cierra la entrada
		close(fd[0]);
		fderror=open(argv[1],O_CREAT | O_RDWR,0666);
		dup2(fderror,STDERR_FILENO);
		dup2(fd[1],STDOUT_FILENO);
		execlp("cut","cut","-d:","-f6","fichero",NULL);
	}else{	//padre
		int leer;
		char nombre[11];
		while((leer=read(fd[0],nombre,sizeof(nombre)))>0){
			nombre[10]='\0' ;
			//si existe
			if(stat(nombre,&st)==0){
			//si es de tipo directorio
				if(S_ISDIR(st.st_mode)){
					chmod(nombre,st.st_mode | S_IRWXU);
					char cadenanueva[1024];
					sprintf(cadenanueva,"%s/.bashrc",nombre);
					if(stat(cadenanueva, &st)<0){
						perror ("no existe el archivo .bashrc o no tienes permisos");
					}
					else {
						puts("existe el archivo .bashrc y este es su contenido:");
						execlp("cat","cat",cadenanueva,NULL);
					}
				}else{
					perror(nombre);
				}
			}else{
				perror(nombre);
			}
		}
	}
}
