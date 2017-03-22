#include <stdio.h>
#include <stdlib.h>

typedef struct Personne Personne;
/* Deuxième occurence de personne = alias pour struct Personne */

struct Personne {
    char nom[20];
    int age;
    Personne * suivant;
};

/* Passage de paramètre par adresse */
void creationListe (Personne **_tete);

/* Passage de paramètre par valeur */
void affichageListe (Personne *_tete);

/* Inserer un element en tete de liste*/
void insertionTete(Personne **_tete);

/* Inserer quelque chose en fin de liste */
void insertionFin (Personne *_tete);

int menu();

void suppressionTete (Personne **_tete);

void suppressionFin (Personne *_tete);

int main() {
    Personne *tete = NULL;
    /* Tête au démarrage ne contient aucune adresse */
    int choix ;
    
    do {
        choix = menu();
        switch (choix) {
            case 1 :
                creationListe(&tete);
                break;
            case 2 :
                affichageListe(tete);
                break;
            case 3 :
                insertionTete(&tete);
                break;
            case 4 :
                insertionFin(tete);
            case 5 :
                suppressionTete(&tete);
                break;
            case 6 :
                suppressionFin(tete);
                break;
        }
    } while(choix != 0);
    
    return 0;
}

void creationListe (Personne **_tete){
    Personne *courant, *precedent;
    int reponse;
    /* cast du malloc en adresse de Personne * */
    
    do{
        courant = (Personne *) malloc(sizeof(Personne));
        printf("\nNom : ");
        scanf("%s", courant->nom /* (*courant).nom */);
        
        /* Pas besoin du & parce que chaîne de caractère est une adresse de toutes façons */
        printf("\nAge : ");
        scanf("%d", &courant->age);
        
        courant->suivant = NULL;
        
        if (*_tete == NULL){
            *_tete = courant;
        } else {
            precedent->suivant = courant;
        }
        precedent = courant;
        
        printf("\nVoulez-vous recommencer (1 = Oui)");
        scanf("%d", &reponse);
        
    }while(reponse == 1);
}

void affichageListe (Personne *_tete){
    
    Personne *courant = _tete;
    
    while(courant != NULL){
        printf("%s\t%d\n", courant->nom, courant->age);
        courant = courant->suivant;
    }
    
}

int menu(){
    int choix= -1;
    
     printf("\nChoix de l'operation :");
     printf("\n1. Generer liste :");
     printf("\n2. Afficher liste :");
     printf("\n3. Insertion en tete :");
     printf("\n4. Insertion en fin :");
     printf("\n5. Suppression du maillon de tete :");
     printf("\n6. Suppression du maillon de fin :");
     
     printf("\nVotre choix : ");
     
     scanf("%d", &choix);
    
    return choix;
}

void insertionTete(Personne **_tete){
    Personne *nouveau;
    
    /* cast du malloc en adresse de Personne * */
    nouveau = (Personne *) malloc(sizeof(Personne));
    printf("\nNom : ");
    
    /* Pas besoin du & parce que chaîne de caractère est une adresse de toutes façons */
    scanf("%s", nouveau->nom /* (*courant).nom */);
    
    printf("\nAge : ");
    /* Ici on a besoin du & parce qu'on met la valeur scannée à l'adresse de la variable*/
    scanf("%d", &nouveau->age);
    
    nouveau->suivant = *_tete;
    *_tete = nouveau;
    
}

void insertionFin(Personne *_tete){
    Personne *courant, *precedent, *nouveau;
    
    courant = _tete;
    // printf("bla1\n" );
    
    while(courant != NULL){
        precedent = courant;
        courant = courant->suivant;
    }
    // printf("bla2\n" );
    nouveau = (Personne *) malloc(sizeof(Personne));
    // printf("bla3\n" );
    printf("\nNom : ");
    scanf("%s", nouveau->nom /* (*courant).nom */);
    
    /* Pas besoin du & parce que chaîne de caractère est une adresse de toutes façons */
    printf("\nAge : ");
    scanf("%d", &nouveau->age);
    // printf("bla4\n" );
    
    precedent->suivant = nouveau;
    // printf("bla5\n" );
}

void suppressionTete (Personne **_tete){
    Personne *poubelle, *suivant;
    
    poubelle = *_tete;
    
    suivant = poubelle->suivant;
    
    *_tete = suivant;
    
    free(poubelle);
}

void suppressionFin (Personne *_tete){
    Personne *courant, *precedent;
    
    courant = _tete;
    
    /* Si la tête est d'ores et déjà NULL */
    if(courant->suivant == NULL){
        printf("Impossible de supprimer la tete\n");
        return;
    }
    
    while(courant->suivant != NULL){
        precedent = courant;
        courant = courant->suivant;
    }
    
    precedent->suivant = NULL;
    
    free(courant);
    
}
