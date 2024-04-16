#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char appname[100];
    printf("Digite o nome do aplicativo: ");
    scanf("%s", appname);

    char command[200];
    sprintf(command, "ps aux | grep '%s' | awk '{print $2}'", appname);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Erro ao executar o comando.\n");
        return 1;
    }

    int pid;
    while (fscanf(fp, "%d", &pid) != EOF) {
        char fail_command[50];
        sprintf(fail_command, "echo 1 | sudo tee /proc/%d/make-it-fail", pid);
        system(fail_command);
    }

    pclose(fp);
    return 0;
}

