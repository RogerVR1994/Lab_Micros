#include <stm32f10x.h>
//Se trata de un código de multiplicación de matrices que utiliza el método
//convencional para lograr esta tarea.
//para poder comprenderlo mejor, es necesario comenzar desde la linea con: "for( k=0; k<3; k++ )"


const uint16_t A[3][3] = { { 1,3,5 }, { 5,1,3 }, { 3,5,1 } };
const uint16_t B[3][3] = { { 2,4,6 }, { 6,2,4 }, { 4,6,2 } };

int main( void )
{
	__IO uint16_t C[3][3] = { { 0,0,0 }, { 0,0,0 }, { 0,0,0 } };
	__IO uint16_t i,j,k,accum;
	

       //Se hace un ciclo que se repetirá 3 veces acumulando las columnas de C
	for( i=0; i<3; i++ )
	{
		for( j=0; j<3; j++ ){			
			accum = 0;

                     //Se hace un ciclo que se repetirá 3 veces realizando la multiplicación que habrá en           cada espacio de C

			for( k=0; k<3; k++ ){
                      //Se realiza la multiplicación  entre cada elemento de la matríz. 	

				accum += A[i][k]*B[k][j];	
			}
//Se hace multiplicando las filas de A por las columnas de B. Se van sumando los resultados

//En esta línea se cóloca en la celda respectiva el resultado de la fila A con la columna B (ya sumados)
			C[i][j] = accum;	
		}

	}	
//El ciclo principal permite pasar a los resultados de las filas que deberán colocarse en C
	
	for(;;);
	
}
