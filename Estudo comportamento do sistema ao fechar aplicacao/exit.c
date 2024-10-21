#include <stdio.h>
#include <unistd.h>  // Para as funções sleep() e _exit()

int main() {
    sleep(5);  // Espera 5 segundos
    _exit(0);  // O programa termina imediatamente sem limpar buffers
}
