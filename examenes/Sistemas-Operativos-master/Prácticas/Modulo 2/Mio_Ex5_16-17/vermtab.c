/*
	Sistemas Operativos - Prueba Modulo 2
	Curso 2º A - Grupo 1
	Apellidos : Barrilao Bensrhir
	Nombre:	Angel
	
	

*/


#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include <signal.h>
#include<dirent.h>


void buscar_dir(DIR *direct, char pathname[], int *reg, int *tamanio){
	struct stat atributos;
	struct dirent *ed;
	DIR *direct_act;
	char cadena[500];
	//Leemos el directorio
	while((ed=readdir(direct)) != NULL){
		// Ignoramos el directorio actual y el superior
		if (strcmp(ed->d_name, ".") != 0 && strcmp(ed->d_name, "..") != 0){
			//En cadena formateamos con %s que indica un string de cadenas
			//le pasamos la ruta y el nombre del archivo
			sprintf(cadena,"%s/%s",pathname,ed->d_name);
			//Verificamos que podamos acceder a los atributos
			if(stat(cadena,&atributos) < 0) {
				printf("\nError al intentar acceder a los atributos de archivo");
				perror("\nError en lstat");
				exit(-1);
			}
			//Si es un directorio ...
			if (S_ISDIR(atributos.st_mode)){
				if ((direct_act = opendir(cadena)) == NULL)//... abrimos el directorio
				printf("\nError al abrir el directorio: [%s]\n",cadena);
				else
				buscar_dir(direct_act, cadena, reg, tamanio);//Y buscamos el directorio
			}else{
				printf("%s %ld \n", cadena, atributos.st_ino);//mostramos por pantalla la ruta y su i-nodo
				if (S_ISREG(atributos.st_mode)){//Si es un archivo
					if (regla1(atributos.st_mode)){//Llamamos a la macro para comprobar los permisos de ejecucion de grupo y otros
						(*reg)++;//Aumentamos el puntero de reg
						(*tamanio)
						+=(int) atributos.st_size;//casteamos tamanio devuelto y lo guardamos
						//en el puntero que apunta a entero llamado tamanio
					}
				}
			}
		}
	}
	closedir(direct);//cerramos el directorio
}





void manejador(int sig){
	 int estado ;
	 wait(&estado); // a NULL si me da igual como acabe. wait para no tener hijos zombi
	 if(estado != 0){
	 	 printf("Finalizado con error "); // muestra mensaje de error al llegar el hijo
	 }
}

int main(int argc, char *argv[])
{
char * usuario=getenv("USER");
char ruta[255]="/etc/mtab";
char recibido[255];
int fd[2];
pid_t PID;
struct sigaction sa;
sa.sa_handler = manejador ; // cuando llegue una senial se ejecuta el manejador.

pipe(fd); // Llamada al sistema para crear un pipe

if ( (PID= fork())<0) {
	perror("\Error en fork");
	exit(-1);
}
if (PID == 0) { // ls
	//Cerrar el descriptor de lectura de cauce en el proceso hijo
	close(fd[0]);

	//Duplicar el descriptor de escritura en cauce en el descriptor
	//correspondiente a la salida estandar (stdout), cerrado previamente en
	//la misma operación
	dup2(fd[1],STDOUT_FILENO);
	if(execlp("grep","grep",usuario,ruta,NULL)){
		perror("Error en la ejecucion de grep");
		exit(-1);
	}

}
else { // sort. Proceso padre porque PID != 0.
	//Cerrar el descriptor de escritura en cauce situado en el proceso padre
	close(fd[1]);

	//Duplicar el descriptor de lectura de cauce en el descriptor
	//correspondiente a la entrada estándar (stdin), cerrado previamente en
	//la misma operación
	dup2(fd[0],STDIN_FILENO);
	if(execlp("cut","cut","-d ","-f",argv[1],NULL)){
		perror("Error en la ejecucion de cut");
		exit(-1);
	}
}


//Comprobamos la señal del hijo
if ((sigaction(EXIT_FAILURE, &sa, NULL)) < 0){
	perror("error en sigaction"),exit(1);
}


//Leemos la salida
read(fd[0],recibido,sizeof(recibido));

switch(argv[1]){

	case 1: printf("\nNombre del dispositivo montado: %s", recibido);
	break;

	case 2: printf("\n Punto de montaje: %s", recibido); 







//HACER POR AQUI
	break;

	case 3: printf("\nTipo de sistema de archivos: %s", recibido);
	break;

	case 4: printf("\nFlags de montaje: %s", recibido);
	break;

}



return(0);
}

