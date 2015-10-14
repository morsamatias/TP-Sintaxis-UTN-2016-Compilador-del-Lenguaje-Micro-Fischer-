#ifndef FUNCIONES_H_
#define FUNCIONES_H_


/////////////////////////INCLUYO BIBLIOTECAS////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMLEX 33
///////Defino Variables

typedef enum {
	INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
	PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO
} TOKEN;

typedef struct{
	TOKEN clase;
	char nombre[TAMLEX];
	int valor;
} REG_EXPRESION;

FILE * archivo;
FILE * archivoSalida;


char buffer[TAMLEX];

TOKEN tokenActual;
int flagToken;

typedef struct{
	char identificador[TAMLEX];
	TOKEN t; /* t=0, 1, 2, 3 Palabra Reservada, t=ID=4 Identificador */
} RegTS;


//////////////Defino Funciones///////////////////

void Match(TOKEN token);
void Objetivo(void);
void Programa(void);
void ListaSentencias(void);
void Sentencia(void);
void Expresion(REG_EXPRESION * presul);
void Primaria(REG_EXPRESION * presul);
void OperadorAditivo(char * presul);
void ErrorSintactico();
void ErrorLexico();
TOKEN ProximoToken(void);
void Identificador(REG_EXPRESION * presul);
void ListaExpresiones(void);
void ListaIdentificadores(void);

TOKEN Scanner(void);
int VerificarLongitud(void);
void AlmacenarEnTS(TOKEN token);
int columna(int c);
void AgregarCaracter(int caracter, int posicion);
void LimpiarBuffer(void);
int validacion(char* arch,char c);

REG_EXPRESION ProcesarCte(void);
REG_EXPRESION ProcesarId(void);
char * ProcesarOp(void);
void Leer(REG_EXPRESION in);
void Escribir(REG_EXPRESION out);
REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2);
void Generar(char * co, char * a, char * b, char * c);
char * Extraer(REG_EXPRESION * preg);
int Buscar(char * id, RegTS * TS, TOKEN * t);
void Colocar(char * id, RegTS * TS);
void Chequear(char * s);
void Asignar(REG_EXPRESION izq, REG_EXPRESION der);
void Comenzar(void);
void Terminar(void);


#endif /* Funciones_H_ */
