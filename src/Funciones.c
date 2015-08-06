/*
 * Funciones.c
 *
 *  Created on: 6/8/2015
 *      Author: utnso
 */
#include "Funciones.h"

int tamanioo_archivo(int fd){
	struct stat buf;
	fstat(fd, &buf);
	return buf.st_size;
}

char* copiar_archivo_a_buffer(char* archivo_origen){

	printf ("Copiando archivo:%s \n",archivo_origen);
	int mapper2;
	char* mapeo2;
	int tamanio2;
	char* buffer2;

	if(( mapper2 = open (archivo_origen, O_RDONLY) ) == -1){
		fprintf(stderr, "No existe el archivo solicitado\n");
		return(NULL);
	}
	tamanio2 = tamanioo_archivo(mapper2);

	//buffer = (char*)malloc(tamanio+1);

	if( (mapeo2 = mmap( NULL, tamanio2, PROT_READ, MAP_SHARED|MAP_NORESERVE, mapper2, 0 )) == MAP_FAILED){
		fprintf(stderr, "Error al ejecutar MMAP del archivo '%s' de tamaño: %d: %s\n", archivo_origen, tamanio2, strerror(errno));
		return(NULL);
	}
	buffer2=strdup(mapeo2);
	//strcpy(buffer,mapeo);

	munmap( mapeo2, tamanio2 );
	close(mapper2);
	return(buffer2);

}


int redireccionar(char* fuente,char* salida,char* script)
{
	pid_t pid;

    int   pipes[NUM_PIPES][2];

    char buffer[100];
    char* copia;

    pipe(pipes[PARENT_WRITE_PIPE] );

    pid=fork();

    if(!pid) {


        dup2(CHILD_READ_FD, STDIN_FILENO);

        close(CHILD_READ_FD);
        close(PARENT_WRITE_FD);

		FILE* f;
        f=fopen(salida,"w");

        FILE* f2=popen(script,"r");

        while ( fgets( buffer, sizeof(buffer), f2))
          {
            fprintf(f,"%s", buffer);
          }

         pclose(f2);
         fclose(f);

		 _Exit(EXIT_SUCCESS);


    } else {

        copia=copiar_archivo_a_buffer(fuente);

	    write(PARENT_WRITE_FD, copia, strlen(copia));
	    close(PARENT_WRITE_FD);
        waitpid(pid,0,0);

		}


    return(0);
 }
