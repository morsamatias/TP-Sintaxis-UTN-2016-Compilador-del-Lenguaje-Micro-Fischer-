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
int validacion(char* arch,char c);


#endif /* Funciones_H_ */
