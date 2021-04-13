# SOR-Semáforos-1S-2021

# TP - Threads y Semáforos

### Desarrollo
Comencé con una lectura detenida al enunciado y al código template ofrecido.
Luego separe al trabajo realizado por los cuatro equipos en 9 pasos.
7 de estos pasos tienen requisitos para comenzar, es decir, que esperan eventos antes de poder ejecutarse.
Realice un pseudocódigo y una prueba de escritorio para sincronizar las tareas.

#### Pseudocódigo y prueba de escritorio
![](https://i.ibb.co/PhvFCxF/pseudocodigo.jpg")
###### Semáforos y Mutex
![](https://i.ibb.co/HF5b30h/prueba-escritorio.png")

#### Código, implementación y problemas encontrados.
Lo que hice fue agregar los demás equipos (threads) al método main, preparar los semáforos y mutex planteados arriba, más el necesario para imprimir en terminal y escribir en el archivo de salida de manera secuencial, crear un método "acción" para cada parte de la receta (cortar,mezclar,salar,etc...) y hacer las operaciones de manejo de archivo.
##### Mutex de recursos compartidos
Estos mutex se comparten por todos los equipos.
```c
//RECURSOS COMPARTIDOS :
pthread_mutex_t mutexSalero = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexSarten = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexHorno = PTHREAD_MUTEX_INITIALIZER;

//Sincroniza salidas
pthread_mutex_t mutexImpresion = PTHREAD_MUTEX_INITIALIZER;

//Sirve para incrementar el contador de llegada sin errores.
pthread_mutex_t mutexPosicion = PTHREAD_MUTEX_INITIALIZER;
```
##### Semáforos de acción
Estos semáforos sincronizan las tareas dentro de cada thread-equipo.
```c
    sem_t sem_mezclar;
    sem_t sem_mezcla_lista;
    sem_t sem_embetunar;
    sem_t sem_apanar;
    sem_t sem_cocinar;
    sem_t sem_hornear;
    sem_t sem_verduras_extras;
    sem_t sem_milanesa_cocinada;
    sem_t sem_pan_horneado;
```
##### Metodos de accion
###### cortar
```c
void* cortar(void *data) {
    char *accion = "cortar";
    struct parametro *mydata = data;
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(5);
    sem_post(&mydata->semaforos_param.sem_mezclar);
    pthread_exit(NULL);
}
```
###### cortarExtra
Esta funcion al igual que cortar, no tiene requisitos para ejecutarse.
```c
void* cortarExtra(void *data) {
    char *accion = "cortar extras";
    struct parametro *mydata = data;
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(5);
    sem_post(&mydata->semaforos_param.sem_verduras_extras);
    pthread_exit(NULL);
}
```
###### mezclar
```c
void* mezclar(void *data) {
    char *accion = "mezclar";
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_mezclar);
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(5);
    sem_post(&mydata->semaforos_param.sem_mezcla_lista);
    pthread_exit(NULL);
}

```
###### salar
```c
void* salar(void *data) {
    char *accion = "salar";
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_mezcla_lista);
    pthread_mutex_lock(&mutexSalero);
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(5);
    pthread_mutex_unlock(&mutexSalero);
    sem_post(&mydata->semaforos_param.sem_embetunar);
    pthread_exit(NULL);
}
```
###### embetunar
```c
void* embetunar(void *data) {
    char *accion = "embetunar";
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_embetunar);
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(5);
    sem_post(&mydata->semaforos_param.sem_apanar);
    pthread_exit(NULL);
}

```
###### apanar
```c
void* apanar(void *data) {
    char *accion = "apanar";
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_apanar);
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(5);
    sem_post(&mydata->semaforos_param.sem_cocinar);
    sem_post(&mydata->semaforos_param.sem_hornear);
    pthread_exit(NULL);
}
```
###### cocinar
```c
void* cocinar(void *data) {
    char *accion = "cocinar";
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_cocinar);
    pthread_mutex_lock(&mutexSarten);
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(10);
    pthread_mutex_unlock(&mutexSarten);
    sem_post(&mydata->semaforos_param.sem_milanesa_cocinada);
    pthread_exit(NULL);
}
```
###### hornear
```c
void* hornear(void *data) {
    char *accion = "hornear";
    struct parametro *mydata = data;
    pthread_mutex_lock(&mutexHorno);
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(20);
    pthread_mutex_unlock(&mutexHorno);
    sem_post(&mydata->semaforos_param.sem_pan_horneado);
    pthread_exit(NULL);
}
```
###### armar
```c
void* armar(void *data) {
    char *accion = "armar";
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_milanesa_cocinada);
    sem_wait(&mydata->semaforos_param.sem_pan_horneado);
    pthread_mutex_lock(&mutexImpresion);
    imprimirAccion(mydata,accion);
    escribirEnArchivo(mydata,accion);
    pthread_mutex_unlock(&mutexImpresion);
    sleep(5);
    pthread_mutex_lock(&mutexPosicion);
    printf("\nEquipo %d llega en posicion : %d\n",mydata->equipo_param,posicionDeLLegada);
    fprintf(salida,"\nEquipo %d llega en posicion : %d\n",mydata->equipo_param,posicionDeLLegada);
    posicionDeLLegada++;
    pthread_mutex_unlock(&mutexPosicion);
    pthread_exit(NULL);
}
```
Respecto a esta parte el único problema que encontré fue que el método usleep que se usaba en el template no funcionaba, no había espera, sino que ejecutaba todo en 1 segundo, probe cambiando a sleep() y funciono correctamente.
##### Manejo de archivos
###### Cargar receta de archivo
```c
void* cargarReceta(void *data) {
    struct parametro *mydata = data;
    FILE *receta = NULL;
    char buffer [100];
    int k = 0;
    receta = fopen("receta.txt","r");
    if(receta == NULL ) {
        printf("No se puedo abrir receta \n");
        exit(-1);
    } else {
        while (fgets(buffer,100,receta) != NULL) {
            char *split = strtok(buffer,"|");
            struct paso paso;
            int i = 0;
            int j = 0;

            while(split != NULL) {
                if(i == 0) {
                    strcpy(paso.accion,split);
                    i++;
                } else {
                    strcpy(paso.ingredientes[j],split);
                    j++;
                }
                split = strtok(NULL,"|");
            }
            strcpy(mydata->pasos_param[k].accion,paso.accion);
            for (int i = 0; i < 5; i++) {
                if(strlen(paso.ingredientes[i]) > 0) {
                    strcpy(mydata->pasos_param[k].ingredientes[i],paso.ingredientes[i]);
                }
            }
            k++;
        }
        fclose(receta);
    }
}
```

###### Escribir en archivo de salida
Para esto inicialice un archivo de salida en main, luego tome la función dada "imprimirAccion", la copie y renombre a "escribirEnArchivo" y cambie los llamados de printf() por fprintf(). La función fprintf() recibe un archivo y un "mensaje" para escribir en dicho archivo.

```c
salida = fopen("salida.txt","a");
```
Es importante inicializar el archivo con la opcion "a", ya que de esa manera siempre escribe al final del archivo.
```c
void* escribirEnArchivo(void *data, char *accionIn) {
    struct parametro *mydata = data;
    
    //calculo la longitud del array de pasos
    int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
    //indice para recorrer array de pasos
    int i;
    for(i = 0; i < sizeArray; i ++){
        //pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
        if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
        fprintf(salida,"\n\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
        //calculo la longitud del array de ingredientes
        int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
        //indice para recorrer array de ingredientes
        int h;
        fprintf(salida,"\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
            for(h = 0; h < sizeArrayIngredientes; h++) {
                //consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
                if(strcmp(mydata->pasos_param[i].ingredientes[h]," ") != 0) {
                    if(strlen(mydata->pasos_param[i].ingredientes[h]) > 2){
                        fprintf(salida,"\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);    
                    }
                }
            }
        }
    }
}
```
### Conclusión
El desarrollo del trabajo lo hice de manera incremental, cada pieza del código fue tomada y probada de manera separada, luego cuando ensamble cada parte, todo funcionó muy bien, no tuve problemas para solucionarlo.


##### Luis Hector Espinoza Navarrete

