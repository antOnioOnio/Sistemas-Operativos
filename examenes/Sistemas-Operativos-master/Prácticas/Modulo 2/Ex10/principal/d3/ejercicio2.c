#include<sys/types.h>  	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>


int main(int argc, char *argv[]) {
    int i=1;
    int fd1;//descriptor de archivo del primer archivo
    int fd2;//descriptor de archivo del segundo archivo
    int tam=10;
    char buf1[80];
    char *name;
    if(argc>=2) {
        name=argv[1];
    }
    if(argc<2) {//si no sele para un pathname, se debe utilizar la entrada estandarpara recogerlo
        char name2[20];
	printf("intruduce el pathname: ");
        scanf("%s",name2);
        name=name2;
    }

    if((fd1=open("archivo2",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
        printf("\nError %d en open",errno);
        perror("\nError en open");
        exit(-1);
    }
    if((fd2=open(name,O_RDONLY,S_IRUSR|S_IWUSR))<0) {
        printf("\nError %d en open",errno);
        perror("\nError en open");
        exit(-1);
    }
    while(read(fd2,buf1,80)>0) {
        char bloc[tam];
        sprintf(bloc,"\nBloque %d\n",i);//en el vector de char bloc metemos  Bloque i//siendo i un numero que va ++
        if(write(fd1,bloc,tam) ==-1) {
            perror("\nError en write");
            exit(-1);
        }
        if(write(fd1,buf1,80) ==-1) {
            perror("\nError en write");
            exit(-1);
        }
        if(i>=10 && i<100) tam=11;
        if(i>=100) tam=12;
        i++;
    }
}
