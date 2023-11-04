#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void new_process(int pipes[])
{
    int prime;
    int number;

    close(pipes[1]); // Close write port of pipe.
    
    int checkRead = read(pipes[0],&prime,sizeof(int));

    if(checkRead != 4)
    {
        exit(1);
        fprintf(2,"Reading prime number failure.\n");
    }

    else 
    {
        printf("prime %d\n",prime);
    }

    int secondPipes[2];
    pipe(secondPipes); // Create new process to write number.

    if(fork())
    {
        close(secondPipes[0]); // Close read port of second pipe.

        while(read(pipes[0],&number,4) == sizeof(int))
        {   
            if(number % prime != 0)
            {
                int checkWrite = write(secondPipes[1],&number,sizeof(int));
                if(checkWrite != 4 )
                {
                    fprintf(2,"Writing number into pipe is failure.\n");
                    exit(1);
                }
            }

        }

        close(secondPipes[1]);
        close(pipes[0]);
        wait(0);
    }
    else 
    {
        new_process(secondPipes);
    }

}

int main(int argc, char * argv[])
{
    // Create pipe
    int pipes[2];
    pipe(pipes);

    if(fork())
    {
        // Parrent Processing
        close(pipes[0]); // Close read port of pipe

        for(int i = 2 ; i <= 35; ++i)
        {
            
            if( write(pipes[1],&i,sizeof(int)) != sizeof(int))
            {
                exit(1); 
                fprintf(2,"Writing element %d into pipe is failure.\n",&i);
            }

        }

        close(pipes[1]); // Close write port of pipe
        wait(0);
    }
    else
    {
        // Child Processing
        new_process(pipes);
    }

    exit(0);
}

