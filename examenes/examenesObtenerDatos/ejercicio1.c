#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<ftw.h>

/**
 * 
 * Crea un programa en C llamado ejercicio1.c e implementa las siguientes tareas:
 * a) [1 punto] Realizar una exploracion completa de todas las entradas del directorio <<Ficheros>> proporcionado. Para ello
 * se puede utilizar la funcion <ntfw>. Filtrar por los siguientes criterios:
 *  -Archivos regulares
 *  -Todos los permisos para usuario, ninguno para grupo y de escritura para otros
 *  -Logitud del nombre de larchivo mayor que 7
 * 
 * b) [1 punto] Por cada entrada seleccionada, escribir una cadena de caracteres por la salida estandar que siga este formato
 *      nombre|permisos|tamaño.
 * **/


int visitar(const char* path, const struct stat* atributos, int flags, struct FTW*ftw) {

    // Nos quedamos con la ultima parte de la direccion
    char *res= strrchr(path, '/');
    // lo movemos a otro sitio para poder moverlo 1 a la derecha en memoria y quitarle la barra
    char line[100];
    sprintf(line, res);
    *line = *(line+1);
    char *cleanPath = strtok(line, " ");

    if(S_ISREG(atributos->st_mode) && (atributos->st_mode & S_IRWXU) &&  !(atributos->st_mode & S_IRWXG) && (atributos->st_mode & S_IWOTH) && strlen(cleanPath) > 7){

        printf("%s|%o|%ld\n", cleanPath, atributos->st_mode & ~S_IFMT, atributos->st_size);

    }
  
    return 0; 
}





int main(int argc, char *argv[]){

      char *pathname;

	if(argc!=2){
		printf("No has pasado un argumento\n");
		exit(EXIT_FAILURE);
	}else{
		pathname = argv[1];
	}

  if(nftw(pathname, visitar, 10, 0) != 0){
    perror("ntfw");
  }

	return 0;

}


// S_IFMT
// type of file
// S_IFBLK
// block special
// S_IFCHR
// character special
// S_IFIFO
// FIFO special
// S_IFREG
// regular
// S_IFDIR
// directory
// S_IFLNK
// symbolic link
// File mode bits:

// S_IRWXU
// read, write, execute/search by owner
// S_IRUSR
// read permission, owner
// S_IWUSR
// write permission, owner
// S_IXUSR
// execute/search permission, owner
// S_IRWXG
// read, write, execute/search by group
// S_IRGRP
// read permission, group
// S_IWGRP
// write permission, group
// S_IXGRP
// execute/search permission, group
// S_IRWXO
// read, write, execute/search by others
// S_IROTH
// read permission, others
// S_IWOTH
// write permission, others
// S_IXOTH
// execute/search permission, others
// S_ISUID
// set-user-ID on execution
// S_ISGID
// set-group-ID on execution
// S_ISVTX
// on directories, restricted deletion flag