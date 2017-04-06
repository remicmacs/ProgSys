#include "ligne_commande.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    printf("Test include\n");
    char * str = (char * ) 255;
    str = "Braaains\0";
    ligne_vide(&str);
    
    return 0;
}
