
#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "process_functions.h"
#include "export.h"


#define NUM_THREADS 36

#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

//gcc -fopenmp image_processing.c
//./a.out
//gcc -shared -o image_processing.so -fPIC image_processing.c -fopenmp

void create_directories(const char *path) {
    char temp_path[256];
    char *p = NULL;
    size_t len;

    snprintf(temp_path, sizeof(temp_path), "%s", path);
    len = strlen(temp_path);
    if (temp_path[len - 1] == '/') {
        temp_path[len - 1] = '\0';
    }
    for (p = temp_path + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            MKDIR(temp_path);
            *p = '/';
        }
    }
    MKDIR(temp_path);
}

void create_directory(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        if (mkdir(dir, 0700) == 0) {
            printf("Directorio creado: %s\n", dir);
        } else {
            perror("Error al crear directorio");
        }
    } else {
        printf("El directorio ya existe: %s\n", dir);
    }
}

EXPORT int processing_all(const char *input_dir, int blur_ratio) {
    char output_dir_gray[256];
    char output_dir_horizontalgray[256];
    char output_dir_horizontalcolor[256];
    char output_dir_verticalgray[256];
    char output_dir_verticalcolor[256];
    char output_dir_blur[256];

    snprintf(output_dir_gray, sizeof(output_dir_gray), "%s/Result/Gray", input_dir);
    create_directories(output_dir_gray);
    snprintf(output_dir_horizontalgray, sizeof(output_dir_horizontalgray), "%s/Result/HorizontalGray", input_dir);
    create_directories(output_dir_horizontalgray);
    snprintf(output_dir_horizontalcolor, sizeof(output_dir_horizontalcolor), "%s/Result/HorizontalColor", input_dir);
    create_directories(output_dir_horizontalcolor);
    snprintf(output_dir_verticalgray, sizeof(output_dir_verticalgray), "%s/Result/VerticalGray", input_dir);
    create_directories(output_dir_verticalgray);
    snprintf(output_dir_verticalcolor, sizeof(output_dir_verticalcolor), "%s/Result/VerticalColor", input_dir);
    create_directories(output_dir_verticalcolor);
    snprintf(output_dir_blur, sizeof(output_dir_blur), "%s/Result/Blur", input_dir);
    create_directories(output_dir_blur);

    FILE *report_file = fopen("report.txt", "w");
    if (report_file == NULL) {
        printf("Archivo nulo\n");
        exit(1);
    }

    printf("Elegiste %d como valor del desenfoque.\n", blur_ratio);

    omp_set_num_threads(NUM_THREADS);
    const double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            { process_images_gray(input_dir, output_dir_gray); }

            #pragma omp section
            { process_images_mirror_horizontal_gray(input_dir, output_dir_horizontalgray); }

            #pragma omp section
            { process_images_mirror_horizontal_color(input_dir, output_dir_horizontalcolor); }

            #pragma omp section
            { process_images_mirror_vertical_gray(input_dir, output_dir_verticalgray); }

            #pragma omp section
            { process_images_mirror_vertical_color(input_dir, output_dir_verticalcolor); }

            #pragma omp section
            { process_images_blur_color(input_dir, output_dir_blur, blur_ratio); }
        }
    }

    const double end_time = omp_get_wtime();
    double tiempo_total = end_time - start_time;
    printf("El valor del tiempo que tomó fue %lf segundos\n", tiempo_total);
    fprintf(report_file, "El valor del tiempo que tomó fue %lf segundos\n", tiempo_total);
    fclose(report_file);

}

int main(int argc, char *argv[]) {
    const char *input_dir = "Images/Original";
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <blur_ratio>\n", argv[0]);
        return 1;
    }
    int blur_ratio = atoi(argv[1]);
    if (blur_ratio < 55 || blur_ratio > 155 || blur_ratio % 2 == 0) {
        fprintf(stderr, "Valor de blur_ratio inválido. Debe ser impar y entre 55 y 155.\n");
        return 1;
    }
    processing_all(input_dir, blur_ratio);
    return 0;

}
