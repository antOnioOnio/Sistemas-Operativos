// Ejercicio 4. Construir un programa que se asegure que solo hay una instancia de él en
// ejecución en un momento dado. El programa, una vez que ha establecido el mecanismo para
// asegurar que solo una instancia se ejecuta, entrará en un bucle infinito que nos permitirá
// comprobar que no podemos lanzar más ejecuciones del mismo. En la construcción del mismo,
// deberemos asegurarnos de que el archivo a bloquear no contiene inicialmente nada escrito en
// una ejecución anterior que pudo quedar por una caída del sistema.
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  int fd;
  struct flock cerrojo;
  char nada[10];
  // Inicializamos el cerrojo para bloquear todo
  cerrojo.l_type= F_WRLCK;
  cerrojo.l_whence= SEEK_SET;
  cerrojo.l_start= 0;
  cerrojo.l_len = 0;

  // Abrir archivo "cerrojo".
  if ((fd = open("archivoBloqueo", O_RDWR | O_CREAT)) == -1)
    printf("Error al abrir blockfile, hay un cerrojo sobre el archivo\n");

  if (fcntl(fd, F_SETLKW, &cerrojo) == -1){
    perror ("Error en fcntl");
    exit(1);
  }

  printf("Archivo bloqueado, pulsa enter para desbloquear\n");
  scanf("%s",nada);

  if (fcntl(fd, F_UNLCK, &cerrojo) == -1){
    perror ("Error en fcntl");
    exit(1);
  }

  while(1);
