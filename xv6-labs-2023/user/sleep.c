#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char * argv[])
{
    // Check if the required command is provided as argument
    if(argc !=2)
    {
        fprintf(2,"Usage: sleep  [argv...]\n");
        exit(1);
    }

    int ticks = atoi(argv[1]);

    sleep(ticks);

    exit(0);
}