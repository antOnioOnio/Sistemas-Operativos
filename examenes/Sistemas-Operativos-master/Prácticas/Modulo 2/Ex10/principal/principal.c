#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, char *argv[])
{
    char fsalida[80];
    int fd[2];
    pid_t PID;
    int pid,n,tama=0,directorios=argc-1,i;
    

    pipe(fd); // Llamada al sistema para crear un pipe
    if (argc<2) {
        printf("Servido: Formato: ./servido <nombre_directorio>\n");
        exit(-1);
    }

    for (i=0; i<directorios; i++) {

        if ( (PID= fork())<0) {
            perror("fork");
            exit(1);
        }
        if (PID == 0) {
            pid=getpid();
            sprintf(fsalida,"fsalida.%d",pid);
            close(fd[0]);
            close(STDOUT_FILENO);
            dup(fd[1]);
            if(execlp("./catdir","catdir",argv[i+1],fsalida,NULL)<0) {
                printf("error exec");
                exit(-1);
            }
        }
        else { // sort. Estoy en el proceso padre porque PID != 0
			int buffer[80];
            close(fd[1]);
            close(STDIN_FILENO);
            dup(fd[0]);
            if (read(fd[0],buffer,sizeof(buffer))<0) {
                perror("\nError en el read");
                exit(-1);
            }
            else {
                if((n=strtol(buffer,NULL,10))==0){
					printf("error strtol\n");
				}//transforma el char a int y a decimal
                tama=n+tama;
                printf("El numero de bloques del directorio %s es:%d\n",argv[i+1],n);
				n=0;
				//buffer=NULL;
            }

        }
    }
    printf("El numero de bloques es:%d\n",tama);

    return(0);
}


