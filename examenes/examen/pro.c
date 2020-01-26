#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <math.h>
#include <dirent.h>
#include <limits.h>

void myHandler(int sig)
{
    pid_t pid;
    int status;

    printf("Hijo finalizado \n");
    
}


int main(int argc, char *argv[])
{


    int fd[2];
    pid_t pid;
    char result[400];
    char buff[10];
    char letra;
    int fdArchivo;
    char archivo[1024];

    DIR *directorioAbierto;
    struct dirent *directorio;
    struct stat atributos;
    struct sigaction mSigaction;

    //---------------------------------primer ejercicio--------------------------------------------------
    pipe(fd);

    if ((pid = fork()) == -1)
    {
        fprintf(stderr, " Could not create a child");
        exit(-1);
    }



    if (pid != 0) 
    {

        close(fd[0]); // -----Cerramos el descriptor de lectura, solo vamos a escribir
       
       
        if ( (directorioAbierto = opendir(".")) == NULL)
        {
            perror("error en opendir"),exit(-1);
        }

        while ( (directorio = readdir(directorioAbierto)) != NULL)
        {
            if (strcmp(directorio->d_name, ".") == 0 || strcmp(directorio->d_name, "..") == 0)
                continue;
                snprintf(archivo,sizeof(archivo),  "./%s", directorio->d_name);
                //  snprintf(path, sizeof(path), "%s/%s", pathname, entry->d_name);

                if ( stat(archivo, &atributos) < 0)
                {
                    printf("\nError al intentar acceder a los atributos de %s\n", directorio->d_name);
                    perror("\nError en lstat");
                    exit(EXIT_FAILURE);
                }

                if(S_ISREG(atributos.st_mode))
                {
                    
                    printf("NUMERO DE INODO %ld\n", atributos.st_ino);
                    printf("UUID %d\n", atributos.st_uid); 

                    if ( ! (atributos.st_mode & S_IRUSR)){
                        if (   (chmod(directorio->d_name,S_IRUSR ) <0  )){
                            perror("\nError en chmod para archivo1");
                            exit(-1);
                        }
                    }
                        write(fd[1],directorio->d_name , strlen(directorio->d_name));
                        write(fd[1], "\n", 2);
                  //  printf("Nombre Archivo %s\n", directorio->d_name);

                }            
    

        }

    }
    // hijo   
    else {

        sigemptyset(&mSigaction.sa_mask);
        mSigaction.sa_flags = 0;
        mSigaction.sa_handler = myHandler;

        if ((sigaction(SIGPIPE, &mSigaction, NULL)) < 0){
            perror("error en sigaction");
            exit(1);
        }


         close(fd[1]);

        char a;
        char buff[20];
        sprintf(buff, "");
        
        while ((read(fd[0], &letra, sizeof(letra))) > 0){
            if(letra != '\n'){ 
                sprintf(buff + strlen(buff), "%c", letra);
            }else {

                // printf("Mostrando contenido \n");
                printf("%s\n", buff);
                // execlp("cat", "cat", buff, NULL);
                sprintf(buff , "");
            
            }
        }



    }



    return 0;

}
