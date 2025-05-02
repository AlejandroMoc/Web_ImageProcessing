#include <stdio.h>
#include <stdlib.h>
#include "process_functions.h"

//gcc -fopenmp image_processing.c
//./a.out

//Ejecución principal
int main() {

    //Abrir archivo de reporte
    FILE *report_file;
    char data[80] = "report.txt";
    report_file = fopen(data, "w");
    if (report_file == NULL){
        printf("Archivo nulo\n");
        exit(1);
    }

    //TODO abrir 100 imágenes

    //Paralelización
    #pragma omp parallel
    {
        #pragma omp sections
        {
            //En escala de grises
            #pragma omp section
            {
                (void)gray_scale("filepath");
            }

            //En espejo respecto de la horizontal a color
            #pragma omp section
            {
                (void)mirror_horizontal_color("filepath");
            }

            //En espejo respecto a la vertical a color
            #pragma omp section
            {
                (void)mirror_vertical_color("filepath");
            }

            //En espejo respecto de la horizontal en escala de grises
            #pragma omp section
            {
                (void)mirror_horizontal_gray("filepath");
            }

            //En espejo respecto a la vertical en escala de grises
            #pragma omp section
            {
                (void)mirror_vertical_gray("filepath");
            }

            //Efecto de desenfoque con un kernel de 55 hasta 155
            #pragma omp section
            {
                (void)blurred_kernel("filepath");
            }

        }
    }
    
    int tiempo_ejecucion = 1;

    //TODO generar un archivo de texto de salida (.txt), donde indique el no. de localidades totales
    //leidas por cada archivo original y las localidades totales de salida escritas después del procesamiento

    fprintf(report_file, "Reporte de resultados\n");
    fprintf(report_file, "Localidades leídas\n");
    fprintf(report_file, "Localidades totales\n");
    fclose(report_file);

    printf("El valor del tiempo es %d.\n", tiempo_ejecucion);
    return 0;
}