#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
    
  	char buf[80];
	int nbytes;
	while ((nbytes=read(0,buf,sizeof(buf)))>0){
		if(write(1,buf,nbytes)< 0) {
			perror("\nError en segundo write");
			exit(-1);
		}
	
	}
        
        
    
    return(0);
}

