#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find(char*path,char*target){
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd=open(path,0))<0){
        printf("can not open");
        return ;
    }
    if(fstat(fd,&st)<0){
        printf("can not stat");
        return ;
    }
    switch(st.type){
        case T_FILE:
        if(strcmp(path+strlen(path)-strlen(target),target)==0){
            printf("%s\n",path);
        }
        break;
        case T_DIR:
        if(strlen(path)+1+DIRSIZ+1>sizeof(buf)){
            printf("find path too long");
            break;
        }
        strcpy(buf,path);
        p=buf+strlen(buf);
        *p++='/';
        while(read(fd,&de,sizeof(de))==sizeof(de)){
            if(de.inum==0){
                continue;
            }
            memmove(p,de.name,DIRSIZ);
            p[DIRSIZ]=0;
            if(stat(buf,&st)<0){
                printf("find can not stat");
                continue;
            }
            if(strcmp(buf +strlen(buf)-2,"/.")&&strcmp(buf+strlen(buf)-3,"/.."))
            find(buf,target);
        } 
        break;
    }
   close(fd);
}
int 
main(int argc,char *argv[]){
    if(argc<3){
        printf("find error");
        exit(1);
    }
    char target[255];
    target[0]='/';
    strcpy(target+1,argv[2]);
    find(argv[1],target);
    exit(0);
}