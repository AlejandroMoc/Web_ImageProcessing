#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "process_functions.h"


#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

//gcc -fopenmp image_processing.c
//./a.out

//Ejecución principal
int main() {

    //Directorios, crearlos si no existen
    const char *input_dir = "Images/Original";
    const char *output_dir_gray = "Images/Result/Gray";
    const char *output_dir_horizontalgray = "Images/Result/HorizontalGray";
    const char *output_dir_horizontalcolor = "Images/Result/HorizontalColor";
    const char *output_dir_verticalgray = "Images/Result/VerticalGray";
    const char *output_dir_verticalcolor = "Images/Result/VerticalColor";
    const char *output_dir_blur = "Images/Result/Blur";

    MKDIR(output_dir_gray);
    MKDIR(output_dir_horizontalgray);
    MKDIR(output_dir_horizontalcolor);
    MKDIR(output_dir_verticalgray);
    MKDIR(output_dir_verticalcolor);
    MKDIR(output_dir_blur);
    
    //Abrir archivo de reporte
    FILE *report_file;
    char data[80] = "report.txt";
    report_file = fopen(data, "w");
    if (report_file == NULL){
        printf("Archivo nulo\n");
        exit(1);
    }

    //Solicitar valor de desenfoque
    int blur_ratio;
    do {
      printf("Introduce un no. impar entre 55 y 155: ");
      scanf("%d", &blur_ratio);

      if (blur_ratio < 55 || blur_ratio > 155 || blur_ratio % 2 == 0) {
        printf("Entrada inválida. Por favor introduce un valor impar entre 55 y 155.\n");
      }
    } while (blur_ratio < 55 || blur_ratio > 155 || blur_ratio % 2 == 0);
    printf("Elegiste %d como valor del desenfoque.\n", blur_ratio);

    //Empezar conteo de tiempo
    const double start_time = omp_get_wtime();

    //Paralelización
    #pragma omp parallel
    {
        #pragma omp sections
        {
            //Escala de grises
            #pragma omp section
            {
                process_images_gray(input_dir, output_dir_gray);
            }

            //Voltear horizontalmente a byn
            #pragma omp section
            {
                process_images_mirror_horizontal_gray(input_dir, output_dir_horizontalgray);
            }

            //Voltear horizontalmente a color
            #pragma omp section
            {
                process_images_mirror_horizontal_color(input_dir, output_dir_horizontalcolor);
            }

            //Voltear verticalmente a byn
            #pragma omp section
            {
                process_images_mirror_vertical_gray(input_dir, output_dir_verticalgray);
            }

            //Voltear verticalmente a color
            #pragma omp section
            {
                process_images_mirror_vertical_color(input_dir, output_dir_verticalcolor);
            }

            //Desenfoque (valor de 55 a 155)
            #pragma omp section
            {
                process_images_blur_color(input_dir, output_dir_blur , blur_ratio);
            }
        }
    }

    //Generar un archivo de texto de salida (.txt), 
    //donde se indique el no. de localidades totales leidas
    //y escritas por cada archivo original
    fprintf(report_file, "Reporte de resultados\n");
    fprintf(report_file, "Localidades leídas\n");
    fprintf(report_file, "Localidades totales\n");

    //Calcular tiempo de ejecución
    const double end_time = omp_get_wtime();
    printf("El valor del tiempo que tomó fue %lf segundos\n", (end_time - start_time));
    fprintf(report_file, "El valor del tiempo que tomó fue %lf segundos\n", (end_time - start_time));
    fclose(report_file);


    return 0;
}