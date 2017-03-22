   /**************************************************************************/
  /*                        hello.c                                         */
 /* par Remi Bourgeon   DUT INFO ASPE      TP1     le 10/O3/2017           */
/**************************************************************************/

/* Pour strlen() */
#include <string.h>
/* Pour perror() */
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[], char *argenv[]) {
    
    int integer;
    
    integer = write( 1 ,"hello, world\n", strlen("hello, world\n"));
        
    /* Si write retourne -1, il y a eu une erreur */
    if (integer==-1) perror("Probleme d'ecriture");
    
    return 0;
}
