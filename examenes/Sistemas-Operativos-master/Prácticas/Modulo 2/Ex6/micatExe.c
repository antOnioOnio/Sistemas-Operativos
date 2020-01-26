/*forma de ejecucion: 
	gcc micatExe.c -o micatExe
	gcc leeOEscriben1.c -o ejer
	./ejer archivo_fuente archivo_destino */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>


int main(){
  char buffer[50];
  int bytes;

  while((bytes = read(STDIN_FILENO,buffer, 50 * sizeof(char))) > 0)
    if(write(STDOUT_FILENO,buffer, bytes * sizeof(char)) < 0) perror("1"),exit(1);
}
