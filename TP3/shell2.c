#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "ligne_commande.h"

/* Détecter la commande de sortie, de changement de dossier ou d'exportation de variable d'environnement */
int exitShell(char * b);
int chDir(char * cmd);
int exportEnv(char * cmd);
void prompt();

int main(int argc, char** argv, char** argenv){
    pid_t child_pid;
    int child_status;
    
    while(1){
        prompt();
        char ** ligne = lis_ligne();
        
        if (!ligne_vide(ligne)){
            fflush(stdin);
            if (exitShell(ligne[0])){
                exit(0);
            }
            
            if (chDir(ligne[0])){
                
                // Si la primitive cd est appelée on appelle chdir
                chdir(ligne[1]);
                
                /* Et on change la variable qui stocke le chemin du répertoire courant */
                char * cwd = (char *) calloc(255,1);
                getcwd(cwd,255);
                setenv("PWD",cwd,1);
            } else {
                
                if(exportEnv(ligne[0])){
                    separeE
                    
                }else{
                    child_pid = fork();
                    
                    // On est chez le fils
                    if(child_pid == 0 ){
                        /* La commande demandée prend la place du processus fils */
                        execvp(ligne[0], ligne);
                        
                        /* Affichage de l'erreur : certes mais ça plante grave */
                        
                        /*switch (errno) {
                        case (E2BIG):
                        perror("Trop d'arguments");
                        break;
                        case (EACCES):
                        perror("Permission denied");
                        break;
                        case (EINVAL):
                        perror("Argument incorrect");
                        break;
                        case (ENOENT):
                        perror("No such file or directory");
                        break;
                        default :
                        perror("Erreur de commande inconnue");
                        break;
                    }*/
                    exit(-1);
                } else {
                    waitpid(child_pid, &child_status, 0);
                }
                }
            }
        }
    }
    return 0;
}

void prompt(){
    char finPr[] = " $ ";
    char * userName = (char * )calloc(strlen(getenv("USER")),1);
    strcpy(userName, getenv("USER"));
    char * userNameAt = strcat(userName, "@");
    char * userNameAtDirectory = strcat(userNameAt, getenv("PWD"));
    char * fullprompt = strcat(userNameAtDirectory, finPr);
    write(1, fullprompt, strlen(fullprompt));
    fflush(stdout);
}

int exitShell(char * cmd){
    return (strcmp(cmd, "exit") == 0) ? 1 : 0 ;
}

int chDir(char * cmd){
    return (strcmp(cmd, "cd") == 0) ? 1 : 0;
}

int exportEnv(char * cmd){
    return (strcmp(cmd, "export") == 0) ? 1 : 0 ;
}
