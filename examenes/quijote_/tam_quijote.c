// Realizar una exploracion a un solo nivel del directorio tam_quijote.c proporcionado
// filtar por archivos regulares
// todos los permisos de usuario, ninguno para grupo y de escritura para otros
// escribe en la salida estandar el tamaño en bytes del archivo quijote.txt



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


int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Uso correcto: \t ./quijote <directorio> \n");
        exit(-1);
    }


    DIR *directorioAbierto;
    struct dirent *directorio;
    struct stat atributos;
    char archivo[1024];
    char *pathname = argv[1];

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
             
            if(S_ISREG(atributos.st_mode) && (atributos.st_mode & 702) ){
              
               if ( strcmp( directorio->d_name, "quijote.txt") == 0){
                   
                   int len = strlen(directorio->d_name);
                   char *extension = &directorio->d_name[len-4];
                   if ( strcmp(extension, ".txt") == 0){
                        printf("%d \n" , atributos.st_size ) ;
                   }
;
               }
            }

    }
    
    closedir(directorioAbierto);

    return 0;

}
