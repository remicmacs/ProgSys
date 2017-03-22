   /**************************************************************************/
  /*                        monCat.c                                        */
 /* par Remi Bourgeon   DUT INFO ASPE      TP1     le 10/O3/2017           */
/**************************************************************************/

/* Pour strlen() */
#include <string.h>
/* Pour perror() */
#include <unistd.h>
#include <stdio.h>

#define MAX_STR 80

int main(int argc, char const *argv[], char *argenv[]) {
    int errorInt;
    
    errorInt = write(1, "Entrez une chaine de char : ", strlen("Entrez une chaine de char : ") );
    
    char chaineLue[MAX_STR+1];
    
    /* On donne à read le nombre de char maximum lus */
    
    errorInt = read(0, chaineLue, 80);
    if (errorInt >= 80 || errorInt < 0){
        /* Si la taille lue atteint ou excède la taille du buffer, on vide le buffer et on signale une erreur*/
        perror("Erreur de lecture dans stdin ");
        while(getchar() != '\n');
    }
    
    /* Ajout d'une marque de fin pour tronquer une chaine éventuelle trop longue */
    chaineLue[80] = '\0';
    
    if (write( 1 ,"Chaine lue :\t", strlen("Chaine lue :\t")) != strlen("Chaine lue :\t") ) {
        perror("Probleme d'ecriture dans stdout ");
    }
        
    if (write( 1 ,chaineLue, strlen(chaineLue)) != strlen(chaineLue) ) {
        perror("Probleme d'ecriture dans stdout ");
    }
    
    return 0;
}
