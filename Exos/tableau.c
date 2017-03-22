#include <stdio.h> /*Entrées / sorties */
#include <time.h> /* Gestion de l'horloge */
#include <stdlib.h>

// Underscore pour signaler des paramètres formels
void remplissage (int _t[], int _taille);

void remplissage_auto(int _t[], int _taille);

void affichage (int _t[], int _taille);

int menu();

void min_et_max(int _t[], int _taille);

int min(int _t[], int _taille);

int max(int _t[], int _taille);

void minimum(int _t[], int _taille, int *_mini);

int main(){
	printf("Hello World!\nASPE TD avec Fares Athmane\n\n");
	int i,j;
	int taille = 5;
	int t[taille];
	int choix;
	int minim;
	srand(time(NULL));
	
	do{
		choix = menu();
		switch (choix){
			case(1): 
				remplissage(t, taille);
				break;
			case(2): 
				remplissage_auto(t, taille);
				break;
			case(3): 
				affichage(t, taille);
				break;
			case(4): 
				min_et_max(t, taille);
				break;
			case(5): 
				minimum(t, taille, &minim);
				printf("Le minimum du tableau est : %d", minim);
				break;
		}
	}while(choix != 0);


	return 0;
}

void remplissage(int _t[], int _taille){
	int i;
	for (i=0 ; i <_taille ; i++){
		printf("T[%d]= ", i);
		scanf("%d", &_t[i]);
	}
}

void remplissage_auto(int _t[], int _taille){
	int i;
	for (i=0 ; i <_taille ; i++){
		_t[i]=rand()%10;
	}
}

void affichage (int _t[], int _taille){
	int i;
	for (i=0 ; i <_taille ; i++){
		printf("%d\t", _t[i]);
	}
	printf("\n");
}

void min_et_max(int _t[], int _taille){
	int maxi;
	maxi = max(_t, _taille);
	int mini;
	mini = min(_t, _taille);
	printf("Le maximum du tableau est : %d\n", maxi);
	printf("Le minimum du tableau est : %d\n", mini);
}

int min(int _t[], int _taille){
	int min,i;
	min = _t[0];
	for(i=0 ; i < _taille ; i++){
		if (min > _t[i]){
			min = _t[i];
		}
	}
	return min;
}

int max(int _t[], int _taille){
	int max,i;
	max = _t[0];
	for(i=0 ; i < _taille ; i++){
		if (max < _t[i]){
			max = _t[i];
		}
	}
	return max;
}

void minimum(int _t[], int _taille, int *_mini){
	*_mini = _t[0];
	
	int i;
	
	for (i = 0; i < _taille ; i++){
		if (_t[i] < *_mini){
			*_mini = _t[i];
		}
	}
}

int menu(){
	int _choix;
	printf("\n1: Saisie Manuelle");
	printf("\n2: Saisie Automatique");
	printf("\n3: Affichage");
	printf("\n4: Min et Max");
	printf("\n5: Minimum par adresse");
	printf("\n0: Sortir");
	printf("\n\n\t\tVotre choix : ");
	scanf("%d", &_choix);
	return _choix;
}

