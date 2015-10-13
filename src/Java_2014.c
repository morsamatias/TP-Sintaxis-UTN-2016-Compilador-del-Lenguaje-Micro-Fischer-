
#include "Funciones.h"

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
	if (validacion(argv[1],'m') && validacion(argv[2],'o')){
		char * ruta = strdup(argv[1]);
		archivo = fopen(ruta, "rb");
		if (archivo == NULL){
			printf("El archivo no existe\n");
			return -1;
		}
		Objetivo();
		fclose(archivo);
	}else{
		printf("La extension del archivo no es valida\n");
	}
	return EXIT_SUCCESS;
}
