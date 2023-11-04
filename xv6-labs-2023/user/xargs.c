#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" 
#include <stddef.h> 

char buffer[1024], character;
char *p = buffer;
char *v[MAXARG];
int para, numberBlanks = 0, offset = 0;
int maxArgs = 2; // Maximum number of arguments per command

int main(int argc, char *argv[]) {
    // Check if the required command is provided as argument
    if (argc <= 1) {
        fprintf(2, "Usage: xargs <command> [argv...]\n");
        exit(1);
    }

    // Store the command and arguments in an array
    for (para = 1; para < argc; para++) {
        v[para-1] = argv[para];
    }
    --para;

    // Read input from standard input
    while (read(0, &character, 1) > 0) {
        if (character == ' ' || character == '\t') {
            numberBlanks++;
            continue;
        }

        if (numberBlanks) {
            // Null-terminate the buffer and store the argument
            buffer[offset++] = 0;
            v[para++] = p;
            p = buffer + offset;
            numberBlanks = 0;
        }

        if (character != '\n') {
            buffer[offset++] = character;
        } else {
            // Null-terminate the buffer and store the argument
            v[para++] = p;
            p = buffer + offset;

            // Fork a new process and execute the command with arguments
            if (!fork()) {
                // Limit the number of arguments passed to the command
                v[para] = NULL; // Null-terminate the argument list
                for (int i = 0; i < para; i += maxArgs) {
                    if (!fork()) {
                        exec(v[i], &v[i]);
                        exit(0);
                    }
                    wait(0);
                }
                exit(0);
            }
            wait(0);

            // Reset the argument index
            para = argc - 1;
        }
    }

    exit(0);
}