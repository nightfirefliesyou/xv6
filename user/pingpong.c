#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int 
main(int argc,char *argv[]){
    int first[2];
    int second[2];
    char buff[255];
     if(pipe(first)<0||pipe(second)<0){
        fprintf(2,"pipe error");
     }
     if(fork()==0){
        close(first[1]);
        if(read(first[0],buff,255)>=0){
            printf("%d: received ping\n",getpid());
        }
        else {
         printf("read error");
         exit(-1);
        }
        close(second[0]);
        if(write(second[1],"pong",sizeof("pong"))<0){
         printf("write error");
         exit(-1);
        }
        exit(0);
     }
     else{
        close(first[0]);
        if(write(first[1],"ping",sizeof("ping"))<0){
       printf("write error");
       exit(-1);
        }
        close(first[1]);
        wait((int*)-1);
        close(second[1]);
        if(read(second[0],buff,255)>=0){
            printf("%d: received pong\n",getpid());
        }
        else {
         printf("read error");
         exit(-1);
        }
        exit(0);
     }
}