#include <stdio.h>
#include <stdlib.h>

//gcc -fopenmp image_processing.c
//./a.out

int main() {
    int tiempo_ejecucion = 1;

    //en escala de grises
    //en espejo respecto de la horizontal a color
    //en espejo respecto a la vertical a color
    //en espejo respecto de la horizontal en escala de grises
    //en espejo respecto a la vertical en escala de grises
    //efecto de desenfoque con un kernel de 55 hasta 155

    //generar un archivo de texto de salida (.txt), donde indique el n´umero de localidades totales
    //leidas por cada archivo original y las localidades totales de salida escritas despu´es del procesamiento

    printf("El valor del tiempo es %d.\n", tiempo_ejecucion);
}