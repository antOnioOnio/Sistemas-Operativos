#include <stdio.h>
#include <signal.h>

static int i = 0 ;

void myHandler(int mysignal){

  printf("handls with signal %d \n", mysignal);
  i++;

}

int main(int argc, char * argv[])
{


    struct sigaction sa;

    sa.sa_handler = myHandler;

    sigemptyset(&sa.sa_mask);


    signal(SIGTERM, myHandler);

    sa.sa_flags = SA_RESTART;
    
    
    if (sigaction(SIGINT, &sa, NULL) == -1){
        printf("error en el manejador");
    
    }
        
    while(i<3);
    

    return 0;

}


