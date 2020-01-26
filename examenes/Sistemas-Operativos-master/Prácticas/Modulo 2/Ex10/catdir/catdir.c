#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

#include<dirent.h>


int main(int argc, char *argv[]){

DIR *directorio;
struct dirent *file;//estructura directorio
int fd=1,fdaux;//lo igualamos a STDOUT_FILENO;
struct stat atributos;
unsigned long nbloques=0;
char buf1[1024];

	if(argc<2) {
		printf("\nSintaxis de ejecucion: catdir [<pathname>]+ [<archivodesalida>]+ \n\n");
		exit(-1);
	}
	if(argc==3){//se ha introducido archivo salida.argv[2] 
		if((fd=open(argv[2],O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {//se crea, si existe se trunca y se abre para escritura.
        		printf("\nError %d en open",errno);
        		perror("\nError en open");
        		exit(-1);
    		}
	}

	directorio = opendir(argv[1]);
	file = readdir(directorio);
	chdir(argv[1]);//abrimos el directorio.
			//podriamos concatenar el nombre del directorio con archivo : argv[1]/nombre_archivo;
	while(file != NULL && strcmp(file->d_name,".")==1 && strcmp(file->d_name,"..")==1){
			printf("\nFile Name: %s\n",file->d_name);
		
			if(stat(file->d_name,&atributos) < 0) {
				printf("\nError al intentar acceder a los atributos");
				perror("\nstat Fail");
				exit(-1);
			}
			if(S_ISREG(atributos.st_mode)&& (atributos.st_mode & S_IWUSR)&& (atributos.st_mode & S_IWGRP )){//es un archivo regular y tiene los permisos determiandos
				printf("el archivo: %s, tiene %lu bloques.\n",file->d_name,atributos.st_blocks);
				nbloques=atributos.st_blocks+nbloques;

				if((fdaux=open(file->d_name,O_RDONLY,S_IRUSR|S_IWUSR))<0) {//lo abrimos para lectura
        				printf("\nError %d en open",errno);
        				perror("\nError en open");
        				exit(-1);
    				}
				while(read(fdaux,buf1,1024)>0) {
        				if(write(fd,buf1,1024) ==-1) {
            					perror("\nError en write");
            					exit(-1);
        				}
    				}
				close(fdaux);
			}			
			file = readdir(directorio);
	}
	
	if(closedir(directorio)<0) {
		printf("\nclosedir Fail \n\n");
		exit(-1);
	}
	if(argc==3){//se ha introducido archivo salida.argv[2] 
		close(fd);
	}
	printf("el numero de bloques es: %lu\n",nbloques);
	

return 0;
}
