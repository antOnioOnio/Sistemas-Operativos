// Ejercicio 3. Construir un programa que verifique que, efectivamente, el kernel comprueba que
// puede darse una situación de interbloqueo en el bloqueo de archivos.

// opem RDWR
// while fnctl F_SETLKW
//     if error == EDEADLK
//         -> Interbloqueo detectado

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
  int fd;
  struct flock cerrojo;

  
    if(argc != 2) {

        //Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
        printf("Modo de uso: %s <programa> <archivo>\n\n", argv[0]);
        exit(1);

    }
    char *str_file = argv[1];
    // Abrimos el archivo
    if (  (fd = open(str_file , O_RDWR)) < 0){
        perror("Error en open\n");
        return 0;
    }

        cerrojo.l_type =F_WRLCK;
        cerrojo.l_whence =SEEK_SET;
        cerrojo.l_start =0;
        cerrojo.l_len =0; //--> bloqueamo el archivo entero

        //BLoquear el archivo
        printf("Bloqueando archivo %s \n", str_file);

        // bloqueamos el archivo
        if (fcntl(fd, F_SETLKW, &cerrojo) == EDEADLK){

			//Si el cerrojo falla, pintamos un mensaje
			printf ("%s ha dado un EDEADLK\n", str_file);

        }
        
        // ahora el bloqueo tiene exito, procesamos el archivo
        printf("Procesando archivo bloqueado\n");

        // hacemos una pausa para poder ejecutar de nuevo el proceso
        sleep(10);

        //Una vez finalizado el trabajo, desbloqueamos el archivo
        cerrojo.l_type=F_UNLCK;
        cerrojo.l_whence=SEEK_SET;
        cerrojo.l_start=0;
        cerrojo.l_len=0;

        if (fcntl (fd, F_SETLKW, &cerrojo) == -1) {
                    perror ("Error al desbloquear el archivo");
        }
        printf("Archivo desbloqueado\n");

        return(0);
}