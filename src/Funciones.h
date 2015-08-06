#ifndef FUNCIONES_H_
#define FUNCIONES_H_


/////////////////////////INCLUYO BIBLIOTECAS////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <commons/config.h>
#include <commons/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <time.h>
#include <features.h>
#include <semaphore.h>


#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02
#define tamanioBloque  20971520

#define NUM_PIPES          2

#define PARENT_WRITE_PIPE  0
#define PARENT_READ_PIPE   1
#define READ_FD  0
#define WRITE_FD 1

#define PARENT_READ_FD  ( pipes[PARENT_READ_PIPE][READ_FD]   )
#define PARENT_WRITE_FD ( pipes[PARENT_WRITE_PIPE][WRITE_FD] )

#define CHILD_READ_FD   ( pipes[PARENT_WRITE_PIPE][READ_FD]  )
#define CHILD_WRITE_FD  ( pipes[PARENT_READ_PIPE][WRITE_FD]  )


///////Defino Variables


int	 	  tam;
char*	  mapeo;
char* 	  copia_mapeo;
char* 	  Temp;
char*     contbloque;
char*	  hilo_bitmap;
pthread_t *hilo;

//////////////Defino Funciones///////////////////

int redireccionar (char* fuente,char* salida,char* script);
char* copiar_archivo_a_buffer(char* archivo_origen);

#endif /* Funciones_H_ */
