   /**************************************************************************/
  /*                        monCp.c                                         */
 /* par Remi Bourgeon   DUT INFO ASPE      TP1     le 07/O3/2017           */
/**************************************************************************/

/* Pour strlen() */
#include <string.h>
/* Pour perror() */
#include <unistd.h>
#include <stdio.h>
/* Pour malloc() et exit() */
#include <stdlib.h>

/* Pour les flags de la commande open */
#include <fcntl.h>

int main(int argc, char const *argv[]) {
    
    int handle1, handle2;
    
    int taille;
    
    int quantiteLue, quantiteEcrite;
    
    char *buffer;
    
    /* Deux erreurs pour le nombre d'arguments */
    if (argc < 4) {
        perror("Pas assez d'arguments");
        exit(-1);
    }
    
    if (argc > 4) {
        perror("Trop d'arguments");
        exit(-1);
    }
    
    /* Faire extraction de caractères pour avoir un nombre entier correct */
    taille = ((int) *argv[3])-48;
    
    buffer = (char * ) malloc (sizeof(char)*taille);
    
    /* Ouverture du premier fichier */
    handle1 = open(argv[1], O_RDONLY);
    
    if (handle1 < 2) {
        perror("Probleme a l'ouverture du fichier de ref ");
        exit(-1);
    }
    
    /* Boucle pour ouvrir le deuxième fichier, et si besoin, le créer */
    do {
        handle2 = open(argv[2], O_WRONLY);
        
        if (handle2 < 2) {
            write(1, "Le fichier de destination n'existe pas\nCreation du fichier\n", 61);
            open(argv[2], O_CREAT, S_IRWXU);
            close(handle2);
        }
    } while(handle2 < 2);
    
    do {
        /* Copie contenu du fichier dans le buffer */
        quantiteLue = read(handle1, buffer, taille);
        
        if (quantiteLue < 0 /*|| quantiteLue > taille */){
            perror("Probleme a la lecture");
            exit(-1);
        }
        
        /* Copie du buffer dans le fichier de destination */
        quantiteEcrite = write(handle2, buffer, quantiteLue);
        if (quantiteEcrite != quantiteLue /*|| quantiteEcrite > quantiteLue*/) {
            perror("Probleme a l'ecriture");
            exit(-1);
        }
    } while(quantiteLue == taille);
    
    close(handle1);
    close(handle2);
    
    return 0;
}
