   /**************************************************************************/
  /*                        lockfile.c                                      */
 /* par Remi Bourgeon   DUT INFO ASPE      TP2     le 24/O3/2017           */
/**************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>

/******************************************************************************/
/* NON DEMANDÉ DANS LES CONSIGNES : mon programme propose la création d'un    */
/*  fichier qui n'existe pas dont le nom est passé en paramètre               */
////////////////////////////////////////////////////////////////////////////////
/* Fonctions de verrouillage : on n'a pas utilisé le paramètre offset car     */
/*   toutes les données manipulées lors de ce TP sont de même taille.         */
/*   Par conséquent le paramètre a été commenté dans les fonctions.           */
////////////////////////////////////////////////////////////////////////////////
/* Test des verrous : pour permettre le test des fonctions de verrouillage    */
/*   l'appel à verrouillage() à été positionné avant le scan du contact à     */
/*   ajouter ou à modifier (voir les commentaires dans le corps du source).   */
/*   cela permet de temporiser le programme et de constater le verrouillage   */
/*   en temps humain. Après les tests les verrous ont été positionnés de      */
/*   manière à ne verrouiller la ressource au strict nécessaire               */
////////////////////////////////////////////////////////////////////////////////
/* Sur les différents types de verrou : le verrou bloquant met le processus   */
/*   demandant un verrou en attente sur la ressource tant que l'autre verrou  */
/*   n'est pas relâché. En revanche le verrou non bloquant permet au code de  */
/*   poursuivre si jamais la ressource est déjà verrouillée, d'où le besoin   */
/*   de tester le retour pour permettre un retour au menu et une éventuelle   */
/*   nouvelle tentative. L'avantage du verrou non bloquant est que le retour  */
/*   au menu principal permet de redemander un verrou sur la bonne position   */
/*   du fichier. Par exemple, avec le verrou bloquant, lorsque deux processus */
/*   veulent ajouter un Contact en fin de fichier, l'ajout du deuxième va     */
/*   venir écraser l'ajout du premier car il obtiendra un verrou sur la même  */
/*   position.                                                                */
/*   Avec un verrou non bloquant on règle ce problème en repassant par le     */
/*   menu principal et en recalculant la position de la fin du fichier en     */
/*   prenant en compte l'ajout de l'autre processus. L'arbitrage des          */
/*   ressources partagées est ainsi mieux géré.                               */
////////////////////////////////////////////////////////////////////////////////
/* Verrou pour les fichiers ouverts en lecture :                              */
/*   Ça ne fonctionne pas et ça produit des erreurs de verrous                */
/*   Notamment pour la création du fichier, il faut créer le fichier, le      */
/*   refermer et l'ouvrir à nouveau                                           */
////////////////////////////////////////////////////////////////////////////////
/* Différence F_TLOCK et F_TEST : F_TEST renvoie systématiquement à la        */
/*    fonction appelante alors que F_TLOCK pose le verrou demandé si la       */
/*    ressource est disponible.                                               */
/******************************************************************************/

#define BUFF_SIZE 30
#define VARIANT_LOCK 2	//0: pas de verrouillage 1: verrouillage bloquant 2: tentative de verrouillage (non bloquant)

typedef struct contact
{
    int id;                		//ID suppose unique
    char name[BUFF_SIZE];        	//Le nom de famille
    char firstName[BUFF_SIZE];   	//Le prenom
}Contact;


int litUneEntree(int fd);
void scanContact(struct contact* unContact);
void affichage(char* fileName);
void ajout(char* fileName);
void affichageEntree(char* fileName);
void modificationEntree(char* fileName);
int verrouillage(int fd/*, int offset*/, int variant);
void deverrouillage(int fd/*, int offset*/);

void usage(char* string)
{
    fprintf(stderr,"Usage: %s fileName\n",string);
    exit(1);
}

int main(int argc, char* argv[])
{
    char fileName[256];
    int choix;

    if(argc<2) usage(argv[0]);

    strcpy(fileName,argv[1]);

    while(1)
    {
        printf("\n === MENU === \n 0-Quitter le programme\n 1-Affichage complet\n 2-Affichage i-ème entrée\n 3-Ajout d'une entrée\n 4-Modification i-ème entrée\n\n ============\n");
        printf("Choix: "); fflush(stdout);
        scanf("%d",&choix);

        switch(choix)
        {
            case 0:     exit(0);

            case 1:     affichage(fileName);
                        break;
            case 2:     affichageEntree(fileName);
                        break;
            case 3:     ajout(fileName);
                        break;
            case 4:     modificationEntree(fileName);
                        break;

            default:    fprintf(stderr,"Choix incorrect\n");
        }
    }

    return(0);
}//fin du main

//Lit une entree struct contact dans le fichier pointé par le descripteur fd
int litUneEntree(int fd)
{
    struct contact entree;
    int quantiteLue = read(fd, &entree, sizeof(Contact));
    if (quantiteLue < 0) {
        perror("Pb de lecture");
        return -1;
        
    /* Si read a moins lu que prévu, on est à la fin du fichier */
    } else if (quantiteLue != sizeof(Contact)) {
        return(-2);
    }
    printf("id: %d\nFirst name: %s\nName: %s\n", entree.id, entree.firstName,entree.name );
	return(0);
}

