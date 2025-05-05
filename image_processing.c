#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_functions.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

//gcc -fopenmp image_processing.c
//./a.out

//Funciones para múltiples imágenes
int process_images_gray(const char *input_dir, const char *output_dir_gray) {
    DIR *dir;
    struct dirent *ent;
    char input_path[256];
    char output_path_gray[256];
    char base_filename[256];
    char *dot;

    dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        //Verificar si es un archivo regular
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error al obtener estatus de archivo para %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Omitiendo archivo no regular: %s\n", input_path);
            continue;
        }

        //Extraer nombre de archivo sin extensión
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        //Constrir ruta de salida, pasar por función
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_gray.bmp", output_dir_gray, base_filename);
        printf("Processing gray: %s -> %s\n", input_path, output_path_gray);

        if (gray_scale(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing gray for %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

int process_images_mirror_horizontal_gray(const char *input_dir, const char *output_dir_gray) {
    DIR *dir;
    struct dirent *ent;
    char input_path[256];
    char output_path_gray[256];
    char base_filename[256];
    char *dot;

    dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        //Verificar si es un archivo regular
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error al obtener estatus de archivo para %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Omitiendo archivo no regular: %s\n", input_path);
            continue;
        }

        //Extraer nombre de archivo sin extensión
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        //Constrir ruta de salida, pasar por función
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_horizontal_gray.bmp", output_dir_gray, base_filename);
        printf("Processing mirror horizontal gray: %s -> %s\n", input_path, output_path_gray);

        if (mirror_horizontal_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing mirror horizontal gray for %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

int process_images_mirror_horizontal_color(const char *input_dir, const char *output_dir_gray) {
    DIR *dir;
    struct dirent *ent;
    char input_path[256];
    char output_path_gray[256];
    char base_filename[256];
    char *dot;

    dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        //Verificar si es un archivo regular
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error al obtener estatus de archivo para %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Omitiendo archivo no regular: %s\n", input_path);
            continue;
        }

        //Extraer nombre de archivo sin extensión
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        //Constrir ruta de salida, pasar por función
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_horizontal_color.bmp", output_dir_gray, base_filename);
        printf("Processing mirror horizontal color: %s -> %s\n", input_path, output_path_gray);

        if (mirror_horizontal_color(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing mirror horizontal color for %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

int process_images_mirror_vertical_gray(const char *input_dir, const char *output_dir_gray) {
    DIR *dir;
    struct dirent *ent;
    char input_path[256];
    char output_path_gray[256];
    char base_filename[256];
    char *dot;

    dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        //Verificar si es un archivo regular
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error al obtener estatus de archivo para %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Omitiendo archivo no regular: %s\n", input_path);
            continue;
        }

        //Extraer nombre de archivo sin extensión
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        //Constrir ruta de salida, pasar por función
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_vertical_gray.bmp", output_dir_gray, base_filename);
        printf("Processing mirror vertical gray: %s -> %s\n", input_path, output_path_gray);

        if (mirror_vertical_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing mirror vertical gray for %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

int process_images_mirror_vertical_color(const char *input_dir, const char *output_dir_gray) {
    DIR *dir;
    struct dirent *ent;
    char input_path[256];
    char output_path_gray[256];
    char base_filename[256];
    char *dot;

    dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        //Verificar si es un archivo regular
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error al obtener estatus de archivo para %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Omitiendo archivo no regular: %s\n", input_path);
            continue;
        }

        //Extraer nombre de archivo sin extensión
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        //Constrir ruta de salida, pasar por función
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_vertical_color.bmp", output_dir_gray, base_filename);
        printf("Processing mirror vertical color: %s -> %s\n", input_path, output_path_gray);

        if (mirror_vertical_color(input_path, output_path_gray) != 0) {
        fprintf(stderr, "Error processing mirror vertical color for %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

int process_images_blur_color(const char *input_dir, const char *output_dir_blur, int kernel_size) {
    if (kernel_size < 55 || kernel_size > 155 || kernel_size % 2 == 0) {
        printf("Kernel inválido. Debe ser impar y entre 5 y 15.\n");
        return 1;
    }

    DIR *dir;
    struct dirent *ent;
    char input_path[256];
    char output_path_blur[256];
    char base_filename[256];
    char *dot;

    dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error obteniendo información de %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Omitiendo archivo no regular: %s\n", input_path);
            continue;
        }

        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        snprintf(output_path_blur, sizeof(output_path_blur), "%s/%s_blur_%dx%d.bmp", output_dir_blur, base_filename, kernel_size, kernel_size);

        printf("Procesando desenfoque: %s -> %s\n", input_path, output_path_blur);

        if (blur_image_color(input_path, output_path_blur, kernel_size) != 0) {
            fprintf(stderr, "Error aplicando desenfoque a %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}


//Ejecución principal
int main() {

    //Directorios, crear si no existen
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

    //TODO open 100 pictures

    //Paralelization
    #pragma omp parallel
    {
        #pragma omp sections
        {
            //Grayscale
            #pragma omp section
            {
                process_images_gray(input_dir, output_dir_gray);
            }

            //Mirror horizontal gray
            #pragma omp section
            {
                process_images_mirror_horizontal_gray(input_dir, output_dir_horizontalgray);
            }

            //Mirror horizontal color
            #pragma omp section
            {
                process_images_mirror_horizontal_color(input_dir, output_dir_horizontalcolor);
            }

            //Mirror vertical gray
            #pragma omp section
            {
                //TODO if cycle to go over all images in the Original folder
                process_images_mirror_vertical_gray(input_dir, output_dir_verticalgray);
            }

            //Mirror vertical color
            #pragma omp section
            {
                process_images_mirror_vertical_color(input_dir, output_dir_verticalcolor);
            }

            //Blur (55 to 155)
            #pragma omp section
            {
                process_images_blur_color(input_dir, output_dir_blur ,55);
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