// Ejercicio 3. Programa una nueva orden que recorra la jerarquía de subdirectorios existentes a partir de uno dado 
// como argumento y devuelva la cuenta de todos aquellos archivos regulares que tengan permiso de ejecución para el 
// grupo y para otros. Además del nombre de los archivos encontrados, deberá devolver sus números de inodo y la suma 
// total de espacio ocupado por dichos archivos. El formato de la nueva orden será:
//       $> ./buscar <pathname>
// donde <pathname> especifica la ruta del directorio a partir del cual queremos que empiece a analizar la estructura 
// del árbol de subdirectorios. En caso de que no se le de argumento, tomará como punto de partida el directorio 
// actual. Ejemplo de la salida después de ejecutar el programa:
//  Los i-nodos son:
//   ./a.out 55
//   ./bin/ej 123
//   ./bin/ej2 87
//   ...
//   Existen 24 archivos regulares con permiso x para grupo y otros
//   El tamaño total ocupado por dichos archivos es 2345674 bytes


#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<dirent.h>

int tamanio = 0, numeroFicheros=0;

void busquedaDir(char *pathname){
    DIR *directorioAbierto;
    struct dirent *directorio;
    struct stat atributos;
    char archivo[150];

    if ( (directorioAbierto = opendir(pathname)) == NULL){
        printf("\nError %d en opendir",errno);
        perror("\nError en opendir");
        exit(EXIT_FAILURE);
    }

    while ( (directorio = readdir(directorioAbierto)) != NULL){
         if(strcmp(directorio->d_name, ".") && strcmp(directorio->d_name, "..")){
             sprintf(archivo, "%s/%s", pathname, directorio->d_name);

             if ( stat(archivo, &atributos) < 0){
                printf("\nError al intentar acceder a los atributos de %s\n", directorio->d_name);
                perror("\nError en lstat");
                exit(EXIT_FAILURE);
             }

            if(S_ISREG(atributos.st_mode) && (atributos.st_mode & S_IXGRP) && (atributos.st_mode & S_IXOTH)){
                printf("%s %ld\n", archivo, directorio->d_ino);
                tamanio += (int) atributos.st_size;
                numeroFicheros++;
            }else if ( S_ISDIR(atributos.st_mode)){
                busquedaDir(archivo);
            }         
         }
    }

    closedir(directorioAbierto);


}

int main(int argc, char *argv[]){
    char *pathname;

    if ( argc == 2){
        pathname = argv[1];
        printf("DEBUUG pathname pasado --> %s \n", pathname);
    }else{
        printf("No se ha pasado directorio como argumento, cogemos el actual \n");
        pathname = ".";
        printf("DEBUUG pathname pasado --> %s \n", pathname);

    }

    printf("Los i-nodos son: \n");
    printf("DEBUUG LLAMANDO A LA FUNCION\n");
    busquedaDir(pathname);
    printf("Existen %d archivos regulares con permiso x para el grupo y otros.\n", numeroFicheros);
    printf("El tamaño total ocupado por dichos ficheros es %d bytes.\n", tamanio);

	return 0;
}