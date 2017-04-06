#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

/* Définition d'un type booléen */
typedef enum {false, true} bool;

/* Afficher une invite de commande */
void prompt();

/* Détecter la commande de sortie */
bool exitShell(char * b);

void readCmd(char * buff, char * cmd);

int main (int argc, char **argv,char *argenv[]){
    char buffer[255] ;
    char cmd[255] ;
    
    do {
        prompt();
        readCmd(buffer, cmd);
        printf("Commande extraite : %s\n", cmd );
    } while(!exitShell(cmd));
    
}

void prompt(){
    write(1,"User $ \t", strlen("User $ \t"));
    fflush(stdout);
}

bool exitShell(char * cmd){
    return (strcmp(cmd, "exit") == 0) ? true : false ;
}

void readCmd(char * buff, char * cmd) {
    int i = 0;
    read(0, buff, 255);
    
    while (buff[i] != '\n') i++;
    buff[i] ='\0';
    
    
    // printf("Buffer extrait : %s", buff);
    strcpy(cmd, buff);
    // printf("Commande extraite dans readCmd : %s\n", cmd);
}

void parserCmd(char * cmd){
    
}
