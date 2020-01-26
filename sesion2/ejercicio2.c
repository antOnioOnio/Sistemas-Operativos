// Ejercicio 2: Realiza un programa en C utilizando las llamadas al sistema necesarias que acepte como entrada:
//   - Un argumento que representa el 'pathname' de un directorio.
//   - Otro argumento que es un número octal de 4 dígitos (similar al que se puede utilizar
//     para cambiar los permisos en la llamada al sistema chmod). Para convertir este
//     argumento tipo cadena a un tipo numérico puedes utilizar la función strtol. Consulta
//     el manual en línea para conocer sus argumentos.
// El programa tiene que usar el número octal indicado en el segundo argumento para cambiar los permisos de todos los archivos que se encuentren en el directorio indicado en el primer argumento.
// El programa debe proporcionar en la salida estándar una línea para cada archivo del directorio que esté formada por:
// <nombre_de_archivo> : <permisos_antiguos> <permisos_nuevos>
// Si no se pueden cambiar los permisos de un determinado archivo se debe especificar la siguiente información en la línea de salida:
// <nombre_de_archivo> : <errno> <permisos_antiguos>


#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<dirent.h>

int main(int argc, char *argv[]){
    int permisos;
    DIR *directorioOpen;
    struct dirent *directorio;
    char *pathname;
    struct stat atributos;
    char archivo[100];
    char cadena2[100];

    if (argc != 3){
        printf("uso: ejecutable <pathname><permisos>\n");
        exit(-1);
    }else{
        pathname = argv[1];
        permisos=strtol(argv[2], NULL, 8);
        if ( (directorioOpen=opendir(pathname))== NULL   ){
            printf("\nError %d en opendir",errno);
		    perror("\nError en opendir");
		    exit(EXIT_FAILURE);
        }

        printf("Este es el pathname: %s y estos los permisos indicados: %d\n", pathname, permisos);
    }

    while( (directorio = readdir(directorioOpen))!= NULL){
        if (strcmp(directorio->d_name,".")==0)      continue; 
        if (strcmp(directorio->d_name,"..")==0)     continue;

        sprintf(archivo,"%s/%s", pathname, directorio->d_name );
        // printf("ruta    -->%s \n", pathname);
        // printf("archivo--->%s \n", archivo);


        if(stat(archivo, &atributos) < 0){
            printf("\nError al intentar acceder a los atributos de %s\n", directorio->d_name);
            perror("\nError en lstat");
            exit(EXIT_FAILURE);
        }

        if (S_ISREG(atributos.st_mode)){
            int permisosAntiguos = atributos.st_mode;

            if ( chmod(archivo, permisos)<0){
                printf("%s : %d %d\n", directorio->d_name, errno, permisosAntiguos);
            }else{
                stat(archivo, &atributos);
                printf("%s : %d %d\n", directorio->d_name, permisosAntiguos, atributos.st_mode);

            }
        }
    }

    closedir(directorioOpen);
    return 0;

}