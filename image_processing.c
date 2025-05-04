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


int process_images_gray(const char *input_dir, const char *output_dir_gray) {
    DIR *dir;
    struct dirent *ent;
    char input_path[256];
    char output_path_gray[256];
    char base_filename[256];
    char *dot;

    dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        // Check if it's a regular file
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error getting file status for %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Skipping non-regular file: %s\n", input_path);
            continue;
        }

        // Extract filename without extension
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        // Construct output paths
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_gray.bmp", output_dir_gray, base_filename);

        // Process the image
        printf("Processing gray: %s -> %s\n", input_path, output_path_gray);

        // Call the mirroring function
        
        if (gray_scale(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing gray mirror for %s\n", input_path);
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
        perror("Error opening directory");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        // Check if it's a regular file
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error getting file status for %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Skipping non-regular file: %s\n", input_path);
            continue;
        }

        // Extract filename without extension
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        // Construct output paths
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_horizontal_gray.bmp", output_dir_gray, base_filename);

        // Process the image
        printf("Processing gray: %s -> %s\n", input_path, output_path_gray);

        // Call the mirroring function
        if (mirror_horizontal_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing gray mirror for %s\n", input_path);
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
        perror("Error opening directory");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        // Check if it's a regular file
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error getting file status for %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Skipping non-regular file: %s\n", input_path);
            continue;
        }

        // Extract filename without extension
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        // Construct output paths
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_horizontal_color.bmp", output_dir_gray, base_filename);

        // Process the image
        printf("Processing gray: %s -> %s\n", input_path, output_path_gray);

        // Call the mirroring function
        if (mirror_horizontal_color(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing gray mirror for %s\n", input_path);
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
        perror("Error opening directory");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        // Check if it's a regular file
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error getting file status for %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Skipping non-regular file: %s\n", input_path);
            continue;
        }

        // Extract filename without extension
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        // Construct output paths
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_vertical_gray.bmp", output_dir_gray, base_filename);

        // Process the image
        printf("Processing gray: %s -> %s\n", input_path, output_path_gray);

        // Call the mirroring function
        if (mirror_vertical_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error processing gray mirror for %s\n", input_path);
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
        perror("Error opening directory");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, ent->d_name);

        // Check if it's a regular file
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Error getting file status for %s\n", input_path);
            continue;
        }

        if (!S_ISREG(path_stat.st_mode)) {
            printf("Skipping non-regular file: %s\n", input_path);
            continue;
        }

        // Extract filename without extension
        dot = strrchr(ent->d_name, '.');
        if (dot != NULL) {
            size_t base_len = dot - ent->d_name;
            strncpy(base_filename, ent->d_name, base_len);
            base_filename[base_len] = '\0';
        } else {
            strcpy(base_filename, ent->d_name);
        }

        // Construct output paths
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_vertical_color.bmp", output_dir_gray, base_filename);

        // Process the image
        printf("Processing gray: %s -> %s\n", input_path, output_path_gray);

        // Call the mirroring function
        // Call the mirroring function
        if (mirror_vertical_color(input_path, output_path_gray) != 0) {
        fprintf(stderr, "Error processing gray mirror for %s\n", input_path);
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
                
                //(void)gray_scale("Images/Original/espacio.bmp", "Images/Result/Gray/espacio_gray");
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
                //TODO if cycle to go over all images in the Original folder
                process_images_mirror_vertical_color(input_dir, output_dir_verticalcolor);
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