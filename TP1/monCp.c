   /**************************************************************************/
  /*                        monCp.c                                         */
 /* par Remi Bourgeon   DUT INFO ASPE      TP1     le 10/O3/2017           */
/**************************************************************************/

/* Pour strlen() */
#include <string.h>
/* Pour perror() */
#include <unistd.h>
#include <stdio.h>
/* Pour exit() */
#include <stdlib.h>

/* Pour open()*/
#include <fcntl.h>

#define CHAR_TAB_MAX 10000

int main(int argc, char const *argv[]) {
    
    int descripteur1, descripteur2;
    
    int quantiteLue, quantiteEcrite;
    
    char buffer[CHAR_TAB_MAX];
    
    /* Deux erreurs pour le nombre d'arguments */
    if (argc < 3) {
        perror("Pas assez d'arguments");
        exit(-1);
    }
    
    if (argc > 3) {
        perror("Trop d'arguments");
        exit(-1);
    }
    
    /* Ouverture du premier fichier */
    descripteur1 = open(argv[1], O_RDONLY);
    
    if (descripteur1 < 2) {
        perror("Probleme a l'ouverture du fichier de ref ");
        exit(-1);
    }
    
    /* Boucle pour ouvrir le deuxième fichier, et si besoin, le créer */
    do {
        descripteur2 = open(argv[2], O_WRONLY);
        
        if (descripteur2 < 2) {
            write(1, "Fichier non existant\nCreation ...\n", 34);
            open(argv[2], O_CREAT, S_IRWXU);
            close(descripteur2);
        }
    } while(descripteur2 < 2);
    
    /* On boucle tant qu'un buffer entier est nécessaire pour copier le contenu du fichier */
    do {
        /* Copie contenu du fichier dans le buffer */
        quantiteLue = read(descripteur1, buffer, CHAR_TAB_MAX);
        
        if (quantiteLue < 0){
            perror("Probleme a la lecture");
            exit(-1);
        }
        
        /* Copie du buffer dans le fichier de destination */
        quantiteEcrite = write(descripteur2, buffer, quantiteLue);
        if (quantiteEcrite != quantiteLue) {
            perror("Probleme a l'ecriture");
            exit(-1);
        }
    } while(quantiteLue == CHAR_TAB_MAX);
    
    close(descripteur1);
    close(descripteur2);
    
    return 0;
}
