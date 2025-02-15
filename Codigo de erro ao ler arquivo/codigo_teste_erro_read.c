#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    FILE *fp;
    char ch = '$';
    printf("PID %d\n", getpid());
    char command[256];
    sprintf(command, "echo 1 | sudo tee /proc/%d/make-it-fail", getpid());
    system(command);
    
    // Tente ler o arquivo tmpmnt/testfile.txt
    int fd;
    fd = open("tmpmnt/testfile.txt", O_RDONLY);
    printf("Read return: %d \n", read(fd, &ch, 1) );
    printf("Ch read: %c\n",  ch );
    perror(NULL);
    printf("Errno: %d\n", errno);
    while (read(fd, &ch, 1) > 0)
        printf("%c", ch);
    close(fd);

    return 0;
}