//Met a jour un contact saisit au clavier
void scanContact(Contact * unContact)
{
    printf("id: ");
    fflush(stdout);
    scanf("%d",&unContact->id);
    printf("First name: ");
    fflush(stdout);
    scanf("%s",unContact->firstName);
    printf("Name: ");
    fflush(stdout);
    scanf("%s",unContact->name);

}//fin de scanContact

//Affiche la totalite du fichier
void affichage(char* fileName)
{
    int fd = open(fileName, O_RDONLY);
    int retour;
    
    if (fd == -1) {
        if (errno == EACCES){
            printf("Vous n'avez pas l'autorisation d'accéder à ce fichier\n");
            exit(-1); // Accès interdit
        }
        else if (errno == ENOENT) {
            /* Le fichier n'existe pas  */
            retour = write(1, "Fichier non existant\n", strlen("Fichier non existant\n"));
            if (retour == -1){
                if (errno == EIO ){
                    printf("Problème d'écriture dans stdio\n");
                    exit(-4) ;
                } else {
                    printf("Problème d'écriture non répertorié\n" );
                    exit(-99);
                }
            }
            close(fd);
            return;
        } else {
            printf("Erreur d'ouverture non répertoriée\n" );
            exit(-3);
        }
    }
    
    do {
        retour = litUneEntree(fd);
    } while(retour == 0);
    
    /* Fin de lecture : test si erreur ou fin de fichier */
    if (retour ==-2) {
        printf("Fin du fichier\n");
    } else if (retour == -1) {
        perror("Pb de lecture détecté");
    }
    close(fd);
}

//Demande a l'utilisateur le numero de l'entree (ieme entree dans le fichier) a afficher puis l'affiche
void affichageEntree(char* fileName)
{
    int entree = 0;
    int retour;
    int positionnement;
    int ok;
    
    /* Entrée utilisateur pour le numéro de l'entrée à lire */
    do {
        retour = write(1,"Choisir le numero de l'entrée à afficher : " , strlen("Choisir le numero de l'entrée à afficher : "));
        
        if (retour == -1){
            if (errno == EIO ){
                printf("Problème d'écriture dans stdio\n");
                exit(-4) ;
            } else {
                printf("Problème d'écriture non répertorié\n" );
                exit(-99);
            }
        }
        fflush(stdout);
        ok = scanf("%d", &entree);
        while (getchar() != '\n');
        
    /* On teste le retour du scanf et la valeur scannée pour éviter d'avoir un négatif*/
    } while(!ok && entree <= 0);
    
    /* Pour que l'entrée numéro 1 du fichier soit bien alignée avec l'adresse de début de fichier */
    entree -=1;
    
    int fd = open(fileName, O_RDONLY);
    
    if (fd == -1) {
        if (errno == EACCES){
            printf("Vous n'avez pas l'autorisation d'accéder à ce fichier\n");
            exit(-1); // Accès interdit
        }
        else if (errno == ENOENT) {
            /* Le fichier n'existe pas  */
            retour = write(1, "Fichier non existant\n", strlen("Fichier non existant\n"));
            if (retour == -1){
                if (errno == EIO ){
                    printf("Problème d'écriture dans stdio\n");
                    exit(-4) ;
                } else {
                    printf("Problème d'écriture non répertorié\n" );
                    exit(-99);
                }
            }
            close(fd);
            return;
        } else {
            printf("Erreur d'ouverture non répertoriée\n" );
            exit(-3);
        }
    }
    
    /* Positionnement du pointeur de lecture */
    positionnement = lseek(fd, sizeof(Contact)*entree ,SEEK_SET);
    
    if (positionnement == -1) {
        if (errno == EINVAL) {
            printf("Valeur excédant les bornes du fichier\n");
            close(fd);
            return;
        } else {
            printf("Erreur de positionnement\n");
            close(fd);
            return;
        }
    }
    
    retour = 0;
    retour = litUneEntree(fd);
    
    if (retour < 0) {
        printf("Hors fichier\n");
    }
    close(fd);

}


