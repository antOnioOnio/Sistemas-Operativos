// Sobre la recepción de información tras el fin del hijo.
// Estudie qué realiza este programa
#include<sys/types.h>							
#include<unistd.h>		
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
pid_t p;
int dato;
char nada[10];
int valor_exit;


if(setvbuf(stdout,NULL,_IONBF,0)) {
	perror("\nError en setvbuf");
}

if( (p=fork())<0) {
	perror("\nError en el fork");
	exit(-1);
}
else if(p==0) {  	//proceso hijo 
	printf("(Hijo): pid=%d; ppid=%d; Escriba codigo de salida:\n",getpid(), getppid());
	scanf("%s",nada);
	valor_exit = strtol(nada, NULL, 10);
	printf("(Hijo): pid=%d; ppid=%d; Fin\n",getpid(), getppid());
	exit(valor_exit);	
}
//proceso padre

printf("(Padre): Se ha creado hijo con pid %d\n", p);

wait(&dato);		
printf ("(Padre): Fin de hijo de pid: %d\n", 	p );
printf ("(Padre): Valor devuelto  : %d\n",	dato>>8);
printf ("(Padre): Causa de fin    : %d\n", 	dato&0377);
                                   

valor_exit= dato>>8  ;
exit(valor_exit);
}
