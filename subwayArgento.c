#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

static int posicionDeLLegada = 1;

//RECURSOS COMPARTIDOS : 
pthread_mutex_t mutexSalero = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexSarten = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexHorno = PTHREAD_MUTEX_INITIALIZER;

FILE *salida = NULL;


//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
	sem_t sem_mezcla_lista;
	sem_t sem_embetunar;
	sem_t sem_apanar;
	sem_t sem_cocinar;
	sem_t sem_hornear;
	sem_t sem_verduras_extras;
	sem_t sem_milanesa_cocinada;
	sem_t sem_pan_horneado;
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[9];
};



//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\n\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strcmp(mydata->pasos_param[i].ingredientes[h]," ") != 0) {
					if(strlen(mydata->pasos_param[i].ingredientes[h]) > 2){
						printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);	
					}							
				}
			}
		}
	}
}
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


//funcion que llama a imprimrAccion y escribirEnArchivo
void* escribir(void *data, char *accionIn) { 
		imprimirAccion(data,accionIn);
		escribirEnArchivo(data,accionIn);
}

void anunciarLlegada(void *data) {
	struct parametro *mydata = data;
	printf("\nEquipo %d llega en posicion : %d\n",mydata->equipo_param,posicionDeLLegada);
	fprintf(salida,"\nEquipo %d llega en posicion : %d\n",mydata->equipo_param,posicionDeLLegada);
	posicionDeLLegada++;
}


void* cortar(void *data) {
	char *accion = "cortar";
	struct parametro *mydata = data;
	sleep(5);
	escribir(data,accion);    
	sem_post(&mydata->semaforos_param.sem_mezclar);       
	pthread_exit(NULL);
}

void* cortarExtra(void *data) {
	char *accion = "cortar extras";
	struct parametro *mydata = data;
	sleep(5);
	escribir(data,accion);
	sem_post(&mydata->semaforos_param.sem_verduras_extras);
	pthread_exit(NULL);
}


void* mezclar(void *data) {
	char *accion = "mezclar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);
	sleep(5);
	escribir(data,accion);
	sem_post(&mydata->semaforos_param.sem_mezcla_lista);
	pthread_exit(NULL);
}

void* salar(void *data) {
	char *accion = "salar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezcla_lista);
	pthread_mutex_lock(&mutexSalero);
	sleep(5);
	pthread_mutex_unlock(&mutexSalero);
	escribir(data,accion);
	sem_post(&mydata->semaforos_param.sem_embetunar);
    pthread_exit(NULL);
}

void* embetunar(void *data) {
	char *accion = "embetunar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_embetunar);
	sleep(5);
	escribir(data,accion);
	sem_post(&mydata->semaforos_param.sem_apanar);
	pthread_exit(NULL);
}

void* apanar(void *data) {
	char *accion = "apanar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_apanar);
	sleep(5);
	escribir(data,accion);
	sem_post(&mydata->semaforos_param.sem_cocinar);
	sem_post(&mydata->semaforos_param.sem_hornear);
	pthread_exit(NULL);
}

void* cocinar(void *data) {
	char *accion = "cocinar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_cocinar);	
	pthread_mutex_lock(&mutexSarten);
	sleep(10);
	pthread_mutex_unlock(&mutexSarten);
	escribir(data,accion);
   	sem_post(&mydata->semaforos_param.sem_milanesa_cocinada);
    pthread_exit(NULL);
}

void* hornear(void *data) {
	char *accion = "hornear";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_hornear);
	pthread_mutex_lock(&mutexHorno);
	sleep(20);
	pthread_mutex_unlock(&mutexHorno);
	escribir(data,accion);
	sem_post(&mydata->semaforos_param.sem_pan_horneado);
	pthread_exit(NULL);
}

void* armar(void *data) {
	char *accion = "armar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_milanesa_cocinada);
	sem_wait(&mydata->semaforos_param.sem_pan_horneado);
	sem_wait(&mydata->semaforos_param.sem_verduras_extras);
	sleep(5);
	escribir(data,accion);	
	anunciarLlegada(data);
	pthread_exit(NULL);
}


 void verficarCreacionThread(int rc) {
	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }
}

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

