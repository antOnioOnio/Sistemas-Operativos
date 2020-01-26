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
    /* EEEEXTEERMINAAATE! */
    while((pid = waitpid(-1, &status, WNOHANG )) > 0);
    //printf("signal received-------->%d\n", sig);
    //printf("Pid received-------->%d\n\n", pid);
    
}

int main(int argc, char *argv[]){
    
    if( argc != 2){
        printf("Correcto uso del programa--> ./Explorar <directorio>\n");
        exit(-1);
    }

    pid_t pid;
    char *archivoRegular;
    int fd[2], fdERROR;
    char datos[1054];
    char *parametrosHijo[5];
    char *line = NULL;
    struct stat atributos;
    struct dirent *directorio;
    DIR *dir;
    struct sigaction mSigaction;
    // sigset_t conjunto_mascaras;

    // inicializamos parametros
    archivoRegular = argv[1];

    parametrosHijo[0] = "cut";
    parametrosHijo[1] = "-d:";
    parametrosHijo[2] = "-f6";
    parametrosHijo[3] = "/etc/passwd";
    parametrosHijo[4] = NULL;


    pipe(fd);

    if ( ( pid = fork()   ) == -1){
        fprintf(stderr, " Could not create a child");
        exit(-1);
    }

    if ( pid == 0){
       close(fd[0]); // --> cerramos lectura

        // Abrimos nuestro archivo regular 
       if ( (fdERROR=open(archivoRegular , O_CREAT | O_RDWR,0666) <0)){
            printf("\nError %d en open",errno);
            perror("\nError en open");
            exit(-1);
       } 
       // redirigimos las salidas
       dup2(fdERROR, STDERR_FILENO);
       dup2( fd[1] , STDOUT_FILENO);

        //printf("hijo ejecuta exec  \n");
        
        execlp("cut", "cut", "-d:", "-f6", "etcpasswd.txt", NULL);

    }
    else {

        // somos el padre, establecemos el manejador de señal
        // memset(&mSigaction, 0, sizeof(mSigaction));
         sigemptyset(&mSigaction.sa_mask);
         mSigaction.sa_flags = 0;
         mSigaction.sa_handler = myHandler;

        if ((sigaction(SIGCHLD, &mSigaction, NULL)) < 0){
            perror("error en sigaction");
            exit(1);
        }

        // signal(SIGCHLD, myHandler);
        close(fd[1]); // ---> cerramos escritura
        dup2(fd[0],STDIN_FILENO);
    
      
        char line[11];
        while((read(fd[0],&line, sizeof(line))) > 0){
            line[10] = '\0';

            if ( stat(line, &atributos) < 0 ){
                printf("\nError al intentar acceder a los atributos de \n");
                perror("\nError en lstat");
                exit(EXIT_FAILURE);
            }

            if ( S_ISDIR(atributos.st_mode))
            {

                if ( (dir = opendir(line)) != NULL )
                {

                    if ( chmod ( line, (atributos.st_mode | S_IRWXU) < 0)){
                        perror("Error en chmod para");
                        exit(EXIT_FAILURE);
                    }

                    printf("___DIRECTORIO___%s\n", line);

                    while ( (directorio = readdir(dir)) != NULL)
                    {
                        if (strcmp(directorio->d_name, ".") == 0 || strcmp(directorio->d_name, "..") == 0)
                        continue;

                        printf("Nombre de lo que hay dentro--->%s \n", directorio->d_name);
                        
                        if ( (strcmp( directorio->d_name, "espdiff") >0))
                        {
                            
                        }else {
                            char direccion[200];
                            sprintf(direccion, "%s/espdiff", line);
                            printf("En %s existe el archivo buscado\n\n\n", line );
                            execlp("cat", "cat", direccion, NULL);
                        }
                    
                    }


                }else {
                    perror("DIRECORIO NULO");
                }

            }else {
                perror("No es directorio");
            } 


        }
 
    }

    return 0;


}