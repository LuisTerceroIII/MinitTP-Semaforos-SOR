#include <stdio.h>
#include <string.h>

#define LIMITE 50

int main() {

    struct paso {
        char accion [LIMITE];
        char ingredientes[4][LIMITE];
   
    };

    struct paso mezclar;

    strcpy(mezclar.accion,"cortar");

    printf("%s\n",mezclar.accion);


    return 0;
}