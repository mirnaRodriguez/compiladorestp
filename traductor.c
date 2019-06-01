#include "traductor.h"

// sintaxis_trad -> elemento.trad 
void sintaxis_trad(){
    elemento_trad();
}

//elemento.trad -> objeto.trad | vector.trad
void elemento_trad(){
    if(t.compLex == L_LLAVE){
        objeto_trad();
    }
    else if(t.compLex == L_CORCHETE){
        vector_trad();
    }
}

//vector.trad -> vectorA.trad 
void vector_trad(){  
    if(t.compLex == L_CORCHETE){
        matcheo(L_CORCHETE);
        vectorA_trad();
    }
}

//vectorA.trad -> <item> lista_elemento.trad </item> | ε
void vectorA_trad(){
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
		fputs("\n",salida);
        imprimir_espacios();
		fputs("<item>\n",salida);
		espacios+=1;
        lista_elemento_trad();
        matcheo(R_CORCHETE);
        imprimir_espacios();
		fputs("</item>\n",salida);
    }
    else if(t.compLex == R_CORCHETE){
		espacios-=1;
        matcheo(R_CORCHETE);
    }
}

//lista_elemento.trad -> elemento.trad lista_elementoA.trad 
void lista_elemento_trad(){
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        elemento_trad();
        lista_elementoA_trad();
        
    }
}

//lista_elementoA.trad -> </item> <item> elemento.trad lista_elementoA.trad | ε
void lista_elementoA_trad(){
    if(t.compLex == COMA){  
        matcheo(COMA);
		imprimir_espacios();
		fputs("</item>\n",salida);
		imprimir_espacios();
		fputs("<item>\n",salida);
		espacios+=1;
        elemento_trad();
        lista_elementoA_trad();     
    }
}

//objeto.trad -> objetoA.trad 
void objeto_trad(){
    if(t.compLex == L_LLAVE){
        matcheo(L_LLAVE);
        objetoA_trad();
    }
}

//objetoA.trad -> lista_atributo.trad | ε
void objetoA_trad(){
    if(t.compLex == STRING){
        lista_atributo_trad();
        matcheo(R_LLAVE);

    }
    else if(t.compLex == R_LLAVE){
        matcheo(R_LLAVE);
    }
}

//lista_atributo.trad -> atributo.trad 
void lista_atributo_trad(){
    if(t.compLex == STRING){
        atributo_trad();
        lista_atributoA_trad();
    }
}

//lista_atributoA.trad ->atributo.trad lista_atributoA.trad | ε
void lista_atributoA_trad(){
    if (t.compLex == R_LLAVE){
        espacios-=1;
		return;
    }
    if(t.compLex == COMA){
        matcheo(COMA);
        atributo_trad();
        lista_atributoA_trad();
    }
}

//atributo.trad -> <nombre_atributo.trad> valor_atributo.trad </nombre_atributo.trad>
void atributo_trad(){
    if(t.compLex == STRING){
		imprimir_espacios();
        char lexema[TAMLEX];
        strcpy(lexema,t.lexema);
		fputs("<",salida);
        nombre_atributo_trad();
		fputs(">",salida);
        matcheo(DOS_PUNTOS);
        valor_atributo_trad();
		fputs("</",salida);
        string_sin_comillas(lexema);
		fputs(">\n",salida);		
    }
}

//nombre_atributo.trad -> string.trad_sin_comilla
void nombre_atributo_trad(){
    if(t.compLex == STRING){    
        string_sin_comillas(t.lexema);
        matcheo(STRING);
    }
}

//valor_atributo.trad -> elemento_trad | string | number | true | false | null

void valor_atributo_trad(){
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
		espacios += 1;
        elemento_trad();
    }
    else if(t.compLex == STRING){
		fputs(t.lexema, salida);
        matcheo(STRING);
    }
    else if(t.compLex == NUMBER){
		fputs(t.lexema, salida);
        matcheo(NUMBER);
    }
    else if(t.compLex == PR_TRUE){
		fputs(t.lexema, salida);
        matcheo(PR_TRUE);
    }
    else if(t.compLex == PR_FALSE){
		fputs(t.lexema, salida);
        matcheo(PR_FALSE);
    }
    else if(t.compLex == PR_NULL){
		fputs(t.lexema, salida);
        matcheo(PR_NULL);
    }
}

void string_sin_comillas(char* cadena){
    char string[TAMLEX];
	char cad[TAMLEX] = "";
    strcpy(string,cadena);
    int i=1;
    while(string[i] != '"'){
		cad[i-1] = string[i];
        i++;
    }
	fputs(cad, salida);
}

void imprimir_espacios(){
    int i;
    for (i=1;i<=espacios;i++){
        fputs("\t", salida);
    }
}


/*Main*/
int main (int argc,char* args[]){
    if(argc > 1)
    {
        if (!(entrada=fopen(args[1],"rt")))
        {
            printf("Fuente no encontrada.\n");
            exit(1);
        }
        lexema();
        sintaxis();
        if(aceptado){
            fclose(entrada);
            entrada=fopen(args[1],"rt");
			salida=(fopen("output.xml","w"));
            lexema();
            sintaxis_trad();
        }
		fclose(salida);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}
