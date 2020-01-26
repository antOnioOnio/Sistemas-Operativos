#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>



int main(int argc, char *argv[]){
	struct stat atributos;
	int diferencia=0,actual=0,aux=0,aux2;
	
	if(argc > 1){
		
		lstat(argv[1],&atributos);
		actual=atributos.st_size;
		diferencia=actual;
		
		while(true){
			
			lstat(argv[1],&atributos);
			actual=atributos.st_size;
			aux=(actual - diferencia);
			
			if(aux2!=actual){
				printf("%10d\n",aux);
			}
			aux2=actual;
		}

		
	}else{
		perror("\nError en num. de param.");
	}
	
	


}
