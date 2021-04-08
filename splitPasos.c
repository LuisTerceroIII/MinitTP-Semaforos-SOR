#include <stdio.h>
#include <string.h>

#define LIMITE 50

int main() {

     struct paso {
        char accion [LIMITE];
        char ingredientes[4][LIMITE];
   
    };
    char paso1[] = "cortar|ajo|perejil|champignong|morron";

    char *splitPaso1 = strtok(paso1,"|");

    struct paso pUno;

    int i = 0;
    int j = 0;

    while(splitPaso1 != NULL) {
        if(i == 0) {
            strcpy(pUno.accion,splitPaso1);
            printf("En accion %s\n",splitPaso1);
            i++;
        } else {
            strcpy(pUno.ingredientes[j],splitPaso1);
            printf("En ingrediente %s\n",splitPaso1);
            j++;
        }
        splitPaso1 = strtok(NULL,"|");
        printf("Saliendo %s\n",splitPaso1);
    }

    int len = sizeof(pUno.ingredientes) / sizeof(pUno.ingredientes[0]);

    printf("Accion paso 1 : %s \n",pUno.accion);

    for ( i = 0; i < len; i++) {
        if(strlen(pUno.ingredientes[i]) != 0) {
                printf("Ingrediente %s accion :%s \n",pUno.ingredientes[i],pUno.accion);
        }
    }  


    return 0;
}