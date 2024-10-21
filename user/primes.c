#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void prime(int pi[2]){
    int buff;
    read(pi[0],&buff,sizeof(buff));
    if(buff==-1)
    exit(0);
    printf("prime %d\n",buff);
    int pe[2];
    pipe(pe);
    if(fork()==0){
        close(pe[1]);
        close(pi[0]);
        prime(pe);
    }
    else{
        close(pe[0]);
        int tmp;
        while(read(pi[0],&tmp,sizeof(tmp))&&tmp!=-1){
        if(tmp%buff!=0){
            write(pe[1],&tmp,sizeof(tmp));
        }
        } 
        close(pi[0]);
         tmp=-1;
        write(pe[1],&tmp,sizeof(tmp));
        close(pe[1]); 
        wait(0);
    }
    exit(0);
}
int 
main(int argc,char* argv[]){
    int fd[2];
    pipe(fd);
    if(fork()==0){
        close(fd[1]);
        prime(fd);
    }
    else{
        close(fd[0]);
        int i=2;
        for(;i<36;i++){
            write(fd[1],&i,sizeof(i));
        }
        i=-1;
        write(fd[1],&i,sizeof(i));
    } 
    wait(0);
    exit(0);//return 会报错
}