//Ajoute une entree en fin de fichier
void ajout(char* fileName)
{
    int retour;
    /* Ouverture et test d'ouverture */
    int fd = open(fileName, O_RDWR);
    if (fd == -1) {
        if (errno == EACCES){
            printf("Vous n'avez pas l'autorisation d'accéder à ce fichier\n");
            exit(-1); // Accès interdit
        }
        else if (errno == ENOENT) {
            /* Le fichier n'existe pas  : Création du fichier */
            retour = write(1, "Fichier non existant\nCréation ...\n", strlen("Fichier non existant\nCréation ...\n"));
            if (retour == -1){
                if (errno == EIO ){
                    printf("Problème d'écriture dans stdio\n");
                    exit(-4) ;
                } else {
                    printf("Problème d'écriture non répertorié\n" );
                    exit(-99);
                }
            }
            
            /* Création avec les droits corrects et ouverture en lecture écriture */
            fd = open(fileName, O_CREAT, S_IRWXU);
            close(fd);
            fd = open(fileName, O_RDWR);
        } else {
            printf("Erreur d'ouverture non répertoriée\n" );
            exit(-3);
        }
    }
    
    /* Création du Contact à rajouter */
    Contact * nouveau;
    nouveau = (Contact *) malloc(sizeof(Contact));
    if (!nouveau) {
        printf("Problème d'allocation dynamique\n");
        return;
    }
    
    /* Positionnement du pointeur de lecture/écriture à la fin du fichier */
    int positionnement = lseek(fd, 0 ,SEEK_END);
    if (positionnement == -1) {
        if (errno == EINVAL) {
            printf("Valeur excédant les bornes du fichier\n");
            close(fd);
            return;
        } else {
            printf("Erreur de positionnement\n");
            close(fd);
            return;
        }
    }
    
    /* Lecture du Contact */
    scanContact(nouveau);
    printf("Entree proposee :\nid: %d\nFirst name: %s\nName: %s\n", nouveau->id, nouveau->firstName,nouveau->name );

    
    /* Enregistrement du contact */
    retour = verrouillage(fd, 2);
    if (retour <0) {
        close(fd);
        return;
    } else {
        retour = 0;
        retour = write(fd, nouveau, sizeof(Contact));
        deverrouillage(fd);
        if (retour == -1){
            if (errno == EIO ){
                printf("Problème d'écriture dans le fichier de données\n");
                exit(-4) ;
            } else {
                printf("Problème d'écriture non répertorié\n" );
                exit(-99);
            }
        }
        close(fd);
    }
}


//Modifie une entree donnee
void modificationEntree(char* fileName)
{
    int entree = 0;
    int retour;
    int positionnement;
    int ok;
    
    
    do {
        write(1,"Choisir le numero de l'entrée à modifier : " , strlen("Choisir le numero de l'entrée à modifier : "));
        fflush(stdout);
        ok = scanf("%d", &entree);
        while (getchar() != '\n');
    } while(!ok && entree <=0);
    
    entree -=1;
    
    int fd = open(fileName, O_WRONLY);
    
    positionnement = lseek(fd, sizeof(Contact)*entree ,SEEK_SET);
    if (positionnement == -1) {
        if (errno == EINVAL) {
            printf("Valeur excédant les bornes du fichier\n");
            close(fd);
            return;
        } else {
            printf("Erreur de positionnement\n");
            close(fd);
            return;
        }
    }
    
    Contact * modif;
    modif = (Contact *) malloc(sizeof(Contact));
    if (!modif) {
        printf("Problème d'allocation dynamique\n");
        return;
    }
    scanContact(modif);
    printf("Entree modifée :\nid: %d\nFirst name: %s\nName: %s\n", modif->id, modif->firstName,modif->name );
    
    positionnement = lseek(fd, sizeof(Contact)*entree,SEEK_SET);
    if (positionnement == -1) {
        if (errno == EINVAL) {
            printf("Valeur excédant les bornes du fichier\n");
            close(fd);
            return;
        } else {
            printf("Erreur de positionnement\n");
            close(fd);
            return;
        }
    }
    
    retour = verrouillage(fd, 2);
    if (retour <0) {
        close(fd);
        return;
    } else {
        retour = 0;
        retour = write(fd, modif, sizeof(Contact));
        if (retour < sizeof(Contact)) {
            perror("Problème d'écriture");
        }
        deverrouillage(fd);
        close(fd);
        }
}

//Variant 0: pas de lock
//Variant 1: lock bloquant
//Variant 2: lock non-bloquant
int verrouillage(int fd/*, int offset*/, int variant)
{
    int succesVerrou = 0;
    if (variant == 1) {
        succesVerrou = lockf(fd, F_LOCK, sizeof(Contact));
        if (succesVerrou == -1 ) {
            if ((errno == EACCES) || (errno == EAGAIN) ) {
                printf("Fichier déjà utilisé par un autre processus\n");
                return -1;
            } else if (errno == EDEADLK) {
                printf("Interblocage évité\n");
                return -2;
            } else {
                printf("Erreur de verrou non répertoriée\n");
                return - 3;
            }
        }
        
        /* Succès */
        return 0;
    } else if (variant == 2 ){
        succesVerrou = lockf(fd, F_TLOCK , sizeof(Contact));
        if (succesVerrou == -1 ) {
            if ((errno == EACCES) || (errno == EAGAIN) ) {
                printf("Verrou déjà posé sur le fichier, retour au menu.\n");
                return -5;
            } else if (errno == EDEADLK) {
                printf("Interblocage évité\n");
                return -2;
            } else {
                printf("Erreur de verrou non répertoriée\n");
                return - 3;
            }
        }
        
        /* Succès */
        return 0;
    }

    return(0);
}

void deverrouillage(int fd/*, int offset*/)
{
    int succesDeverrou = 0;
    succesDeverrou = lockf(fd, F_ULOCK, -(sizeof(Contact)));
    
    if (succesDeverrou == -1 ){
        printf("Erreur de déverrouillage, aie aie aie !\n");
        exit(-666);
    }

}
