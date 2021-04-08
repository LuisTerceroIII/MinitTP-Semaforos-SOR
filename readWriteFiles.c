#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIMITE 50

int main() {

     struct paso {
        char accion [LIMITE];
        char ingredientes[4][LIMITE];
    };

    struct parametro {
        struct paso pasos_param[8];
    };

    struct  parametro *pasos = malloc(sizeof(struct parametro));

    
    for(int i = 0; i < 8; i++) {
        strcpy(pasos->pasos_param[i].accion," ");
    }
        for(int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                 strcpy(pasos->pasos_param[i].ingredientes[j]," ");
            }
    }
    
    
    //Puntero para asignar un archivo
    FILE *receta = NULL;
    char buffer [100];
    int k = 0;
    receta = fopen("/home/luisterceroiii/Escritorio/TP-Semaforos/MinitTP-Semaforos-SOR/receta.txt","r+");
    
    if(receta == NULL ) {
        printf("No se puedo abrir receta \n");
        return -1;
    } else {
        
        while (fgets(buffer,100,receta) != NULL) {
            printf("BUFFER : %s\n",buffer);
            char *split = strtok(buffer,"|");
            struct paso paso;

          

            int i = 0;
            int j = 0;

            while(split != NULL) {
                if(i == 0) {
                    strcpy(paso.accion,split);
                    printf("En accion %s\n",split);
                    i++;
                } else {
                    strcpy(paso.ingredientes[j],split);
                    printf("En ingrediente %s\n",split);
                    j++;
                }
                split = strtok(NULL,"|");
            }

            strcpy(pasos->pasos_param[k].accion,paso.accion);
            for (int i = 0; i < 5; i++) {
                if(strlen(paso.ingredientes[i]) > 0) {
                    strcpy(pasos->pasos_param[k].ingredientes[i],paso.ingredientes[i]);
                }
            }
            k++;
        }
        fclose(receta);
    }

    int sizePasos = sizeof(pasos->pasos_param) / sizeof(pasos->pasos_param[0]);

    printf("Ingrediente : %s\n",pasos->pasos_param[1].ingredientes[3]);

    printf("---------------------------\n");
    FILE *salida = NULL;
    salida = fopen("/home/luisterceroiii/Escritorio/TP-Semaforos/MinitTP-Semaforos-SOR/salida.txt","w"); 
    fputs("Pasos: \n",salida);
    for (int i = 0; i < sizePasos; i++) {
       if(strcmp(pasos->pasos_param[i].accion," ") != 0) {
            fputs("Accion : ",salida);
            fputs(pasos->pasos_param[i].accion,salida);
            fputs("\n",salida);
            printf("Accion : %s\n",pasos->pasos_param[i].accion);
            int sizeArrayIngredientes = sizeof(pasos->pasos_param[i].ingredientes) / sizeof(pasos->pasos_param[i].ingredientes[0]);
            fputs("Ingredeientes : \n",salida);
            for (int j = 0; j < sizeArrayIngredientes; j++) {
                if(strcmp(pasos->pasos_param[i].ingredientes[j]," ") != 0) {
                    
                    printf("Ingrediente : %s\n",pasos->pasos_param[i].ingredientes[j]);
                    fputs(pasos->pasos_param[i].ingredientes[j],salida);
                     fputs("\n",salida);
                }
            }
       }
    }
    
    fputs("\n\nLuis Espinoza",salida);
    fclose(salida);
    
/* 



    printf("---------------------------\n"); 


    printf("Accion 1 : %s\n",pasos.pasos_param[0].accion);
    printf("Ingredientes 1 : %s\n",pasos.pasos_param[0].ingredientes[0]);
    printf("Ingredientes 1 : %s\n",pasos.pasos_param[0].ingredientes[1]);
    printf("Ingredientes 1 : %s\n",pasos.pasos_param[0].ingredientes[2]);
    printf("Ingredientes 1 : %s\n",pasos.pasos_param[0].ingredientes[3]);

    printf("---------------------------\n"); 



    printf("Accion 2 : %s\n",pasos.pasos_param[1].accion);
    printf("Ingredientes 1 : %s\n",pasos.pasos_param[1].ingredientes[0]);
    printf("Ingredientes 1 : %s\n",pasos.pasos_param[1].ingredientes[1]); */





    return 0;
}