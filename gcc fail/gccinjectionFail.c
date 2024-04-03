#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    FILE *fp;
    char ch;
    printf("PID %d\n", getpid());
    char command[256];
    // Tente escrever 1 no arquivo /proc/$$/make-it-fail
    sprintf(command, "echo 1 | sudo tee /proc/%d/make-it-fail", getpid());
    system(command);

   // Compilar um arquivo usando GCC
    printf("Compilando arquivo...\n");
    system("gcc -o gccinjectionFail gccinjectionFail.c");
    
    printf("Continuou ou fechou...\n");

    return 0;
}