void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
	sem_t sem_mezcla_lista;
	sem_t sem_embetunar;
	sem_t sem_apanar;
	sem_t sem_cocinar;
	sem_t sem_hornear;
	sem_t sem_verduras_extras;
	sem_t sem_milanesa_cocinada;
	sem_t sem_pan_horneado;

	// Semaforos utiles para imprimir
	sem_t sem_cortar_imprimir;
	sem_t sem_cortar_extra_imprimir;
	sem_t sem_mezclar_imprimir;
	sem_t sem_mezcla_lista_imprimir;
	sem_t sem_embetunar_imprimir;
	sem_t sem_apanar_imprimir;
	sem_t sem_cocinar_imprimir;
	sem_t sem_hornear_imprimir;
	sem_t sem_armar_imprimir;
	
	//variables hilos
	pthread_t pasoUnoCortar; 
	pthread_t pasoDosMezclar; 
	pthread_t pasoTresSalar; 
	pthread_t pasoCuatroEmbetunar; 
	pthread_t pasoCincoApanar; 
	pthread_t pasoCocinar; 
	pthread_t pasoHornear; 
	pthread_t pasoArmar; 
	pthread_t pasoCortarExtras; 
	
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);
	fprintf(salida,"Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_mezcla_lista = sem_mezcla_lista;
	pthread_data->semaforos_param.sem_embetunar = sem_embetunar;
	pthread_data->semaforos_param.sem_apanar = sem_apanar;
	pthread_data->semaforos_param.sem_cocinar = sem_cocinar;
	pthread_data->semaforos_param.sem_hornear = sem_hornear;
	pthread_data->semaforos_param.sem_verduras_extras = sem_verduras_extras;
	pthread_data->semaforos_param.sem_milanesa_cocinada = sem_milanesa_cocinada;
	pthread_data->semaforos_param.sem_pan_horneado = sem_pan_horneado;
	

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
	cargarReceta(pthread_data);

	//inicializo los semaforos
	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_mezcla_lista),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_embetunar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_apanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_hornear),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_verduras_extras),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_milanesa_cocinada),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_pan_horneado),0,0);

	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;

    rc = pthread_create(&pasoUnoCortar,NULL,cortar,pthread_data);  
	verficarCreacionThread(rc); 

    rc = pthread_create(&pasoDosMezclar,NULL,mezclar,pthread_data); 
	verficarCreacionThread(rc); 

    rc = pthread_create(&pasoTresSalar,NULL,salar,pthread_data);  									
	verficarCreacionThread(rc);  

    rc = pthread_create(&pasoCuatroEmbetunar,NULL,embetunar,pthread_data);  	
	verficarCreacionThread(rc);  

    rc = pthread_create(&pasoCincoApanar,NULL,apanar,pthread_data);  	
	verficarCreacionThread(rc);   

    rc = pthread_create(&pasoCocinar,NULL,cocinar,pthread_data);  
	verficarCreacionThread(rc);   

    rc = pthread_create(&pasoHornear,NULL,hornear,pthread_data);  																
	verficarCreacionThread(rc);   

    rc = pthread_create(&pasoArmar,NULL,armar,pthread_data); 
	verficarCreacionThread(rc);   

	rc = pthread_create(&pasoCortarExtras,NULL,cortarExtra,pthread_data); 
	verficarCreacionThread(rc);   

	//join de todos los hilos
	pthread_join (pasoUnoCortar,NULL);
	pthread_join (pasoDosMezclar,NULL);
	pthread_join (pasoTresSalar,NULL);
	pthread_join (pasoCuatroEmbetunar,NULL);
	pthread_join (pasoCincoApanar,NULL);
	pthread_join (pasoCocinar,NULL);
	pthread_join (pasoHornear,NULL);
	pthread_join (pasoArmar,NULL);
	pthread_join (pasoCortarExtras,NULL);


	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_mezcla_lista);
	sem_destroy(&sem_embetunar);
	sem_destroy(&sem_apanar);
	sem_destroy(&sem_cocinar);
	sem_destroy(&sem_hornear);
	sem_destroy(&sem_verduras_extras);
	sem_destroy(&sem_milanesa_cocinada);
	sem_destroy(&sem_pan_horneado);
	
	//salida del hilo
	 pthread_exit(NULL);
}




int main ()
{	

	salida = fopen("salida.txt","a"); 

	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3; 
	pthread_t equipo4;
  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,          
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,          
                                equipoNombre2);

 	rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,          
                                equipoNombre3); 

    rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,          
                                equipoNombre4);



   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);

	fclose(salida);
	
    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o subway -lpthread
//Para ejecutar:   ./subway

