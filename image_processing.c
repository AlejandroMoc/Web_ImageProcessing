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
        printf("Procesando escala de grises: %s -> %s\n", input_path, output_path_gray);

        if (gray_scale(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar escala de grises para %s\n", input_path);
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
        printf("Procesando volteado horizontal a byn: %s -> %s\n", input_path, output_path_gray);

        if (mirror_horizontal_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado horizontal a byn para %s\n", input_path);
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
        printf("Procesando volteado horizontal a color: %s -> %s\n", input_path, output_path_gray);

        if (mirror_horizontal_color(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado horizontal a color para %s\n", input_path);
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
        printf("Procesando volteado verticalmente a byn: %s -> %s\n", input_path, output_path_gray);

        if (mirror_vertical_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado verticalmente a byn para %s\n", input_path);
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
        printf("Procesando volteado verticalmente a color: %s -> %s\n", input_path, output_path_gray);

        if (mirror_vertical_color(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado verticalmente a color para %s\n", input_path);
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