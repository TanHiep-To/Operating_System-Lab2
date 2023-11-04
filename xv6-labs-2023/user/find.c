#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void find_all_files(char* dir_path, char* file_name){
    int fd;
    struct stat st;

    if((fd = open(dir_path, O_RDONLY)) < 0){
        printf("Cannot open %s\n", dir_path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf("Cannot stat %s\n", dir_path);
        close(fd);
        return;
    }

    char buff[512];
    strcpy(buff, dir_path);
    char* p = buff + strlen(buff);
    *p++ = '/';
    
    switch(st.type){
    case T_DEVICE:
    case T_FILE:
    {
        char* name = fmtname(dir_path);
        if (strcmp(name, file_name) == 0){
            printf("%s\n", dir_path);
        }
        break;
    }

    case T_DIR:
    {
        if(strlen(dir_path) + 1 + DIRSIZ + 1 > sizeof buff){
            printf("Path too long\n");
            break;
        }
        struct dirent de;

        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, strlen(de.name));
            p[strlen(de.name)] = 0;
            find_all_files(buff, file_name);
        }
        break;
            }
    }
    close(fd);
}

int main(char argc, char* argv[]){
    if (argc < 3){
        fprintf(2,"Usage: find  [argv...]\n");
        exit(1);
    }
    
    if (argc > 3){
        printf("Too much parameters!\n");
        exit(1);
    }
    
    //3 parameters
    find_all_files(argv[1], argv[2]);
    return 0;
}