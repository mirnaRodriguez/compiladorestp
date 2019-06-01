#include "sintactico.h"

/***********************************/

//raiz de la gramatica
void sintaxis(){
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	//synchset de sincronizacion
	elemento(c_siguiente);
}

//puede ser un objeto o un vector
void elemento(int synchset[]){
    int c_primero[] = {L_CORCHETE,L_LLAVE,'\0'};
    int c_siguiente[] = {COMA,R_CORCHETE,R_LLAVE, '\0'};

    checkeo_entrada(c_primero, synchset);

    if(t.compLex == L_LLAVE)
        objeto(c_siguiente);
    else if(t.compLex == L_CORCHETE)
        vector(c_siguiente);

    checkeo_entrada(c_siguiente, c_primero);
}

void vector(int synchset[]){
	int c_primero[] = {L_CORCHETE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	checkeo_entrada(c_primero, synchset);

	if (t.compLex == L_CORCHETE){
		matcheo(L_CORCHETE);
		vector_(c_siguiente);
	}

	checkeo_entrada(c_siguiente, c_primero);

}

void vector_(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, R_CORCHETE, '\0'};

	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, L_CORCHETE, '\0'};

	checkeo_entrada(c_primero, synchset);

	if ((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		
		lista_elemento(c_siguiente);
		matcheo(R_CORCHETE);
	}
	else if(t.compLex == R_CORCHETE){
		matcheo(R_CORCHETE);
		
	}

	checkeo_entrada(c_siguiente, c_primero);
}

void lista_elemento(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, '\0'};
	int c_siguiente[] = {R_CORCHETE, '\0'};

	checkeo_entrada(c_primero, synchset);

	if((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		elemento(c_siguiente);
		lista_elemento_(c_siguiente);
	}

	checkeo_entrada(c_siguiente, c_primero);
}

void lista_elemento_(int synchset[]){

	if(t.compLex == R_CORCHETE)
       return;

	int c_primero[] = {COMA,'\0'};
	int c_siguiente[] = {R_CORCHETE, '\0'};

	checkeo_entrada(c_primero, synchset);

	

	if (t.compLex == COMA){
		matcheo(COMA);
		elemento(c_siguiente);
		lista_elemento_(c_siguiente);
	}

	checkeo_entrada(c_siguiente, c_primero);
}

void objeto(int synchset[]){
	int c_primero[] = {L_LLAVE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	
	checkeo_entrada(c_primero, synchset);

	if (t.compLex == L_LLAVE) {
		matcheo(L_LLAVE);
		objeto_(c_siguiente);
	}
	
	checkeo_entrada(c_siguiente, c_primero);
}

void objeto_(int synchset[]){
	int c_primero[] = {STRING, R_LLAVE, '\0'};

	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, STRING, '\0'};

	checkeo_entrada(c_primero, synchset);

	if (t.compLex == STRING){
		lista_atributo(synchset);
		matcheo(R_LLAVE);
	} else if (t.compLex == R_LLAVE){
		matcheo(R_LLAVE);
	}

	checkeo_entrada(c_siguiente, c_primero);
}

void lista_atributo(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {R_LLAVE, '\0'};

	checkeo_entrada(c_primero, synchset);

	if (t.compLex == STRING){
		atributo(c_siguiente);
		lista_atributo_(c_siguiente);
	}

	checkeo_entrada(c_siguiente, c_primero);

}

void lista_atributo_(int synchset[]){

	if (t.compLex == R_LLAVE)
        return;

	int c_primero[] = {COMA, '\0'};
	int c_siguiente[] = {R_LLAVE, '\0'};

	checkeo_entrada(c_primero, synchset);


	if (t.compLex == COMA){
		matcheo(COMA);
		atributo(c_siguiente);
		lista_atributo_(c_siguiente);
	}

	checkeo_entrada(c_siguiente, c_primero);
}

void atributo(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {COMA, R_LLAVE, '\0'};

	checkeo_entrada(c_primero, synchset);

	if (t.compLex == STRING){
		nombre_atributo(c_siguiente);
		matcheo(DOS_PUNTOS);
		valor_atributo(c_siguiente);
	}

	checkeo_entrada(c_siguiente, c_primero);
}

void nombre_atributo(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {DOS_PUNTOS, '\0'};

	checkeo_entrada(c_primero, synchset);

	if (t.compLex == STRING)
		matcheo(STRING);

	checkeo_entrada(c_siguiente, c_primero);
}

void valor_atributo(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, STRING,
		NUMBER, PR_TRUE, PR_FALSE, PR_NULL, '\0'};
	int c_siguiente[] = {COMA, R_LLAVE, '\0'};

	checkeo_entrada(c_primero, synchset);

	if (t.compLex == L_LLAVE){
		elemento(c_siguiente);
	}
	else if (t.compLex == L_CORCHETE) {
		elemento(c_siguiente);
	}
	else if (t.compLex == STRING){
		matcheo(STRING);
	}
	else if (t.compLex == NUMBER){
		matcheo(NUMBER);
	}
	else if (t.compLex == PR_TRUE){
		matcheo(PR_TRUE);
	}
	else if (t.compLex == PR_FALSE){
		matcheo(PR_FALSE);
	}
	else if (t.compLex == PR_NULL){
		matcheo(PR_NULL);
	}

	checkeo_entrada(c_siguiente, c_primero);

}

//llama al siguiente elemento de la pila
void matcheo(int expectedToken){
    if(t.compLex == expectedToken)
        lexema();
}


void checkeo_entrada(int c_primero[], int c_siguiente[]){

    int indice1=0;
    if(t.compLex == EOF)
    	return;

    //entra cuando no hay errores
    while(c_primero[indice1] != '\0') 
    {
        if(t.compLex == c_primero[indice1])
            return;
        indice1++;
    }
 
 	indice1=0;    
    
    int synchset[]={1}; //acumulador
    
    while(c_primero[indice1] != '\0'){ 
        synchset[indice1] = c_primero[indice1];
        indice1++;
    }
    int indice2=0;

    while(c_siguiente[indice2] != '\0'){
        synchset[indice1] = c_siguiente[indice2];
        indice1++; //aumenta indice1
        indice2++; //aumenta indice2
    }

    escaneo(synchset);

}


//si no esta en el synchset llama a lexema() e imprimer errores del checkeo de entrada
void escaneo(int synch[]){
    errorSintactico();
	int indice1 = 0;
    while(t.compLex != synch[indice1] && t.compLex != EOF){   
        if (synch[indice1] == '\0'){
            lexema();
            indice1=0;        
        }
        indice1++;
    }
    //el lexema que falta luego de salir
    lexema();
    return;    	   
}

/*muestra los errores en pantalla*/
void errorSintactico(){
	aceptado=0;
   	printf("\nError sintactico: Linea %d. Se encontro %s.\n", numLinea, t.lexema);
}

/*Main*/
/*int main (int argc,char* args[]){
    if(argc > 1)
    {
        if (!(entrada=fopen(args[1],"rt")))
        {
            printf("Fuente no encontrada.\n");
            exit(1);
        }
        lexema();
        sintaxis();
        if(aceptado) printf("Sintaxis correcta de la fuente.\n");
        fclose(entrada);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}*/
