/* Programa que recorre un sub-árbol con la función nftw */
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include <ftw.h>


int tamanio = 0 , numeroFicheros =0;
int visitar(const char* path, const struct stat* atributos, int flags, struct FTW*ftw) {

    if(S_ISREG(atributos->st_mode) && (atributos->st_mode & S_IXGRP) && (atributos->st_mode & S_IXOTH)){
        printf("%s %ld\n", path, atributos->st_ino);
        tamanio += (int) atributos->st_size;
        numeroFicheros++;
    }
    return 0; 
}


int main(int argc, char** argv) {
	char *pathname;

  if ( argc == 2){
        pathname = argv[1];
        printf("DEBUUG pathname pasado --> %s \n", pathname);
    }else{
        printf("No se ha pasado directorio como argumento, cogemos el actual \n");
        pathname = ".";
        printf("DEBUUG pathname pasado --> %s \n", pathname);

    }

  printf("Los i-nodos son %s: \n", pathname);

  if(nftw(pathname, visitar, 10, 0) != 0){
    perror("ntfw");
  }

  printf("Existen %d archivos regulares con permiso x para el grupo y otros.\n", numeroFicheros);
  printf("El tamaño total ocupado por dichos ficheros es %d bytes.\n", tamanio);

	return 0;

}