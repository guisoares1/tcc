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

    // Configuração do tracing
    system("echo 0 > /sys/kernel/debug/tracing/tracing_on");
    system("echo > /sys/kernel/debug/tracing/trace");
    system("echo function > /sys/kernel/debug/tracing/current_tracer");

    // Define tamanho do buffer (250 KB)
    char buffer_command[100];
    snprintf(buffer_command, sizeof(buffer_command), 
             "echo 250 > /sys/kernel/debug/tracing/buffer_size_kb");
    system(buffer_command);

    // Define PID para monitoramento
    char pid_command[100];
    snprintf(pid_command, sizeof(pid_command),
             "echo %d > /sys/kernel/debug/tracing/set_ftrace_pid", main_pid);
    system(pid_command);

    system("echo 1 > /sys/kernel/debug/tracing/tracing_on");
    
    // Verifica se deseja injetar em todos os PIDs
    char tipoInjecao[2]; // Buffer corrigido para 1 caractere + '\0'
    printf("Deseja injetar em todos os pids do app? (1 - Sim/ 2 - Nao) ");
    scanf("%1s", tipoInjecao); // Limita a 1 caractere
    
    if (strcmp(tipoInjecao, "1") == 0) { // Comparação correta
        sprintf(command, "pgrep %s", appname);
        fp = popen(command, "r");
        if (fp == NULL) {
            printf("Erro ao obter os PIDs da aplicação.\n");
            return 1;
        }

        int pid;
        while (fscanf(fp, "%d", &pid) == 1) {
            char fail_command[100];
            snprintf(fail_command, sizeof(fail_command),
                     "echo 1 | sudo tee /proc/%d/make-it-fail", pid);
            system(fail_command);
        }
        pclose(fp);
    } else {
        char fail_command[100];
        snprintf(fail_command, sizeof(fail_command),
                 "echo 1 | sudo tee /proc/%d/make-it-fail", main_pid);
        system(fail_command);
    }

    // Aguarda o processo finalizar
    while (kill(main_pid, 0) == 0) {
        sleep(1);
    }

    // Salva o trace em um arquivo
    char output_file[150]; // Tamanho aumentado para evitar overflow
    snprintf(output_file, sizeof(output_file), "trace_%s.txt", appname);
    char move_command[200];
    snprintf(move_command, sizeof(move_command),
             "cat /sys/kernel/debug/tracing/trace > %s", output_file);
    system(move_command);
    
    system("echo 0 > /sys/kernel/debug/tracing/tracing_on");
    
    printf("Trace gravado no arquivo %s\n", output_file);
    printf("Rastreamento do processo %d e seus filhos concluído.\n", main_pid);

    return 0;
}
