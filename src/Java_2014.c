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

int tamanio_archivo(int fd){
	struct stat buf;
	fstat(fd, &buf);
	return buf.st_size;
}


/////////////////gcc2 fuente salida

int main(int argc,char* argv[]) {

	if (argc>3){

		fprintf(stderr,"Por favor solo ingrese el archivo fuente y el nombre que desea darle al ejecutable. \n");
		return (1);

	}else{

		if (argc <3){
			if (argc==2)
				fprintf(stderr,"Por favor ingrese el nombre que desea darle al ejecutable.\n");
			if (argc==1)
				fprintf(stderr,"Por favor ingrese el nombre del archivo fuente y el nombre que desea darle al ejecutable.\n");
			return (1);
		}
	}

	redireccionar( argv[1],argv[2],"script");


	return 0;
}
