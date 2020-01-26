#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *archivo_regular;
  pid_t pid;
  int fd[2];
  int fdregular;
  int bytesLeidos;
  char cadenaLeida[11];
  struct stat atributos;

  if(argc!=2){
    printf("USO: ./explorar <archivo_regular>\n");
    exit(EXIT_FAILURE);
  }

  //Guardo la ruta del fichero regular dado como argumento.
  archivo_regular = argv[1];

  //Creo el cauce sin nombre
  pipe(fd);

  //Creo el hijo
  if((pid = fork()) < 0){
    perror("\nError en el fork");
    exit(EXIT_FAILURE);
  }

  if(pid==0){
    //El hijo ejecutará la orden "cut -d: -f6 etcpasswd".
    //Cierro lectura del cauce ya que solo vamos a escribir.
    close(fd[0]);
    //Abrimos fichero archivo regular pasado como argumento
    fdregular = open(archivo_regular, O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR);
    //Cambio la salida de error del fichero por el fichero que hemos abierto antes "fdregular"
    dup2(fdregular, STDERR_FILENO);
    //Cambiamos la salida estandar por la salida al fichero
    dup2(fd[1], STDOUT_FILENO);
    //Finalmente se ejecuta la orden "cut -d: -f6 etcpasswd"
    execlp("cut", "cut", "-d:", "-f6", "etcpasswd", NULL);
  }else{
    //El proceso padre para cada ruta elegida comprobará si existe y es de tipo directorio, comprobará que tenga permisos rwx para el
    //propietario y se comprobará que exista el fichero .bashrc.
    //Cierro la escritura en el cauce por que solo voy a leer.
    close(fd[1]);
    while((bytesLeidos=read(fd[0], cadenaLeida, sizeof(cadenaLeida)))>0){
      //Imprimo la ruta
      //Añado el caracter final de string
      cadenaLeida[10]='\0';
      printf("Cadena leida: %s\n", cadenaLeida);
      //Genero la ruta de cadenaLeida/.bashrc
      char rutaBashrc[1024];
      sprintf(rutaBashrc, "%s/.bashrc", cadenaLeida);
      printf("Cadena bashrc: %s\n", rutaBashrc);
      //Se calcula el stat de la ruta actual leida del cauce para saber si existe o no
      if(stat(cadenaLeida, &atributos) >= 0){
        //Se comprueba si es un directorio la ruta
        if(S_ISDIR(atributos.st_mode)){
          //Se cambian los permisos al directorio dado como ruta
          if(chmod(cadenaLeida, atributos.st_mode | S_IRWXU) < 0){
            perror("No se ha podido cambiar los permisos para la ruta");
          }

          //Comprobamos si existe el fichero .bashrc y si existe lo imprimimos
          if(stat(rutaBashrc, &atributos) >= 0){
            printf("El directorio con ruta %s existe y el fichero %s tiene el contenido siguiente: \n", cadenaLeida, rutaBashrc);
            execlp("cat", "cat", rutaBashrc, NULL);
          }else{
            perror("No existe el fichero .bashrc en la ruta.");
          }
        }else{
          perror("No es un directorio la ruta");
        }
      }else{
        perror("No existe la ruta");
      }
    }
  }
}
