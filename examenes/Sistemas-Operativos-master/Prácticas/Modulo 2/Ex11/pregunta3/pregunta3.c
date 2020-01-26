#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main (int argc, char * argv[])
{

    struct flock cerrojo;//estructura del cerrojo
    int fd;//descriptor de archivo

    if ((fd = open("testfilea.txt", O_RDWR| O_CREAT, S_IRUSR| S_IWUSR)) == -1 ) {
        perror("open fallo");
        exit(-1);
    }
    cerrojo.l_type=F_WRLCK;//cerrojo para escritura
    cerrojo.l_whence=SEEK_SET;//
    cerrojo.l_start=0;	  //archivo completo
    cerrojo.l_len=0;	  //

    if (fcntl (fd, F_SETLK, &cerrojo) == -1) {
        perror("fcntl fallo");
        exit(-1);
    }
    printf ("procesando el archivo\n");
    while(1);


    cerrojo.l_type=F_UNLCK;
    cerrojo.l_whence=SEEK_SET;
    cerrojo.l_start=0;
    cerrojo.l_len=0;
    if (fcntl (fd, F_SETLKW, &cerrojo) == -1) {
        perror ("Desbloqueo");
        exit(-1);
    }
    return 0;
}
