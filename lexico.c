#include "lexico.h"

/************* Variables globales **************/

token t;				    // token global para recibir componentes del Analizador Lexico


/***** Variables para el analizador lexico *****/

FILE *entrada;			    // Fuente json
FILE *salida;				// Salida del analisis lexico
char id[TAMLEX];		    // Utilizado por el analizador lexico
int numLinea=1;			    // Numero de Linea

/**************** Funciones **********************/

// Rutinas del analizador lexico
void error(const char* mensaje)
{
	printf("\nLin %d: Error Lexico. %s",numLinea,mensaje);	
}

void lexema()
{
	int indice=0;
    char car=0;
    int bandera=0;
	int acepto=0;
	int estado=0;
	char msg[50];
	char aux[TAMAUX] = " "; // Vector auxiliar para leer false null true
     
   	while((car=fgetc(entrada))!=EOF)
	{
        if(car=='\n')
	    {
		    //incrementar el numero de linea
		    numLinea++;
//			fputs("\n",salida);
		    continue;
	    }
	    else if (car==' ')
	    {
            do
            {
                // se encarga de cargar los espacios leidos
//				fputs(" ",salida);
                car=fgetc(entrada); 
            }while(car ==' ');
            car=ungetc(car,entrada);
	    }
        else if (car=='\t')
        { 
            //si es un tabulador que guarde los 4 espacios correspondientes
            while(car=='\t')
            {
//				fputs("\t",salida);
	        	car=fgetc(entrada);
            }
        }
    	else if (isdigit(car))
	    {
            //es un numero
            indice=0;
            estado=0;
            acepto=0;
            id[indice]=car;
			while(!acepto)
			{
				switch(estado)
				{
				    case 0: //una secuencia netamente de digitos, puede ocurrir . o e
						car=fgetc(entrada);
						if (isdigit(car))
						{
							id[++indice]=car;
							estado=0;
						}
						else if(car=='.')
						{
							id[++indice]=car;
							estado=1;
						}
						else if(tolower(car)=='e')
						{
							id[++indice]=car;
							estado=3;
						}
						else
							estado=6;
						break;		
					case 1://un punto, debe seguir un digito 
						car=fgetc(entrada);						
						if (isdigit(car))
						{
							id[++indice]=car;
							estado=2;
						}
						else{
							sprintf(msg,"No se esperaba '%c' despues del . ",car);
							fputs("Error lexico",salida);
							estado=-1;
						}
						break;
					case 2://la fraccion decimal, pueden seguir los digitos o e
						car=fgetc(entrada);
						if (isdigit(car))
						{
							id[++indice]=car;
							estado=2;
						}
						else if(tolower(car)=='e')
						{
							id[++indice]=car;
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://una e, puede seguir +, - o una secuencia de digitos
						car=fgetc(entrada);
						if (car=='+' || car=='-')
						{
							id[++indice]=car;
							estado=4;
						}
						else if(isdigit(car))
						{
							id[++indice]=car;
							estado=5;
						}
						else
						{
							sprintf(msg,"Se esperaba signo o digitos despues del exponente");
							fputs("Error lexico",salida);
							estado=-1;
						}
						break;
					case 4://necesariamente debe venir por lo menos un digito
						car=fgetc(entrada);
						if (isdigit(car))
						{
							id[++indice]=car;
							estado=5;
						}
						else
						{
							sprintf(msg,"No se esperaba '%c' despues del signo",car);
							fputs("Error lexico",salida);
							estado=-1;
						}
						break;
					case 5://una secuencia de digitos correspondiente al exponente
						car=fgetc(entrada);
						if (isdigit(car))
						{
							id[++indice]=car;
							estado=5;
						}
						else
							estado=6;
						break;
					case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
						if (car!=EOF)
							ungetc(car,entrada);
						else
                            car=0;
						id[++indice]='\0';
						acepto=1;
                        t.compLex=NUMBER;
						strcpy(t.lexema,id);
//						fputs("NUMBER",salida);
//						fputs(" ",salida);
						break;
					case -1:
						if (car==EOF){
                            error("No se esperaba el fin de entrada\n");
							fputs("Error lexico",salida);
						}
						else
                            error(msg);
                        acepto=1;
                    t.compLex=VACIO;
                    while(car!='\n')
                        car=fgetc(entrada);
                    ungetc(car,entrada);
					break;
				}
			}
			break;
		}
        else if (car=='\"')
		{
            //un caracter o una cadena de caracteres
			indice=0;
			id[indice]=car;
			indice++;
			do
			{
				car=fgetc(entrada);
				if (car=='\"')
				{
                    id[indice]=car;
                    indice++;
                    bandera=1;
                    break;
				}
                else if(car==EOF || car==',' || car=='\n' || car==':')
				{
                    sprintf(msg,"Se esperaba que finalice el literal");
					error(msg);
					fputs("Error lexico",salida);
                    
                    while(car!='\n')
                        car=fgetc(entrada);

                    ungetc(car,entrada);
                    break;                       
				}
				else
				{
					id[indice]=car;
					indice++;
				}
			}while(isascii(car) || bandera==0);
			    id[indice]='\0';
            strcpy(t.lexema,id);
			t.compLex = STRING;
//			fputs("STRING",salida);
//			fputs(" ",salida);
			break;
		}
		else if (car==':')
		{
            //puede ser un :
            t.compLex=DOS_PUNTOS;
            strcpy(t.lexema,":");
//			fputs("DOS_PUNTOS",salida);
//			fputs(" ",salida);
            break;
		}
		else if (car==',')
		{
			t.compLex=COMA;
			strcpy(t.lexema,",");
//			fputs("COMA",salida);
//			fputs(" ",salida);
			break;
		}
		else if (car=='[')
		{
			t.compLex=L_CORCHETE;
			strcpy(t.lexema,"[");
//			fputs("L_CORCHETE",salida);
//			fputs(" ",salida);
			break;
		}
		else if (car==']')
		{
			t.compLex=R_CORCHETE;
			strcpy(t.lexema,"]");
//			fputs("R_CORCHETE",salida);
//			fputs(" ",salida);
			break;
		}
		else if (car=='{')
		{
			t.compLex=L_LLAVE;
			strcpy(t.lexema,"{");
//			fputs("L_LLAVE",salida);
//			fputs(" ",salida);
			break;		
        }
        else if (car=='}')
		{
			t.compLex=R_LLAVE;
			strcpy(t.lexema,"}");
//			fputs("R_LLAVE",salida);
//			fputs(" ",salida);			
			break;		
        }
		else if (car=='n' || car=='N')
        {
            ungetc(car,entrada);
            fgets(aux,5,entrada);//ver si es null
            if (strcmp(aux, "null")==0 || strcmp(aux, "NULL")==0)
            {
                t.compLex = PR_NULL;
                strcpy(t.lexema,aux);
//				fputs("PR_NULL",salida);
//				fputs(" ",salida);
            }
            else
            {
                sprintf(msg,"%c no esperado",car);
			    error(msg);
				fputs("Error lexico",salida);

                while(car!='\n')
                    car=fgetc(entrada);

                t.compLex = VACIO;
                ungetc(car,entrada);
            }
            break;
        }   
        else if (car=='f' || car=='F')
        {
            ungetc(car,entrada);
            fgets(aux,6,entrada);//ver si es null
            if (strcmp(aux, "false")==0 || strcmp(aux, "FALSE")==0)
            {
                t.compLex = PR_FALSE;
                strcpy(t.lexema,aux);
//				fputs("PR_FALSE",salida);
//				fputs(" ",salida);
            }
            else{
                sprintf(msg,"%c no esperado",car);
			    error(msg);
				fputs("Error lexico",salida);

                while(car!='\n')
                    car=fgetc(entrada);    

                t.compLex = VACIO;
                ungetc(car,entrada);
            }
            break;
        }   
        else if (car=='t' || car=='T')
        {
            ungetc(car,entrada);
            fgets(aux,5,entrada);//ver si es null
            if (strcmp(aux, "true")==0 || strcmp(aux, "TRUE")==0)
            {
                t.compLex = PR_TRUE;
                strcpy(t.lexema,aux);
//				fputs("PR_TRUE",salida);
//				fputs(" ",salida);
            }
            else
            {
                sprintf(msg,"%c no esperado",car);
			    error(msg);
				fputs("Error lexico",salida);

                while(car!='\n')
                    car=fgetc(entrada);

                t.compLex = VACIO;
                ungetc(car,entrada);
            }
            break;
        }
        else if (car!=EOF)
		{
			sprintf(msg,"%c no esperado",car);
			error(msg);
			fputs("Error lexico",salida);
            while(car!='\n')
                car=fgetc(entrada);
            ungetc(car,entrada);
		}
	}
	if (car==EOF)
	{
		t.compLex=EOF;
		strcpy(t.lexema,"EOF");
		sprintf(t.lexema,"EOF");
//		fputs("EOF",salida);
//		fputs(" ",salida);
	}
}

/*int main(int argc,char* args[])
{
	//apertura y lectura del archivo fuente
	if(argc > 1)
	{
		if (!(entrada=fopen(args[1],"rt")))
		{
			printf("Fuente no encontrada.\n");
			exit(1);
		}
		salida = fopen("output.txt","w");
		while (t.compLex!=EOF)
		{
			lexema();
		}
		fclose(entrada);
		fclose(salida);
	}
	else
	{
		printf("Debe pasar como parametro el path al entrada fuente.\n");
		exit(1);
	}

	return 0;
}*/