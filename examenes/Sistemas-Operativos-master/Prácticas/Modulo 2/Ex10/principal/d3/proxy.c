#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#define tamano 1024
#define longnombre 50

void bloqueo(int dbloqueo,int orden) {
    struct flock cerrojo;
    cerrojo.l_type=orden;
    cerrojo.l_whence=SEEK_SET;
    cerrojo.l_start=0;
    cerrojo.l_len=0;
    if (fcntl(dbloqueo,F_SETLKW,&cerrojo)==-1) {
        perror("PROXY:problema");
        exit(1);
    }
}



main() {
    char temp[longnombre],fifopidproxy[longnombre];;
    char buffer[tamano];
    int nbytes;
    int dtemp;
    int pidproxy;
    //int fd[2];
    int dbloqueo;
    //pipe(fd);
    pidproxy = getpid();
    sprintf(temp,"temp-%d",pidproxy);
    printf("PROXY %d: creamos un archivo temporal %s para lectura y escritura\n",pidproxy,temp);
    sprintf(fifopidproxy,"fifo.%d",pidproxy);
    if ((dtemp=open(temp,O_RDWR|O_CREAT))<0) {//lo abre para escritura y lectura
        perror("PROXY :ERRORR en abir temp");
        exit(-1);
    }
    printf("PROXY %d: leemos de la STDIN_FILENO y escribimos en el temporal\n",pidproxy);
    //nbytes=read(fd[0],buffer,sizeof(buffer));// lee entrada y escribe en temporal
    nbytes=read(STDIN_FILENO,buffer,sizeof(buffer));// lee entrada y escribe en temporal
    while (nbytes>0) {
        write(dtemp,buffer,nbytes);
        //nbytes=read(fd[0],buffer,sizeof(buffer));
        nbytes=read(STDIN_FILENO,buffer,sizeof(buffer));
    }
    printf("PROXY %d: abrimos el archivo bloqueo\n",pidproxy);
    if ((dbloqueo=open("bloqueo",O_RDWR))<0) {
        perror("PROXY:ERRORR en abir bloqueo");
        exit(-1);
    }
    printf("PROXY %d: bloqueamos\n",pidproxy);
    bloqueo(dbloqueo,F_WRLCK);
    printf("PROXY %d: ya tenemos la pantalla en EM\n",pidproxy);
    printf("PROXY %d: colocamos el puntero del archivo temporal en el origen\n",pidproxy);
    if(lseek(dtemp,0,SEEK_SET)<0){
	perror("PROXY :ERRORR en lseek");
        exit(-1);
    }
    printf("PROXY %d:Leemos del temporal y escribimos en la pantalla\n",pidproxy);
    nbytes=read(dtemp,buffer,sizeof(buffer));//lee de temporal y esribe en pantalla
    while (nbytes>0) {
        printf("%s",buffer);
        nbytes=read(dtemp,buffer,sizeof(buffer));
    }
    bloqueo(dbloqueo,F_UNLCK);
    printf("\nPROXY %d:desbloqueamos\n",pidproxy);
    printf("PROXY %d: eliminamos el archivo temporal %s\n",pidproxy,temp);
    if ((unlink(temp))==-1) {//elimina el archivo
        perror("PROXY:ERRORR en eliminar temp");
        exit(-1);
    }
     printf("PROXY %d: eliminamos el archivo fifopidproxy %s\n",pidproxy,fifopidproxy);
    if ((unlink(fifopidproxy))==-1) {//elimina el archivo
        perror("PROXY:ERRORR en eliminar fifopidproxy");
        exit(-1);
    }
    exit(0);
}
