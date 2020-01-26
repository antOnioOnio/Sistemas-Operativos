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
#include <limits.h>


int main(int argc , char * argv[]){
	
	
	if(argc != 2)
		perror("error en el num. args"),exit(1);
		
	
	DIR * dir;
	struct stat st;
	struct dirent *ent;
	char *lista=getenv("PATH");
	int pid,archivo;
	int pcut[2];
	char rutadir[PATH_MAX];
	
	if((pid=fork()<0))
		perror("error en el fork"),exit(1);
	
	if(archivo=open("listabusqueda",O_CREAT|O_TRUNC|O_RDWR,0666)<0)
		perror("Error al abrir/crear archivo"),exit(1);
		
	if(write(archivo,lista,sizeof(lista))<0)	
		perror("Error al escribir en archivo"),exit(1);
		
	if(pipe(pcut)<0)
		perror("Error al crear pipe"),exit(1);
	
	
	if(pid==0){//hijo
		close(pcut[0]);
		dup2(pcut[1],STDOUT_FILENO);
		execlp("cut","cut","-d:","-f",argv[2],"-z","listabusqueda",NULL);
		perror("Error en el exec del hijo"),exit(1);
	}else{//padre
		close(pcut[1]);
		int bytes_leidos=read(pcut[0],&rutadir,PATH_MAX);
		
		if(bytes_leidos==1){
			printf("No se ha leido nada en el cauce\n"),exit(0);
		}else if(bytes_leidos>1){
			if((dir=opendir(rutadir))==NULL)
				perror("error al abrir directorio"),exit(1);
			
			while((ent=readdir(dir))>0){
				if(strcmp(ent->d_name,argv[1])){//Si existe argv[1]
					if(stat(argv[1],&st)<0)
						perror("error en el stat"),exit(1);
					printf("tam. bytes %ld, UID: %d, GID: %d\n",st.st_size,st.st_uid,st.st_gid);
				}
			}
		}
	}
}



