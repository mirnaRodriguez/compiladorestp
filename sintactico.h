/*********** Inclusión de cabecera **************/

#include "lexico.c"
#define CANT_COMP_LEX 10

/***********************************

sintaxis -> elemento EOF
elemento -> objeto | vector
vector -> [ vector_
vector_ -> lista_elemento ] | ]
elemento-list -> elemento lista_elemento_
lista_elemento_ ->  ,elemento lista_elemento_ | e
objeto -> { objeto_
objeto_ -> lista_atributo} | }
atributo-list -> atributo atributo-list_
atributo-list_ -> atributo lista_atributo_ | e
atributo -> nombre_atributo : valor_atributo
nombre_atributo -> string
valor_atributo -> elemento | string | number | true | false | null

***********************************/

/**Prototipos de funciones**/

void sintaxis();
void elemento(int []);
void vector(int []);
void vector_(int []);
void lista_elemento(int []);
void lista_elemento_(int []);
void objeto(int []);
void objeto_(int []);
void atributo(int []);
void lista_atributo(int []);
void lista_atributo_(int []);
void nombre_atributo(int []);
void valor_atributo(int []);
int aceptado=1;

/**Funciones de control**/

void matcheo(int);
void checkeo_entrada(int [], int []);
void escaneo(int []);
void errorSintactico();
