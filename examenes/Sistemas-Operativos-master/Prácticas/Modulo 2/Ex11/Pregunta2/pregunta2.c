
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd1[2];//fd1[1] escritura, fd1[0] lectura
	int salida=0,fds;
    char r0[]="-";
	pipe(fd1);
    if (argc<3) {
        printf("Error falta de argumentos: ./micat f|- d|-\n");
	exit(-1);
    }
    if (strcmp(argv[1],r0)==0) {//entrada-lectura
        //fd1[0]=0;
	//entradaestandar=1;
        printf("redireccion de entrada es la entrada estandar\n");

    }
    else {
        printf("la entrada sera el archivo %s\n",argv[1]);
        if((fd1[0]=open(argv[1],O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR))<0) { //lo abrimos para lectura
            perror("\nError en open");
            exit(-1);
        }
    }
    if (strcmp(argv[2],r0)==0) {
        //fd1[1]=1;
	//salidaestandar=1;
        printf("redireccion de entrada es la salida estandar\n");
    }
    else {
        printf("la salida sera el archivo %s\n",argv[2]);
	salida=1;
        if((fds=open(argv[2],O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
            perror("\nError en open");
            exit(-1);
        }
    }

    // Llamada al sistema para crear un pipe



    if ((pid = fork())<0) {
        perror("Error en fork\n");
        exit(-1);
    }
    if (pid==0) {
	close(fd1[0]);

        
        close(STDOUT_FILENO);

        //Duplicar el descriptor de escritura en cauce en el descriptor
        //correspondiente a la salida estándar (stdout)
        dup(fd1[1]);
      if ((execlp("./leede0escribe1","leede0escribe1",NULL))<0) {
            perror("\nError en el execv");
            exit(-1);
        }
        
    }
    else {
       		//Establecer la dirección del flujo de datos en el cauce cerrando
    // el descriptor de escritura en el cauce del proceso padre.
    
	close(fd1[1]);

    //Redirigir la entrada estándar para tomar los datos del cauce.
    //Cerrar la entrada estándar del proceso padre
    close(STDIN_FILENO);

    //Duplicar el descriptor de lectura de cauce en el descriptor
    //correspondiente a la entrada estándar (stdin)
    dup(fd1[0]);
	if(salida==1){


	dup2(fds,STDOUT_FILENO);
}
    
      if ((execlp("./leede0escribe1","leede0escribe1",NULL))<0) {
            perror("\nError en el execv");
            exit(-1);
        }


    }
    return(0);
}
