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
{
  char mensajeerror[]="Debe dar 1 argumento: ruta de un directorio\n";
  DIR *midir;
  struct stat a;
  char ruta[256];
  struct dirent *entrada;  // mi_dirent es un puntero a una estructura
  if( argc != 2 )
  { 
    	write(STDERR_FILENO, mensajeerror, strlen(mensajeerror));
    	exit(EXIT_FAILURE);
  }
  if( (midir = opendir( argv[1])) == NULL )
  {   perror( "opendir" );
  	exit(EXIT_FAILURE);
  }
   while( (entrada = readdir( midir )) != NULL ){
    if (strcmp(entrada->d_name,".")==0)      continue;  // se puede hacer como si el nombre != "." y ".." lo proceso
    if (strcmp(entrada->d_name,"..")==0)     continue;
      
      strcpy (ruta , argv[1]);
      strcat ( ruta, "/");
      strcat (ruta, entrada->d_name);
      printf( "ruta=%s\n", ruta);

    if ( stat(ruta, &a)<0){
      printf("Error en stat sobre %s\n",entrada->d_name);
      perror("Error en stat");
    }
    else {
       printf("nombre:%s permisos:%o\n", entrada->d_name, a.st_mode);
    }
   
   }  
  closedir( midir );	
  exit(EXIT_SUCCESS);
}

