#include <stdio.h>
#include <signal.h>

void myHandler(int mysignal){

  printf("handls with signal %d \n", mysignal);


}

int main(int argc, char * argv[]){

    int i = 0 ;
    signal(SIGTERM, myHandler);

    while(1){
        printf("i = %d \n", i);
        i++;
        sleep(1);
    }


    return 0;
}