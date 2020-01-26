#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char * argv[]){
  if(argc != 3){
    printf("Arg. erroneos");
    exit(1);
  }
  char buffer[50];
  int fdent, fdsal, pid, fd[2],bytes;

  fdent = STDIN_FILENO;
  fdsal = STDOUT_FILENO;
  if(strcmp(argv[1], "-")){
    if((fdent = open(argv[1], O_RDONLY)) < 0)perror("1"),exit(1);
  }
  if(strcmp(argv[2], "-")){
    if((fdsal = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT,0666)) < 0)perror("2"),exit(1);
  }
  if (pipe(fd) < 0)perror("3"),exit(1);
  if((pid = fork()) < 0)perror("4"),exit(1);
  else if (pid == 0){
    close(fd[0]);
    while((bytes = read(fdent, buffer, 50*sizeof(char))) > 0)
      if(write(fd[1],buffer,bytes * sizeof(char)) < 0)perror("5"),exit(1);
  }
  else{
    close(fd[1]);
    while((bytes = read(fd[0], buffer, 50*sizeof(char))) > 0)
    if(write(fdsal, buffer, bytes * sizeof(char)) < 0)perror("6"),exit(1);
  }
}
