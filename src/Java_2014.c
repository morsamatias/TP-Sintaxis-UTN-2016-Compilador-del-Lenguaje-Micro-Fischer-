/*
 ============================================================================
 Name        : Java_2014.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Funciones.h"


int main(int argc, char * argv[]) {
	/*if(argc==1){
		printf("Debe ingresar una cadena en linea de comandos\n");
		return -1;
	}
	if(argc!=2){
		printf("Numero incorrecto de argumentos\n");
		return -2;
	}

	argv[1]=strdup("+3");
	if(!verifica(argv[1])){
		printf("Los caracteres no pertenecen al alfabeto\n");
		return -3;
	}
	if(automata(argv[1])){
		printf("Es palabra del lenguaje\n");
	}else{
		printf("No es palabra del lenguaje\n");
	}*/


	char * ruta = strdup("archivo.m");
	archivo = fopen(ruta, "rb");
	if (archivo == NULL){
		printf("El archivo no existe");
		abort();
	}

	Objetivo();
	fclose(archivo);
	return EXIT_SUCCESS;
}
