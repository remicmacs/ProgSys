#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "ligne_commande.h"


/* Définition d'un type booléen */
typedef enum{false, true} bool;

/* Afficher une invite de commande */
void prompt();

/* Détecter la commande de sortie ou de changement de dossier*/
bool exitShell(char * b);
bool chDir(char * cmd);

int main (int argc, char **argv,char *argenv[]){
    pid_t child_pid;
    int child_status;
    do {
        prompt();
        char ** ligne = lis_ligne();
        
        /* On vérifie s'il y a bien une entrée */
        if (!ligne_vide(ligne)){
            /* En premier lieu on attrappe la condition de sortie */
            if (exitShell(ligne[0])) break;
            //J'ai essayé d'implémenter un cd ..
            if (chDir(ligne[0])){
                
                // Si la primitive cd est appelée on appelle chdir
                chdir(ligne[1]);
                
                /* Et on change la variable qui stocke le chemin du répertoire courant */
                char * cwd = calloc(255,1);
                getcwd(cwd,255);
                setenv("PWD",cwd,1);
            } else {
                
                child_pid = fork();
                
                printf("child pid=%d\n", child_pid );
                
                // le processus enfant
                if(child_pid == 0) {
                    
                    /* La commande demandée prend la place du processus fils */
                    execvp(ligne[0], ligne);
                    
                    /* Si execvp n'est pas correctement exécuté, alors on est toujours ici*/
                    free(ligne);
                    switch (errno) {
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
                    }
                    exit(-1);
                    
                    // On est chez le père
                } else {
                    // On attend le retour du fils
                    // Il y a peut-être des tests à faire.
                    pid_t retourPid = waitpid(child_pid, &child_status, WNOHANG);
                    printf("PID retour fils : %d\n", retourPid);
                }
            }
        }
    } while(true);
    
    return 0;
}

/* Fonction qui met à jour l'invite de commande et qui l'affiche */
void prompt(){
    char * finPr = " $ ";
    char * userName = calloc(strlen(getenv("USER")),1);
    strcpy(userName, getenv("USER"));
    char * userNameAt = strcat(userName, "@");
    char * userNameAtDirectory = strcat(userNameAt, getenv("PWD"));
    char * fullprompt = strcat(userNameAtDirectory, finPr);
    write(1, fullprompt, strlen(fullprompt));
    fflush(stdout);
}

bool exitShell(char * cmd){
    return (strcmp(cmd, "exit") == 0) ? true : false ;
}

bool chDir(char * cmd){
    return (strcmp(cmd, "cd") == 0) ? true : false;
}

void prompt2(){
    
}

void parserCmd(char * cmd){
    
}
