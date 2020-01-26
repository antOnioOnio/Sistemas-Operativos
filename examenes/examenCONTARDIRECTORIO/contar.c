
// 1.[3pts] Implementa un programa en C llamado contar que tiene la siguiente sintaxis:
// 	contar <archivo_salida> <directorio>    donde :

// a) Ambos argumentos son obligatorios: el primer argumento debe ser el nombre de un archivo regular que puede 
//  o no existir y el segundo debe ser el nombre de un directorio existente. Realiza el control de errores adecuado.

// b)La orden debe escribir en archivo_salida el nombre y tamaño en bloques de todos 
// los archivos de tipo directorio del directorio dado como argumento que , ademas 
// tengan permiso de ejecución para grupo. También va sumando los bloques
// ocupados por todos los subdirectorios que cumplen la condicion dada.

// c)archivo_salida debe crearse en el diretorio actual si no existe.
//  Si existiera, se sustituirá su contenido por el que genera este programa.

// d)Ademas, y justo antes de terminar, debe imprimir por la salida estandar la suma total
// de los bloques ocupados por todos los archivos que cumplían las condiciones dadas en b). Imprime solo un numero.

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<dirent.h>
#include <ftw.h>

int tamanio=0, tamanioTotal=0;
char buffer[5024];


int visitar(const char* path, const struct stat* atributos, int flags, struct FTW*ftw) {
    
    if( S_ISDIR(atributos->st_mode) && (atributos->st_mode & S_IXGRP)){

        sprintf(buffer +strlen(buffer), "%s %ld\n", path, atributos->st_size);
        
        tamanioTotal += (int) atributos->st_size; 
        return 0;
    }
    return 0; 
}

void busquedaDir(char *pathname, int fd){
    DIR *directorioAbierto;
    struct dirent *directorio;
    struct stat atributos;
    char archivo[1024];

    if ( (directorioAbierto = opendir(pathname)) == NULL){
        printf("\nError %d en opendir",errno);
        perror("\nError en opendir");
        exit(EXIT_FAILURE);
    }

    while ( (directorio = readdir(directorioAbierto)) != NULL){
        if (strcmp(directorio->d_name, ".") == 0 || strcmp(directorio->d_name, "..") == 0)
            continue;
            snprintf(archivo,sizeof(archivo),  "%s/%s", pathname, directorio->d_name);
            //  snprintf(path, sizeof(path), "%s/%s", pathname, entry->d_name);

             if ( stat(archivo, &atributos) < 0){
                printf("\nError al intentar acceder a los atributos de %s\n", directorio->d_name);
                perror("\nError en lstat");
                exit(EXIT_FAILURE);
             }

            if(S_ISDIR(atributos.st_mode) && (atributos.st_mode & S_IXGRP) && (atributos.st_mode & S_IXOTH)){
                
                sprintf(buffer +strlen(buffer), "%s %ld\n", archivo, atributos.st_size);
                
                tamanioTotal += (int) atributos.st_size;
                busquedaDir(archivo, fd); 
            
            }            
         }
    

    closedir(directorioAbierto);
}


int main(int argc, char *argv[]){

    if (argc != 3){
        printf("Uso correcto: \t ./contar <archivo_salida> <directorio> \n");
        exit(-1);
    }

    char *file = argv[1];       // archivo saliad
    char *directorio = argv[2]; 
    struct dirent *directorioLeido;
    struct stat atributos;
    DIR *dir;
    char nombre_directorio[100];
    int fd;  



    //Abrimos el archivo salida, y sino existe lo creamos
    if( (fd=open(file, O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR)) < 0 ) {
        printf("\nError %d en open",errno); //Imprimir por pantalla el error
        perror("\nError en open");
        exit(-1);
    }
    //Cogemos los atributos del archivo abierto
    if ( stat(file,&atributos) < 0 ){
        printf("\nError al intentar acceder a los atributos del archivo");
        perror("\nError en lstat");
        exit(-1);
    }

    // comprobamos que el archivo abierto sea de tipo regular
    if(! S_ISREG(atributos.st_mode)){
        printf("\nError, el archivo no es regular");
        perror("\nNo es archivo regular");
        exit(-1);
    
    }
    // comprobamos que el directorio exista
    if ( (dir=opendir(directorio)) == NULL){
        printf("\nError, directorio no existe");
        perror("\nDirectorio no existe");
        exit(-1);
    }

    //opcion 1

    if( (nftw(directorio, visitar, 10, 0)) != 0){
        perror("ntfw");
    }
  

    //opcion  2  
    //busquedaDir(directorio, fd);

    write(fd, buffer, sizeof(buffer));
    printf("%d" , tamanioTotal);

        

    close(fd);

    return 0;



}