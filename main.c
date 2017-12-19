#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "instance.h"




void core(int argc, char *argv[]){
    Parameters p;
    p.filePath = argv[1];
    p.code = argv[2];
    p.method = argv[3];

    printf("Filepath : %s \n",p.filePath);
    printf("Code : %s \n",p.code);
    printf("Method : %s \n",p.method);

    char dest[1024];
    strcpy(dest,p.filePath);
    strcat(dest,"/_mknapcb1_res.txt");
    printf("%s\n",dest);

    FILE *file = fopen(dest,"r");
    readLine(file);
    readLine(file);
    readLine(file);
    readLine(file);
    if(file != NULL){
        Instance_createInstance(file);
        fclose(file);
    }
}

time_t timer_start(){
    return clock();
}

double timer_getTime(time_t timer){
    double timeElapsed;
    timeElapsed = (clock()-timer)/(double)CLOCKS_PER_SEC;
    return timeElapsed;
}

int main(int argc, char *argv[])
{
    time_t timer;

    timer = timer_start();
    core(argc,argv);
    //sleep(1);
    printf("TIME : %fs\n",timer_getTime(timer));

    /*for(int i=0; i<1000000;i++){
            for(int j=0; j<1000;j++){}
    }*/

    printf("TIME : %fs\n",timer_getTime(timer));

    //FILE *file = fopen(dest,"r");

    //printf("Hello world!\n");

    return 0;
}
