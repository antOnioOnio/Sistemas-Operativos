#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
char buf1[100];
char buf2[100];
char salto_linea[2]="\n";
char a[100];
int i = 0;
int fdEntrada,fdSalida, wachado;
int numchar=1;

int main(int argc, char *argv[]){


  //Abrimos el archivo de entrada
  if( (fdEntrada=open(argv[1],O_RDONLY)) < 0) {
      printf("\nError %d en open",errno); //Imprimir por pantalla el error
      perror("\nError en open");
      exit(-1);
  }

  //Abrimos el archivo de salida, es decir lo creamos
  if( (fdSalida=open("salida.txt",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR)) < 0 ) {
      printf("\nError %d en open",errno); //Imprimir por pantalla el error
      perror("\nError en open");
      exit(-1);
  }

  sprintf(a,"Bloque numero--> %d\n\n",i);
  write(fdSalida,a,strlen(a));

  while (read(fdEntrada,buf1,1) != 0){
    if (numchar%80==0){
        write(fdSalida,  salto_linea, strlen(salto_linea));
        write(fdSalida,  salto_linea, strlen(salto_linea));
        i++;
       // write(fdSalida, salto_linea, strlen(salto_linea));
        sprintf(a,"Bloque numero--> %d\n",i);
        //Escribimos el bloque
        write(fdSalida,a,strlen(a));
        
    }    
    
    write(fdSalida,buf1,strlen(buf1)) ;

    numchar++;
  }

  
    if ( lseek(fdSalida, 0, SEEK_SET) < 0){
        perror("\nError en lseek");
		exit(-1);
    }
	sprintf(buf2, "\n El número de bloques es %d\n", i);
	write(fdSalida, buf2,strlen(buf2));


  close(fdEntrada);
  close(fdSalida);
  return 0;
}
