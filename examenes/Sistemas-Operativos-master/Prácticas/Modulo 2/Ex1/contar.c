// Examen 04 - Ejercicio 01

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


int main(int argc, char * argv[]){

	DIR * d;
	struct dirent * ent;
	struct stat att;
	
	int fdent , bloques = 0;

	if(argc != 3) {perror("uso: contar <archivo_salida> <directorio>"),exit(1);}
	
	if(setvbuf(stdout,NULL,_IONBF,0)) {perror("\nError en setvbuf");}
	
	if((fdent = open (argv[1],O_WRONLY|O_TRUNC|O_CREAT, 0666))< 0){perror(" error en open"),exit(1);}

	
	if((d=opendir(argv[2])) == NULL){perror("error en opendir"),exit(-1);}
	
	while ((ent=readdir(d))!=NULL){
		char ruta[512];
		sprintf(ruta,"%s/%s",argv[2], ent-> d_name); //meto el nombre del archivo en ruta

		if(stat(ruta, &att) < 0 ){perror("error en stat1"),exit(-1);}
		
		if(S_ISDIR(att.st_mode)){
			if(att.st_mode & S_IXGRP){
				bloques += (int) att.st_size; 
				write(fdent,ruta,sizeof(ruta));
			}
		} 
	}
	printf("%d",bloques);
	closedir(d);
	close(fdent);

	return 0 ;	
}
