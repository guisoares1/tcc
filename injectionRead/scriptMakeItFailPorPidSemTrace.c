#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char appname[100];
    printf("Digite o nome do aplicativo: ");
    scanf("%99s", appname); // Limita o input para evitar overflow

    // Obtém o PID do processo principal
    char command[200];
    sprintf(command, "pgrep -o %s", appname);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Erro ao executar o comando.\n");
        return 1;
    }

    int main_pid;
    if (fscanf(fp, "%d", &main_pid) != 1) {
        printf("Aplicativo não encontrado.\n");
        pclose(fp);
        return 1;
    }
    pclose(fp);
    
    printf("PID %d\n", main_pid);
    
    char tipoInjecao[2]; // Buffer corrigido para 2 caracteres
    printf("Deseja injetar em todos os pids do app? (1 - Sim/ 2 - Nao) ");
    scanf("%1s", tipoInjecao); // Limita a leitura a 1 caractere
    
    if (strcmp(tipoInjecao, "1") == 0) { // Comparação corrigida
      // Obtém todos os PIDs da aplicação
      sprintf(command, "pgrep %s", appname);
      fp = popen(command, "r");
      if (fp == NULL) {
          printf("Erro ao obter os PIDs da aplicação.\n");
          return 1;
      }

      // Injetar a falha em todos os PIDs
      int pid;
      while (fscanf(fp, "%d", &pid) == 1) {
          char fail_command[100];
          sprintf(fail_command, "echo 1 | sudo tee /proc/%d/make-it-fail", pid);
          system(fail_command);
      }
      pclose(fp);
    } else {
      char fail_command[100];
      sprintf(fail_command, "echo 1 | sudo tee /proc/%d/make-it-fail", main_pid);
      system(fail_command);
    }

    // Aguarda a aplicação finalizar
    while (kill(main_pid, 0) == 0) {
        sleep(1);
    }
   
    printf("Rastreamento do processo %d e seus filhos concluído.\n", main_pid);

    return 0;
}
