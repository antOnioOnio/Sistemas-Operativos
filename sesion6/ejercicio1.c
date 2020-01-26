// Ejercicio 1. Implementa un programa que admita t argumentos. El primer argumento será una
// orden de Linux; el segundo, uno de los siguientes caracteres “<” o “>”, y el tercero el nombre de
// un archivo (que puede existir o no). El programa ejecutará la orden que se especifica como
// argumento primero e implementará la redirección especificada por el segundo argumento hacia
// el archivo indicado en el tercer argumento. Por ejemplo, si deseamos redireccionar la entrada
// estándar de sort desde un archivo temporal, ejecutaríamos:
//                      $> ./mi_programa sort “<” temporal

#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

int main (int argc, char * argv[])
{

  char * orden= argv[1];
  char * redireccion= argv[2];
  int fd;

  if (strcmp(redireccion,"<") == 0){ //Redirigir la entrada, leer del archivo

    fd = open (argv[3], O_RDONLY);
    close (0);
    if (fcntl(fd, F_DUPFD, 0) == -1 )
        perror ("Fallo en fcntl");

  } else if ( strcmp(redireccion,">") == 0 ){ //Redirigir la salida, escribir en el archivo

    fd = open (argv[3], O_WRONLY | O_CREAT);
    close (1);
    if (fcntl(fd, F_DUPFD, 1) == -1 )
        perror ("Fallo en fcntl");

  }

  if (execlp(orden,orden,NULL) < 0){
    perror("Error en execlp");
  }

}