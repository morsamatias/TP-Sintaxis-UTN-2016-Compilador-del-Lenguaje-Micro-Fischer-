/*
 * Funciones.c
 *
 *  Created on: 6/8/2015
 *      Author: utnso
 */
#include "Funciones.h"

RegTS TS[1000] = { {"inicio", INICIO}, {"fin", FIN}, {"leer", LEER}, {"escribir", ESCRIBIR}, {"$", 99} };

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
	REG_EXPRESION izq, der;
	switch (tok){
		case ID:
			Identificador(&izq);
			Match(ASIGNACION);
			Expresion(&der);
			Asignar(izq, der);
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

void Expresion(REG_EXPRESION * presul){
	REG_EXPRESION operandoIzq, operandoDer;
	TOKEN t;
	char op[TAMLEX];
	Primaria(&operandoIzq);
	for(t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken()){
		OperadorAditivo(op);
		Primaria(&operandoDer);
		operandoIzq = GenInfijo(operandoIzq, op, operandoDer);
	}
	*presul = operandoIzq;
}

void Primaria(REG_EXPRESION * presul){
	//todo: VERIFICAR SI ESTA BIEN PRIMARIA
	switch(ProximoToken()){
		case ID:
			Identificador(presul);
			break;
		case CONSTANTE:
			Match(CONSTANTE);
			*presul = ProcesarCte();
			break;
		case PARENIZQUIERDO:
			Match(PARENIZQUIERDO);
			Expresion(presul);
			Match(PARENDERECHO);
			break;
	}
}

void OperadorAditivo(char * presul){
	TOKEN t = ProximoToken();
	if (t == SUMA || t == RESTA){
		Match(t);
		strcpy(presul, ProcesarOp());
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

void Identificador(REG_EXPRESION * presul){
	Match(ID);
	*presul = ProcesarId();
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

/******************Rutinas Semanticas******************************/
REG_EXPRESION ProcesarCte(void)
{
/* Convierte cadena que representa numero a numero entero y construye un registro semantico */
REG_EXPRESION reg;
reg.clase = CONSTANTE;
strcpy(reg.nombre, buffer);
sscanf(buffer, "%d", &reg.valor);
return reg;
}
REG_EXPRESION ProcesarId(void)
{
/* Declara ID y construye el correspondiente registro semantico */
REG_EXPRESION reg;
Chequear(buffer);
reg.clase = ID;
strcpy(reg.nombre, buffer);
return reg;
}
char * ProcesarOp(void)
{
/* Declara OP y construye el correspondiente registro semantico */
return buffer;
}
void Leer(REG_EXPRESION in)
{
/* Genera la instruccion para leer */
Generar("Read", in.nombre, "Entera", "");
}
void Escribir(REG_EXPRESION out)
{
/* Genera la instruccion para escribir */
Generar("Write", Extraer(&out), "Entera", "");
}
REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2)
{
/* Genera la instruccion para una operacion infija y construye un registro semantico con el resultado */
REG_EXPRESION reg;
static unsigned int numTemp = 1;
char cadTemp[TAMLEX] ="Temp&";
char cadNum[TAMLEX];
char cadOp[TAMLEX];
if ( op[0] == '-' ) strcpy(cadOp, "Restar");
if ( op[0] == '+' ) strcpy(cadOp, "Sumar");
sprintf(cadNum, "%d", numTemp);
numTemp++;
strcat(cadTemp, cadNum);
if ( e1.clase == ID) Chequear(Extraer(&e1));
if ( e2.clase == ID) Chequear(Extraer(&e2));
Chequear(cadTemp);
Generar(cadOp, Extraer(&e1), Extraer(&e2), cadTemp);
strcpy(reg.nombre, cadTemp);
return reg;
}

void Generar(char * co, char * a, char * b, char * c)
{
	/* Produce la salida de la instruccion para la MV por stdout */
	fprintf(archivoSalida, "%s %s%c%s%c%s\n", co, a, ',', b, ',', c);
}
char * Extraer(REG_EXPRESION * preg)
{
/* Retorna la cadena del registro semantico */
return preg->nombre;
}

int Buscar(char * id, RegTS * TS, TOKEN * t){
	/* Determina si un identificador esta en la TS */
	int i = 0;
	while ( strcmp("$", TS[i].identifi) )
	{
	if ( !strcmp(id, TS[i].identifi) )
	{
	*t = TS[i].t;
	return 1;
	}
	i++;
	}
	return 0;
}
void Colocar(char * id, RegTS * TS)
{
/* Agrega un identificador a la TS */
int i = 4;
while ( strcmp("$", TS[i].identifi) ) i++;
if ( i < 999 )
{
strcpy(TS[i].identifi, id );
TS[i].t = ID;
strcpy(TS[++i].identifi, "$" );
}
}
void Chequear(char * s)
{
/* Si la cadena No esta en la Tabla de Simbolos la agrega, y si es el nombre de una variable genera la instruccion */
TOKEN t;
if ( !Buscar(s, TS, &t) )
{
Colocar(s, TS);
Generar("Declara", s, "Entera", "");
}
}
void Comenzar(void)
{
/* Inicializaciones Semanticas */
}
void Terminar(void)
{
/* Genera la instruccion para terminar la ejecucion del programa */
Generar("Detiene", "", "", "");
}
void Asignar(REG_EXPRESION izq, REG_EXPRESION der)
{
/* Genera la instruccion para la asignacion */
Generar("Almacena", Extraer(&der), izq.nombre, "");
}
