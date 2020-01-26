// Ejercicio 2. Reescribir el programa que implemente un encauzamiento de dos órdenes pero
// utilizando fcntl. Este programa admitirá tres argumentos. El primer argumento y el tercero
// serán dos órdenes de Linux. El segundo argumento será el carácter “|”. El programa deberá
// ahora hacer la redirección de la salida de la orden indicada por el primer argumento hacia el
// cauce, y redireccionar la entrada estándar de la segunda orden desde el cauce. Por ejemplo,
// para simular el encauzamiento ls|sort, ejecutaríamos nuestro programa como:
// $>       ./mi_programa2   ls     “|”     sort
//          0                 1      2      3

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

    if (argc != 4){
      printf("Uso: ./ejercicio2 [orden1] '|' [orden2] \n");
       exit(-1);
    } else if (strcmp(argv[2],"|") != 0){
      printf("El segundo argumento debe ser '|'\n");
       exit(-1);
    }


    char * orden1= argv[1];
    char * orden2= argv[3];
    int fd[2];
    pid_t PID;


    pipe(fd); // Llamada al sistema para crear un pipe

    if ( (PID= fork())<0) {
      perror("fork");
      exit(-1);
    }

    if(PID == 0) { // ls
      close(fd[0]);

      if (fcntl(fd[1], F_DUPFD, 1) == -1 )
          perror ("Fallo en fcntl\n");

      execlp(orden1,orden1,NULL);
      exit(-1);
    }
    else { // sort. Estoy en el proceso padre porque PID != 0
      close(fd[1]);

      if (fcntl(fd[0], F_DUPFD, 0) == -1 )
          perror ("Fallo en fcntl\n");

      execlp(orden2,orden2,NULL);
      exit(-1);
    }

    return(0);


}