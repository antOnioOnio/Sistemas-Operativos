// programa que muestra los nombres de los hijos del directorio dado como argumento

#include <dirent.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>

int main( int argc, char *argv[] )
  char mensajeerror[]="Debe dar 1 argumento: ruta de un directorio\n";
  if( argc != 2 )
	if (strcmp(entrada->d_name,".")==0)      continue;
	if (strcmp(entrada->d_name,"..")==0)     continue;
   }  