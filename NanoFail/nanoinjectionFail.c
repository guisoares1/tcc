#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    FILE *fp;
    pid_t p;
    int status;
    char ch;
    printf("PID %d\n", getpid());
    char command[256];
    // Tente escrever 1 no arquivo /proc/$$/make-it-fail
    sprintf(command, "echo 0 | sudo tee /proc/%d/make-it-fail", getpid());
    system(command);

   // Compilar um arquivo usando GCC
   // printf("Compilando arquivo...\n");
    
   // system("nano resultados.txt");
     p = fork();
     if(p == 0){
        int x = execl("/usr/bin/nano","nano","./resultados.txt");
        printf("Resultado exec %d\n", x);
        
     }else if (p>0){
        sleep(1);
        sprintf(command, "echo 1 | sudo tee /proc/%d/make-it-fail", p);
        system(command);
        if (waitpid(p, &status, 0) > 0) 
        {
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
	              printf("\nSucces\n");
            } else if (WIFEXITED(status) && WEXITSTATUS(status)) 
              {
               if (WEXITSTATUS(status) == 127) {
       		   printf("\nexecl() Failed\n");
               } else {
                       printf("\nThe program terminated normally, but returned a non-zero status\n");
                       exit(0);
        	       switch (WEXITSTATUS(status)) 
                       {
               	                 /* handle each particular return code that the program can return */
                       }
                     }
             } else {
                          printf("\nthe program didn't terminate normally\n");
                       }
        }else {
        	printf("\nwaitpid() failed\n");
              }
        }
      
    
    
   // printf("Continuou ou fechou...\n");

    return 0;
}

