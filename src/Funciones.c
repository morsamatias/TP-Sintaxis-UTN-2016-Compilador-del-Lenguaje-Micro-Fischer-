/*
 * Funciones.c
 *
 *  Created on: 6/8/2015
 *      Author: utnso
 */
#include "Funciones.h"

char TS[1000][33] = {"inicio", "fin", "leer", "escribir"};
int TS_token[1000] = {INICIO, FIN, LEER, ESCRIBIR};
int posUltimoSimbolo = 4;

/* ------------------------------------------PARSER---------------------------------------------------------------*/

void Match(TOKEN token){
	if (ProximoToken()!=token){
		printf("El token no coincide");
		ErrorSintactico();
	}
	flagToken=0;
}

void Objetivo(void){
	Programa();
	Match(FDT);
}

void Programa(void){
	Match(INICIO);
	ListaSentencias();
	Match(FIN);
}

void ListaSentencias(void){
	Sentencia();
	while(1){
		switch(ProximoToken()){
			case ID: case LEER: case ESCRIBIR:
				Sentencia();
				break;
			default:
				return;
		}
	}
}

void Sentencia(void){
	TOKEN tok = ProximoToken();
	switch (tok){
		case ID:
			Identificador();
			Match(ASIGNACION);
			Expresion();
			Match(PUNTOYCOMA);
			break;
		case LEER:
			Match(LEER);
			Match(PARENIZQUIERDO);
			ListaIdentificadores();
			Match(PARENDERECHO);
			Match(PUNTOYCOMA);
			break;
		case ESCRIBIR:
			Match(ESCRIBIR);
			Match(PARENIZQUIERDO);
			ListaExpresiones();
			Match(PARENDERECHO);
			Match(PUNTOYCOMA);
			break;
		default:
			ErrorSintactico();
			break;
	}
}

void Expresion(void){
	TOKEN t;
	Primaria();
	for(t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken()){
		OperadorAditivo();
		Primaria();
	}
}

void Primaria(void){
	//todo: VERIFICAR SI ESTA BIEN PRIMARIA
	switch(ProximoToken()){
		case ID:
			Identificador();
			break;
		case CONSTANTE:
			Match(CONSTANTE);
			break;
		case PARENIZQUIERDO:
			Match(PARENIZQUIERDO);
			Expresion();
			Match(PARENDERECHO);
			break;
	}
}

void OperadorAditivo(void){
	TOKEN t = ProximoToken();
	if (t == SUMA || t == RESTA)
		Match(t);
	else
		ErrorSintactico();
}

void ErrorSintactico(){
	printf("ERROR SINTACTICO");
	abort();
}

void ErrorLexico(){
	printf("ERROR LEXICO");
	abort();
}

void ListaIdentificadores(void){
	TOKEN t;
	Identificador();
	for ( t = ProximoToken(); t == COMA; t = ProximoToken()){
		Match(COMA);
		Identificador();
	}
}

void ListaExpresiones(void){
	TOKEN t;
	Expresion();
	for ( t = ProximoToken(); t == COMA; t = ProximoToken()){
		Match(COMA);
		Expresion();
	}
}

TOKEN ProximoToken(void){
	if (flagToken!=1){
		tokenActual = Scanner();
		flagToken = 1;
		if(tokenActual == ERRORLEXICO){
			ErrorLexico();
		}
		if (tokenActual == ID){
			tokenActual = EsReservada();
		}

	}
	return tokenActual;
}

void Identificador(void){
	Match(ID);
}

/* -----------------------------------------------SCANNER------------------------------------------------------*/

int estadoFinal(int e){
	if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 )
		return 0;
	return 1;
}

TOKEN Scanner(void){
	char c;
	int col, i = 0, estado_actual = 0;

	static int tabla [15][13]={
		{1,3,5,6,7,8,9,10,11,14,13,0,14},
		{1,1,2,2,2,2,2,2,2,2,2,2,2},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{4,3,4,4,4,4,4,4,4,4,4,4,4},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,12,14,14,14},
		{14,14,14,14,14,14,14,14,14,12,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14},
		{14,14,14,14,14,14,14,14,14,14,14,14,14}
	};

	LimpiarBuffer();

	do{
		c = getc(archivo);
		col = columna(c);
		estado_actual = tabla[estado_actual][col];
		if (col!= 11){
			AgregarCaracter(c, i);
			i++;
		}
	}
	while (!estadoFinal(estado_actual) && !(estado_actual == 14) );

	switch(estado_actual){
		case 2:
			if (col != 11){
				ungetc(c, archivo);
				buffer[i-1] = '\0';
			}
			return ID;
		case 4:
			if (col != 11){
				ungetc(c, archivo);
				buffer[i-1] = '\0';
			}
			return CONSTANTE;
		case 5:
			return SUMA;
		case 6:
			return RESTA;
		case 7:
			return PARENIZQUIERDO;
		case 8:
			return PARENDERECHO;
		case 9:
			return COMA;
		case 10:
			return PUNTOYCOMA;
		case 12:
			return ASIGNACION;
		case 13:
			return FDT;
		case 14:
			return ERRORLEXICO;

	}
	return FDT;
}

int VerificarLongitud(void){
	int i;
	while (buffer[i]!='\0'){
		i++;
	}
	return i;
}

void AlmacenarEnTS(TOKEN token){
	strcpy(TS[posUltimoSimbolo], buffer);
	TS_token[posUltimoSimbolo]=token;
	posUltimoSimbolo++;
}

int columna(int c){
	if(isalpha(c)){
		return 0;
	}else if (isdigit(c)){
		return 1;
	}else if (isspace(c)){
		return 11;
	}
	switch (c){
		case '+':
			return 2;
		case '-':
			return 3;
		case '(':
			return 4;
		case ')':
			return 5;
		case ',':
			return 6;
		case ';':
			return 7;
		case ':':
			return 8;
		case '=':
			return 9;
		case EOF:
			return 10;
		default:
			return 12;

	}
}

void AgregarCaracter(int caracter, int posicion){
	buffer[posicion] = caracter;
}

TOKEN EsReservada(void){
	int i;
	for(i=0; i < posUltimoSimbolo; i++){
		if (strcmp(TS[i], buffer)==0){
			return TS_token[i];
		}
	}
	return ID;
}

void LimpiarBuffer(void){
	int i;
	for (i=0; i < TAMLEX; i++){
		buffer[i]='\0';
	}
}
