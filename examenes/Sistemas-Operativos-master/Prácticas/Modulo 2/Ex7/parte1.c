#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int main(){//vicen
	char * p ;
	int pid, fd[2], fda, leido;
	char nombre[256], blq[512];
	DIR * dir;
	struct dirent * ent;
	struct stat st;
	
	if(pipe(fd) < 0) perror("..."),exit(1);
	if((pid=fork()) < 0) perror("..."),exit(1);
	else if(pid != 0){
		close(fd[0]);
		if ((dir = opendir(".")) == NULL){perror("error opendir"),exit(1);}
		while((ent = readdir(dir)) != NULL){
			if(stat(ent->d_name, &st)< 0){perror("error stat"),exit(1);}
			if(S_ISREG(st.st_mode)){
				printf("numero inodo es: %ld \n id del propietario es : %u \n", st.st_ino ,st.st_uid );
				if((write(fd[1],ent->d_name,sizeof(char)*256)) < 0){perror("error write"),exit(1);}
			}
			
		}
	}else {
		close(fd[1]);
		while((read(fd[0], nombre, sizeof(char)*256)) > 0 ){
			if((fda=open(nombre, O_RDONLY))< 0){perror ("error en open"), exit(1);}
			
			if(stat(nombre, &st) < 0){perror ("error en stat"), exit(1);}
			p=(char*)mmap(0,st.st_size,PROT_READ,MAP_SHARED,fda,0);
			
			for(int i = 0 ; i < st.st_size; i++){
				printf("%c",p[i]);
			}
			
			
			if(munmap(p, st.st_size) < 0){perror ("error en munmap"), exit(1);}
			close(fda);
		}
		
	}
}


// el campo de name tiene siempre 256
/*
int main(){

	int fd[2], numero = 256, leido;
	int fdarchivo, leido_archivo;
	char nombre_leido[256];
	char bloque[512];
	pid_t PID;
	
	if(pipe(fd) < 0) perror("..."),exit(1);
	if((PID=fork()) < 0) perror("..."),exit(1);
	
	if (PID == 0){
		close(fd[1]);
		if((leido=read(fd[0], nombre_leido, sizeof(char)*numero)) < 0 ) perror ("..."), exit(1);
		printf("se ha leido %s \n", nombre_leido);
		
		if ((fdarchivo = open(nombre_leido, O_RDONLY))< 0){perror ("error en open"), exit(1);}
		
		while((leido_archivo=read(fdarchivo, bloque, sizeof(char)*512)) > 0){
			write(fdarchivo,STDOUT_FILENO,sizeof(char)*leido_archivo);
		}
	}else{
		close(fd[0]);
	
		DIR * carpeta;
		struct dirent * entrada ;
		if ((carpeta = opendir(".")) == NULL){perror("error opendir"),exit(1);}//directorio actual
		
		while(entrada = readir(carpeta) != NULL){
			printf("numero inodo es: %d \n id del propietario es : %s \n",entrada -> d_inodo ,entrada -> d_name );
		
			if((write(fd[1],nombre_leido, sizeof(char)*leido)) < 0) perror(".."), exit(1) ;
		}
	}
	
}*/
