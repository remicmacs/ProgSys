  /****************************************************************************//*  shell1.c  10/04/2017 TP SE ASPE                                        */
 /*  Particularités : j'ai implémenté une primitive cd et un prompt dynamique*/
/***************************************************************************/


/******************************************************************************/
/* Les primitives                                                             */
/*  Certaines commandes doivent être gérées par le shell lui-même et pas par  */
/*    ses enfants : par exemple pour la commande cd qui change de répertoire  */
/*    il est possible de la faire exécuter par un execvp via un enfant.       */
/*    mais lorsque la commande aura réussi, l'enfant meurt et le père est     */
/*    toujours dans le même répertoire courant.                               */
/*    C'est le même principe pour la primitive export : si on change les      */
/*    variables d'environnement dans l'enfant, lorsque la commande est        */
/*    finie dans l'enfant, la commande meure avec son environnement modifié   */
/*    et l'environnement du parent est inchangé.                              */
/******************************************************************************/
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
void killZombie(pid_t child_pid_retour, pid_t child_pid, int child_status );

/* Afficher l'invite de commande */
void prompt();

int main(int argc, char** argv, char** argenv){
    pid_t child_pid = 0;
    pid_t child_pid_retour = 0;
    int child_status;
    setenv("INVITE", "none", 1);
    
    while(1){
        /* Tuer les zombies restants au cas où */
        killZombie(child_pid_retour, child_pid, child_status);
        prompt();
        char ** ligne = lis_ligne();
        
        /* On teste si on a entré une ligne vide, pour éviter de passer un tableau vide à une fonction qui essaye de lire une str dans le tableau */
        if (!ligne_vide(ligne)){
            fflush(stdin);
            
            /* On vérifie si l'utilisateur a appelé la primitive exit*/
            if (exitShell(ligne[0])){
                exit(0);
            }
            
            /* On vérifie si l'utilisateur a appelé la primitive cd */
            if (chDir(ligne[0])){
                
                // Si la primitive cd est appelée on appelle chdir
                chdir(ligne[1]);
                
                /* Et on change la variable qui stocke le chemin du répertoire courant */
                char * cwd = (char *) calloc(255,1);
                getcwd(cwd,255);
                setenv("PWD",cwd,1);
            } else {
                
                /* On vérifie si l'utilisateur a appelé la primitive export */
                if(exportEnv(ligne[0])){
                    char * modEnv = separe_egal(ligne[1]);
                    char * key = ligne[1];
                    setenv(key, modEnv, 1);
                    
                /* Sinon on exécute la commande */
                }else{
                    /* L'enfant va se charger d'exécuter une commande non primitive */
                    child_pid = fork();
                    
                    // On est chez le fils
                    if(child_pid == 0 ){
                        /* La commande demandée prend la place du processus fils */
                        execvp(ligne[0], ligne);
                        
                        /* Affichage de l'erreur : perror avait un comportement étrange donc on utilise write pour écrire dans le stream d'erreur directement */
                        switch (errno) {
                        case (E2BIG):
                            write(2, "Trop d'arguments\n", strlen("Trop d'arguments\n"));
                            break;
                        case (EACCES):
                            write(2, "Permission denied\n", strlen("Permission denied\n"));
                            break;
                        case (EINVAL):
                            write(2, "Argument incorrect\n", strlen("Argument incorrect\n"));
                            break;
                        case (ENOENT):
                            write(2, "No such file or directory\n", strlen("No such file or directory\n"));
                            break;
                        default :
                            write(2, "Erreur de commande inconnue\n", strlen("Erreur de commande inconnue\n"));
                            break;
                        }
                        exit(-1);
                    } else {
                        child_pid_retour = waitpid(child_pid, &child_status, WUNTRACED | WCONTINUED);
                    }
                }
            }
        }
    }
    return 0;
}

void killZombie(pid_t child_pid_retour, pid_t child_pid, int child_status){
    /* Si le retour pid du wait n'est pas le pid du child, on infanticide */
    if (child_pid_retour != child_pid) {
        kill(child_pid, SIGKILL);
    }
}

void prompt(){
    char finPr[] = " $> ";
    /* Si on utilise l'invite par défaut*/
    if (strcmp(getenv("INVITE"), "none") == 0) {
        char * userName = (char * )calloc(strlen(getenv("USER")),1);
        strcpy(userName, getenv("USER"));
        char * userNameAt = strcat(userName, "@");
        char * userNameAtDirectory = strcat(userNameAt, getenv("PWD"));
        char * fullprompt = strcat(userNameAtDirectory, finPr);
        write(1, fullprompt, strlen(fullprompt));
    /* Si une invite de commande personnalisée est fixée par l'utilisateur */
    } else {
        char * invite = getenv("INVITE");
        char * promptPerso = strcat(invite, finPr);
        printf("promptPerso = <%s>\n", promptPerso);
        write(1, promptPerso, strlen(promptPerso));
    }
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
