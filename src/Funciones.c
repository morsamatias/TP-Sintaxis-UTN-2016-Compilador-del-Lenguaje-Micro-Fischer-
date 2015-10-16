/*
 * Funciones.c
 *
 *  Created on: 6/8/2015
 *      Author: utnso
 */
#include "Funciones.h"

RegTS TS[1000] = { {"inicio", INICIO}, {"fin", FIN}, {"leer", LEER}, {"escribir", ESCRIBIR}, {"$", 99} };
int numeroVariableTemporal=1;

/*----------------------------------------------------Validacion-----------------------------------*/


int validacion (char* archivo,char c){

	int i=0;
	while(archivo[i]!='.'){
		i=i+1;
	}
	if ((archivo[i+1] == c)&&(archivo[i+2] =='\0')){
		return 1;
	}else{
		return 0;
	}
}




/* ------------------------------------------PARSER---------------------------------------------------------------*/

void Match(TOKEN token){
	if (ProximoToken()!=token){
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
	Terminar();
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
	REG_EXPRESION izquierda, derecha;
	switch (tok){
		case ID:
			Identificador(&izquierda);
			Match(ASIGNACION);
			Expresion(&derecha);
			Asignar(izquierda, derecha);
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

void Expresion(REG_EXPRESION * resultado){
	REG_EXPRESION izquierda, derecha;
	TOKEN t;
	char operador[TAMLEX];
	Primaria(&izquierda);
	for(t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken()){
		OperadorAditivo(operador);
		Primaria(&derecha);
		izquierda = GenInfijo(izquierda, operador, derecha);
	}
	*resultado = izquierda;
}

void Primaria(REG_EXPRESION * resultado){
	switch(ProximoToken()){
		case ID:
			Identificador(resultado);
			break;
		case CONSTANTE:
			Match(CONSTANTE);
			*resultado = ProcesarCte();
			break;
		case PARENIZQUIERDO:
			Match(PARENIZQUIERDO);
			Expresion(resultado);
			Match(PARENDERECHO);
			break;
	}
}

void OperadorAditivo(char * resultado){
	TOKEN t = ProximoToken();
	if (t == SUMA || t == RESTA){
		Match(t);
		strcpy(resultado, ProcesarOp());
	}else
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
	REG_EXPRESION reg;
	Identificador(&reg);
	Leer(reg);
	for ( t = ProximoToken(); t == COMA; t = ProximoToken()){
		Match(COMA);
		Identificador(&reg);
		Leer(reg);
	}
}

void ListaExpresiones(void){
	TOKEN t;
	REG_EXPRESION reg;
	Expresion(&reg);
	Escribir(reg);
	for ( t = ProximoToken(); t == COMA; t = ProximoToken()){
		Match(COMA);
		Expresion(&reg);
		Escribir(reg);
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
			Buscar(buffer, TS, &tokenActual);
		}

	}
	return tokenActual;
}

void Identificador(REG_EXPRESION * resultado){
	Match(ID);
	*resultado = ProcesarId();
}

/* -----------------------------------------------SCANNER------------------------------------------------------*/

int esEstadoFinal(int e){
	switch (e){
		case 0: case 1: case 3: case 11: case 14:
			return 0;
	}
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
	}while (!esEstadoFinal(estado_actual));

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

void LimpiarBuffer(void){
	int i;
	for (i=0; i < TAMLEX; i++){
		buffer[i]='\0';
	}
}

/*-----------------------------Rutinas Semanticas-------------------------------*/
REG_EXPRESION ProcesarCte(void){
	REG_EXPRESION reg;
	reg.clase = CONSTANTE;
	strcpy(reg.nombre, buffer);
	sscanf(buffer, "%d", &reg.valor);
	return reg;
}

REG_EXPRESION ProcesarId(void){
	REG_EXPRESION reg;
	Chequear(buffer);
	reg.clase = ID;
	strcpy(reg.nombre, buffer);
	return reg;
}

char * ProcesarOp(void){
	return buffer;
}

void Leer(REG_EXPRESION in){
	Generar("Read", in.nombre, "Entera", "");
}

void Escribir(REG_EXPRESION out){
	Generar("Write", Extraer(&out), "Entera", "");
}

REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2){
	REG_EXPRESION reg;
	char cadenaTemporal[TAMLEX] = "Temp&";
	char numero[TAMLEX];
	char cadenaOperador[TAMLEX];

	if (op[0] == '-')
		strcpy(cadenaOperador, "Restar");
	if (op[0] == '+')
		strcpy(cadenaOperador, "Sumar");
	sprintf(numero, "%d", numeroVariableTemporal);
	numeroVariableTemporal++;
	strcat(cadenaTemporal, numero);
	if (e1.clase == ID)
		Chequear(Extraer(&e1));
	if (e2.clase == ID)
		Chequear(Extraer(&e2));
	Chequear(cadenaTemporal);
	Generar(cadenaOperador, Extraer(&e1), Extraer(&e2), cadenaTemporal);
	strcpy(reg.nombre, cadenaTemporal);
	return reg;
}

void Generar(char * co, char * a, char * b, char * c){
	fprintf(archivoSalida, "%s %s", co, a);

	if (b[0]!='\0')
		fprintf(archivoSalida, "%c%s", ',', b);

	if (c[0]!='\0')
		fprintf(archivoSalida, "%c%s", ',', c);

	fprintf(archivoSalida, "\n");
}

char * Extraer(REG_EXPRESION * reg){
	return reg->nombre;
}

int Buscar(char * id, RegTS * TS, TOKEN * t){
	int i = 0;
	while (strcmp("$", TS[i].identificador)){
		if (!strcmp(id, TS[i].identificador)){
			*t = TS[i].t;
			return 1;
		}
		i++;
	}
	return 0;
}

void Colocar(char * id, RegTS * TS){
	int i = 4;
	while (strcmp("$", TS[i].identificador)) i++;
	if (i <= 999){
		strcpy(TS[i].identificador, id );
		TS[i].t = ID;
		strcpy(TS[++i].identificador, "$" );
	}
}
void Chequear(char * s){
	TOKEN t;
	if (!Buscar(s, TS, &t)){
		Colocar(s, TS);
		Generar("Declara", s, "Entera", "");
	}
}

void Asignar(REG_EXPRESION izq, REG_EXPRESION der){
	Generar("Almacena", Extraer(&der), izq.nombre, "");
}

void Comenzar(void){

}

void Terminar(void){
	Generar("Detiene", "", "", "");
}
