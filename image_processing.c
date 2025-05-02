#include <stdio.h>
#include <stdlib.h>

//gcc -fopenmp image_processing.c
//./a.out

//Funciones
void gray_scale() {
    printf("\nEn escala de grises\n");
}

void mirror_horizontal_color() {
    printf("\nEn espejo respecto a la horizontal a color\n");
}

void mirror_vertical_color() {
    printf("\nEn espejo respecto a la vertical a color\n");
}

void mirror_horizontal_gray() {
    printf("\nEn espejo respecto a la horizontal en escala de grises\n");
}

void mirror_vertical_gray() {
    printf("\nEn espejo respecto a la vertical en escala de grises\n");
}

void blurred_kernel() {
    printf("\nEfecto de desenfoque con un kernel de 55 hasta 155\n");
}


//Ejecución principal
int main() {
    //TODO abrir 100 imágenes

    #pragma omp parallel
    {
        #pragma omp sections
        {
            //En escala de grises
            #pragma omp section
            {
                (void)gray_scale();
            }

            //En espejo respecto de la horizontal a color
            #pragma omp section
            {
                (void)mirror_horizontal_color();
            }

            //En espejo respecto a la vertical a color
            #pragma omp section
            {
                (void)mirror_vertical_color();
            }

            //En espejo respecto de la horizontal en escala de grises
            #pragma omp section
            {
                (void)mirror_horizontal_gray();
            }

            //En espejo respecto a la vertical en escala de grises
            #pragma omp section
            {
                (void)mirror_vertical_gray();
            }

            //Efecto de desenfoque con un kernel de 55 hasta 155
            #pragma omp section
            {
                (void)blurred_kernel();
            }

        }
    }
    
    int tiempo_ejecucion = 1;

    //generar un archivo de texto de salida (.txt), donde indique el n´umero de localidades totales
    //leidas por cada archivo original y las localidades totales de salida escritas despu´es del procesamiento

    printf("El valor del tiempo es %d.\n", tiempo_ejecucion);
    return 0;
}