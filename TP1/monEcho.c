   /**************************************************************************/
  /*                        monEcho.c                                       */
 /* par Remi Bourgeon   DUT INFO ASPE      TP1     le 10/O3/2017           */
/**************************************************************************/

/* Pour strlen() */
#include <string.h>
/* Pour perror() */
#include <unistd.h>
#include <stdio.h>
/* Pour exit() */
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    int i;
    
    /* Test argc < 2 pour vérifier le nombre d'arguments */
    if (argc < 2){
        perror("Pas assez d'arguments");
        exit(-1);
        }
    
    /* On saute le premier argument = indice 0 du tableau argv qui est le
        nom de la commande */
    for(i=1 ; i < argc ; i++){
        if (write(1 , argv[i], strlen(argv[i])) != strlen(argv[i]) ){
            perror("Probleme d'ecriture dans stdout ");
        } else {
            write(1, " ", strlen(" "));
        }
    }
    return 0;
}
