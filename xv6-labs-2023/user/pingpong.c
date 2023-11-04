#include "kernel/types.h"
#include "user/user.h"

int pipes[2]; // descriptor
char buffer[1];

int main(int argc, char *argv[])
{
    //Check if the required command is provided as argument
    if(argc > 1 )
    {
        fprintf(2,"Usage: pingpong \n");
        exit(1);

    }

    // Create a pipe
    pipe(pipes); 

    if(!fork())
    {
        // Child process
        read(pipes[0],buffer,1);
        printf("%d: received ping\n",getpid());
        write(pipes[1],buffer,1);
    }

    else 
    {
        // Parent process
        write(pipes[1],buffer,1);
        wait(0);
        read(pipes[0],buffer,1);
        printf("%d: received pong\n",getpid());
    }
    exit(0);
}
