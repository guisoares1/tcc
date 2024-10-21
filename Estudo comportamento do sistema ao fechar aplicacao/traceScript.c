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
    scanf("%s", appname);

    // Obtém o PID do processo principal
    char command[200];
    sprintf(command, "pgrep -o %s", appname);
    //sprintf(command, "pidof %s", appname);
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
    /* Versao para injetar o pid direto
    char appname[100];
    printf("Digite o nome do aplicativo: ");
    scanf("%s", appname);
    int main_pid;
    printf("Pid ");
    scanf("%d", &main_pid);
    */
    printf("PID %d\n",main_pid);

    char buffer_size[20];
    sprintf(buffer_size, "%d", 250);  // Definido para 250 KB

   // Desliga o tracing e limpa o buffer
    system("echo 0 > /sys/kernel/debug/tracing/tracing_on");
    system("echo > /sys/kernel/debug/tracing/trace");

    // Define o tracer como "function" (chamadas de função)
    system("echo function > /sys/kernel/debug/tracing/current_tracer");

    // Define o tamanho do buffer circular
    char buffer_command[1000];
    sprintf(buffer_command, "echo %s > /sys/kernel/debug/tracing/buffer_size_kb", buffer_size);
    system(buffer_command);

    // Define o PID do processo a ser monitorado
    char pid_command[100];
    sprintf(pid_command, "echo %d > /sys/kernel/debug/tracing/set_ftrace_pid", main_pid);
    system(pid_command);

    // Ativa o tracing
    system("echo 1 > /sys/kernel/debug/tracing/tracing_on");
    

    // Aguarda a aplicação finalizar
    while (kill(main_pid, 0) == 0) {
        sleep(1);
    }
    // Move o conteúdo do buffer de trace para o arquivo de saída na pasta atual
    char output_file[100];
    sprintf(output_file, "trace_%s_fechamento_normal.txt", appname);  
    char move_command[200];
    sprintf(move_command, "cat /sys/kernel/debug/tracing/trace > %s", output_file);
    system(move_command);
    
    system("echo 0 > /sys/kernel/debug/tracing/tracing_on");
    
    // Exibe uma mensagem ao final da execução
    printf("Trace gravado no arquivo %s\n", output_file);
  

    printf("Rastreamento do processo %d e seus filhos concluído.\n", main_pid);


    return 0;
}
