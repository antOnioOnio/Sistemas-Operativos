#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
//se define PATH_MAX a 1024 (máximo numero de bytes en una ruta, incluyendo el caracter de terminacion nulo)

int main( int argc, char *argv[] )
{
char rutaarchivo[NAME_MAX], rutaarchivotipo[NAME_MAX];
int estado;
if(setvbuf(stdout,NULL,_IONBF,0)) {
	perror("\nError en setvbuf");
}
	DIR *dir;
	struct dirent *mi_dirent;
	struct stat a;

	if( argc != 2 )	{
		printf( "Error en argumentos; debe proporcionar un directorio\n");
		exit(EXIT_FAILURE);
		}
	if( (dir = opendir( argv[1] )) == NULL ) {
		perror( "opendir error " );
		exit(EXIT_FAILURE);
		}

	while( (mi_dirent = readdir( dir )) != NULL ){
		if(!strcmp(mi_dirent->d_name,".") ) continue;
		if(!strcmp(mi_dirent->d_name,"..") ) continue;
		sprintf(rutaarchivo,"%s/%s",argv[1], mi_dirent->d_name);
		sprintf(rutaarchivotipo,"%s.tipo/%s.tipo",argv[1], mi_dirent->d_name);
		if(stat(rutaarchivo,&a) < 0) {
			perror("\nError en stat");
			printf("\nError en stat sobre el archivo=%s\n", rutaarchivo);
			exit(EXIT_FAILURE);
		}
		if (fork()==0) {
			int fd;
			if( (fd=open(rutaarchivotipo,O_WRONLY|O_CREAT|O_TRUNC,0700)) <0)
			{		perror("Error en open");
					printf("No se ha podido abrir %s\n", rutaarchivotipo);
					exit(EXIT_FAILURE);
			}
			dup2 (fd,STDOUT_FILENO);
			printf( "uid:%i\n", (int) a.st_uid);
			printf( "SUID:%s\n",a.st_mode & S_ISUID?"true":"false");
			execlp("file","file", rutaarchivo,NULL);
			perror("Error en exec");
			printf("No se ha ejecutado file sobre %s\n",rutaarchivo);
			exit(EXIT_FAILURE);
		}//fin hijo
	}
	closedir(dir);
	while (wait(&estado)!=-1);//nada
	exit(EXIT_SUCCESS);
}
