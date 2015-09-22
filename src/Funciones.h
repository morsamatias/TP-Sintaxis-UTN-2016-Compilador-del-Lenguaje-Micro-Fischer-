#ifndef FUNCIONES_H_
#define FUNCIONES_H_


/////////////////////////INCLUYO BIBLIOTECAS////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include <sys/mman.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

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

typedef enum {
	INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
	PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO
} TOKEN;

FILE * archivo;

#define TAMLEX 33

char buffer[TAMLEX];

TOKEN tokenActual;
int flagToken;




int	 	  tam;
char*	  mapeo;
char* 	  copia_mapeo;
char* 	  Temp;
char*     contbloque;
char*	  hilo_bitmap;
pthread_t *hilo;


//////////////Defino Funciones///////////////////

void Match(TOKEN token);
void Objetivo(void);
void Programa(void);
void ListaSentencias(void);
void Sentencia(void);
void Expresion(void);
void Primaria(void);
void OperadorAditivo(void);
void ErrorSintactico();
void ErrorLexico();
TOKEN ProximoToken(void);
void ListaIdentificadores(void);
void ListaExpresiones(void);
void Identificador(void);

TOKEN Scanner(void);
int VerificarLongitud(void);
void AlmacenarEnTS(TOKEN token);
int columna(int c);
void AgregarCaracter(int caracter, int posicion);
TOKEN EsReservada(void);
void LimpiarBuffer(void);


int redireccionar (char* fuente,char* salida,char* script);
char* copiar_archivo_a_buffer(char* archivo_origen);


#endif /* Funciones_H_ */
