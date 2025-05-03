#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    //TODO open 100 pictures

    //Paralelization
    #pragma omp parallel
    {
        #pragma omp sections
        {
            //Grayscale
            #pragma omp section
            {
                (void)gray_scale("Images/Original/espacio.bmp", "Images/Result/Gray/espacio_gray");
            }

            //Mirror horizontal gray
            #pragma omp section
            {
                (void)mirror_horizontal_gray("Images/Original/espacio.bmp", "Images/Result/HorizontalGray/espacio_horizontal_gray");
            }

            //Mirror horizontal color
            #pragma omp section
            {
                (void)mirror_horizontal_color("Images/Original/espacio.bmp", "Images/Result/HorizontalColor/espacio_horizontal_color");
            }

            //Mirror vertical gray
            #pragma omp section
            {
                (void)mirror_vertical_gray("Images/Original/espacio.bmp", "Images/Result/VerticalGray/espacio_vertical_gray");
            }

            //Mirror vertical color
            #pragma omp section
            {
                (void)mirror_vertical_color("Images/Original/espacio.bmp", "Images/Result/VerticalColor/espacio_vertical_color");
            }

            //Blur (55 to 155)
            #pragma omp section
            {
                //TODO ask blur ratio

                //TODO if cycle to go over all images in the Original folder
                (void)blurred_kernel("Images/Original/espacio.bmp", "Images/Result/Blur/espacio_blurred");
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

    printf("El valor del tiempo es %d.\n", tiempo_ejecucion);   //TODO
    return 0;
}