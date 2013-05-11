#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX 256
#define TUB1 "TUB"
#define TUB2 "TUB1"

int fifo1, fifo2, estado = 1;

char texto[MAX];

void pintar(char *tab);                                             // Pinto Triqui

int pregunta();                                                     // Pregunto si se Desea seguir Jugando   



int main()

{
	

	
    int fila, columna, jugador, triunfo, juego=0, espacios;

    char num, triqui[15][15];

    while (juego==0)

    {

        for (fila=0;fila<15;fila++)                                 // Reseteo Grilla y Jugadores

            for (columna=0;columna<15;columna++)

                triqui[fila][columna]=' ';   

        jugador=1;

        triunfo=0;

        espacios=0;

        for(columna=6;columna<11;columna+=4)                        // Armo Columnas

            for(fila=2;fila<13;fila++)

                triqui[fila][columna]='|';

        for(fila=5;fila<10;fila+=4)

            for(columna=2;columna<15;columna++)                     // Armo Filas

                triqui[fila][columna]='-';

        num='1';

        for(columna=4;columna<13;columna+=4)                        // Numeros Columnas

        {

            triqui[0][columna]=num;       

            num++;

        }

        num='1';

        for(fila=3;fila<12;fila+=4)                                 // Numeros Filas

        {

            triqui[fila][0]=num;       

            num++;

        }

        while(triunfo==0)

        {   

            pintar(&triqui[0][0]);

            fila=columna=0;

                    
            mknod(TUB1, S_IFIFO | 0666, 0);
			mknod(TUB2, S_IFIFO | 0666, 0);
	
		
			switch(estado){
			
			case 1:
			
				printf("Escribiendo\n");
				
				if((fifo1 = open(TUB1, O_WRONLY)) == 0){
					
					perror("fifo1");
					exit(-1);
				
				} else {
					
					do{
						
						printf("\n\n\t	Ingrese coordenadas (fil, col):");
						gets(texto);
						write(fifo1, texto, MAX);
						
						if(strcmp(texto, "fin") == 0){
							
							printf("Adios\n");
							exit(0);
							
						}
						
					} while(strcmp(texto, "fin") == 0);
					
					fila = (int)texto[0] -48;
					columna = (int)texto[2] - 48;
					
								
					close(fifo1);
					
					estado = 2;
				}
				
				break;
				
							
			case 2:
			
				printf("Escuchando\n");
				
				if((fifo2 = open(TUB2, O_RDONLY)) == 0){
					
					perror("fifo2");
					exit(-1);				
					
				} else {
					
					do{
						
						fflush(stdout);
						read(fifo2, texto, MAX);
						printf("%s\n", texto);
						
						if(strcmp(texto, "fin") == 0){
							
							printf("Adios\n");
							exit(0);
							
						}
						
					} while(strcmp(texto, "fin") == 0);
					
					
					fila = (int)texto[0] -48;
					columna = (int)texto[2] - 48;
					
					close(fifo2);
					estado = 1;
					
				}
			
				break;
				
			
		}
		

            if(fila<1||fila>3||columna<1||columna>3)

            {

                printf("\n\t\t Error, coordenadas incorrectas\n\n\t\t ");

                system("pause");

            }

            else if(triqui[fila*3+fila-1][columna*4]!=' ')  

            {

                printf("\n\t\t     Error, espacio ocupado\n\n\t\t ");

                system("pause");

            }

            else

            {

                if(jugador==1)                                      // Lleno Casillas

                    triqui[fila*3+fila-1][columna*4]='X';

                else

                    triqui[fila*3+fila-1][columna*4]='O';

                espacios++;     

                for (fila=1;fila<=3;fila++)                         // Reviso Filas

                {

                    num=triqui[fila*3+fila-1][4];

                    if (num!=' '&&triunfo==0)

                    {

                        for (columna=1;columna<=3;columna++)

                        {

                            if (triqui[fila*3+fila-1][columna*4]!=num)

                                break;

                            else if (columna==3)

                            {

                                triunfo=1;

                                break;

                            }

                        }   

                    }

                }

                for (columna=1;columna<=3;columna++)                // Reviso Columnas

                {

                    num=triqui[3][columna*4];

                    if (num!=' '&&triunfo==0)

                    {

                        for (fila=1;fila<=3;fila++)

                        {

                            if (triqui[fila*3+fila-1][columna*4]!=num)

                                break;

                            else if (fila==3)

                            {

                                triunfo=1;

                                break;

                            }

                        }   

                    }

                }

                columna=0;

                num=triqui[3][4];

                for (fila=1;fila<=3;fila++)                         // Reviso Diagonal Izq - Der

                {

                    if (num!=' '&&triunfo==0)

                    {

                        columna+=4;

                        if (triqui[fila*3+fila-1][columna]!=num)

                            break;

                        else if (columna==12)

                        {

                            triunfo=1;

                            break;

                        }   

                    }

                }

                columna=16;

                num=triqui[3][12];

                for (fila=1;fila<=3;fila++)                         // Reviso Diagonal Der - Izq

                {

                    if (num!=' '&&triunfo==0)

                    {

                        columna-=4;

                        if (triqui[fila*3+fila-1][columna]!=num)

                            break;

                        else if (columna==4)

                        {

                            triunfo=1;

                            break;

                        }   

                    }

                }

                if (triunfo==1)                                     // Victoria

                {

                    pintar(&triqui[0][0]);   

                    if (jugador==1)

                        printf("\t\tGana jugador con X\n");

                    else       

                        printf("\t\tGana jugador con O\n");

                    juego=pregunta();                           

                }

                else                                                //  No Victoria

                {

                    if (jugador==1)

                        jugador=2;

                    else

                        jugador=1;

                    if (espacios==9)                                // Si se Llenaron Todas las Casillas y no Gano Nadie

                    {

                        pintar(&triqui[0][0]);      

                        printf("\t\t\t    Empate\n");

                        triunfo=1;

                        juego=pregunta();

                    }

                }            

            }  

        }

    }              

    return 0;

}



void pintar(char *tab)                                              // Pinto Triqui

{

    int fil,col;   

    system("clear");

    printf("\n\n\t\t\t     Triqui\n\n\n");

    for(fil=0;fil<15;fil++)                                        

    {

        for(col=0;col<15;col++)

        {

            if (col==0)

                printf("\t\t\t");

            printf("%c",tab[(15*fil)+col]);

            if (col==14)

                printf("\n");

        }

    }

}



int pregunta()                                                      // Pregunto si se Desea seguir Jugando

{

    char tec=' ';

    while(tec!='s'&&tec!='n')

    {   

        printf("\n\n\t\tNueva Partida? (s/n): ");

        scanf("%c",&tec);

    } 

    if (tec=='n')

        return 1;

    else

        return 0;     

}
