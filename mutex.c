#include <pthread.h>    // para usar threads
#include <stdio.h>
#include <stdlib.h>    

static void *writeFile(void *frase);
FILE *file = NULL;
pthread_mutex_t mutexFile = PTHREAD_MUTEX_INITIALIZER;
static int num = 0;
int main() {

    file = fopen("/home/luisterceroiii/Escritorio/TP-Semaforos/MinitTP-Semaforos-SOR/salida.txt","a"); 
    

    pthread_t t1;
    pthread_t t2;
    char *nombre1 = "Luis";
    char *nombre2 = "Espinoza";
    pthread_create(&t1,NULL,writeFile, (void*) nombre1);

    pthread_create(&t2,NULL,writeFile, (void*) nombre2);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("%d\n",num);

    pthread_exit(NULL); 
    fclose(file); 
    return 0;

}

 static void *writeFile(void *frase) {
    char *string = (char*) frase;
    pthread_mutex_lock(&mutexFile);
    fputs(string,file);
    fputs(" ",file);
    num = num + 50;
    pthread_mutex_unlock(&mutexFile);
}





