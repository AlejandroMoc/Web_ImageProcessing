#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "export.h"

///FUNCIONES INDIVIDUALES

//Escala gris
EXPORT extern int gray_scale(char input_path[40], char name_output[40]) {
    printf("\nEn escala de grises\n");
    
    FILE *image, *outputImage, *report;
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s", name_output);
    snprintf(output_path, sizeof(output_path), "%s.bmp", name_output);

    image = fopen(input_path, "rb");
    if (!image) {
        perror("Error al abrir imagen de entrada");
        return 1;
    }

    outputImage = fopen(output_path, "wb");
    if (!outputImage) {
        perror("Error al crear imagen de salida");
        fclose(image);
        return 1;
    }

    if (!image || !outputImage) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    unsigned char r, g, b, pixel;
    unsigned char header[54];
    int pixels_processed = 0;

    // Leer y copiar el encabezado
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    // Procesar los píxeles
    while (!feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);

        if (feof(image)) break;

        pixel = 0.21 * r + 0.72 * g + 0.07 * b;

        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);

        pixels_processed++;
    }

    fclose(image);
    fclose(outputImage);

    // Escribir en el archivo de reporte
    report = fopen("report.txt", "a");  // modo append
    if (report != NULL) {
        fprintf(report, "Imagen gray_scale: %s\n", input_path);
        fprintf(report, "Localidades leídas: %d\n", pixels_processed);
        fprintf(report, "Localidades escritas: %d\n\n", pixels_processed);
        fclose(report);
    } else {
        perror("No se pudo abrir el archivo de reporte");
    }

    return 0;
}

//Voltear horizontalmente
EXPORT extern int mirror_horizontal_gray(char input_path[40], char name_output[40]) {
    printf("\nEn espejo respecto a la horizontal en escala de grises\n");
    FILE *image, *outputImage, *report;
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s", name_output);
    snprintf(output_path, sizeof(output_path), "%s.bmp", name_output);

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    // Contadores de localidades (por byte)
    int localidades_leidas = 54;     
    int localidades_escritas = 54; 

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* row = (unsigned char*)malloc(row_padded);
    unsigned char* gray_row = (unsigned char*)malloc(row_padded);

    for (int i = 0; i < height; i++) {
        fread(row, sizeof(unsigned char), row_padded, image);
        localidades_leidas += row_padded;
        for (int j = 0; j < width; j++) {
            int idx = j * 3;
            unsigned char r = row[idx+2];
            unsigned char g = row[idx+1];
            unsigned char b = row[idx];
            unsigned char gray = 0.21*r + 0.72*g + 0.07*b;
            gray_row[(width - 1 - j)*3]     = gray;
            gray_row[(width - 1 - j)*3 + 1] = gray;
            gray_row[(width - 1 - j)*3 + 2] = gray;
        }
        fwrite(gray_row, sizeof(unsigned char), row_padded, outputImage);
        localidades_escritas += row_padded;
    }

    free(row);
    free(gray_row);
    fclose(image);
    fclose(outputImage);

     // Escribir en el reporte
     report = fopen("report.txt", "a");
     if (report != NULL) {
         fprintf(report, "Imagen mirror_horizontal_gray: %s\n", input_path);
         fprintf(report, "Localidades leídas: %d\n", localidades_leidas);
         fprintf(report, "Localidades escritas: %d\n\n", localidades_escritas);
         fclose(report);
     } else {
         perror("No se pudo abrir el archivo de reporte");
     }
    return 0;
}

EXPORT extern int mirror_horizontal_color(char input_path[40], char name_output[40]) {
    printf("\nEn espejo respecto a la horizontal a color\n");

    FILE *image, *outputImage, *report;
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s", name_output);
    snprintf(output_path, sizeof(output_path), "%s.bmp", name_output);

    image = fopen(input_path, "rb");
    if (!image) {
        fprintf(stderr, "Error al abrir imagen de entrada: %s\n", input_path);
        return 1;
    }

    outputImage = fopen(output_path, "wb");
    if (!outputImage) {
        fprintf(stderr, "Error al crear imagen de salida: %s\n", output_path);
        fclose(image);
        return 1;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* row = (unsigned char*)malloc(row_padded);
    unsigned char* mirrored_row = (unsigned char*)malloc(row_padded);

    int localidades_leidas = 54;
    int localidades_escritas = 54;

    for (int i = 0; i < height; i++) {
        fread(row, sizeof(unsigned char), row_padded, image);
        localidades_leidas += row_padded;

        for (int j = 0; j < width; j++) {
            int src_idx = j * 3;
            int dst_idx = (width - 1 - j) * 3;
            mirrored_row[dst_idx] = row[src_idx];
            mirrored_row[dst_idx + 1] = row[src_idx + 1];
            mirrored_row[dst_idx + 2] = row[src_idx + 2];
        }

        fwrite(mirrored_row, sizeof(unsigned char), row_padded, outputImage);
        localidades_escritas += row_padded;
    }

    free(row);
    free(mirrored_row);
    fclose(image);
    fclose(outputImage);

    report = fopen("report.txt", "a");
    if (report != NULL) {
        fprintf(report, "Imagen mirror_horizontal_color: %s\n", input_path);
        fprintf(report, "Localidades leídas: %d\n", localidades_leidas);
        fprintf(report, "Localidades escritas: %d\n\n", localidades_escritas);
        fclose(report);
    } else {
        perror("No se pudo abrir el archivo de reporte");
    }

    return 0;
}


//Voltear verticalmente
EXPORT extern int mirror_vertical_gray(char input_path[80], char name_output[80]){
    printf("\nEn espejo respecto a la vertical en escala de grises\n");
    FILE *image, *outputImage, *report;
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s", name_output);
    snprintf(output_path, sizeof(output_path), "%s.bmp", name_output);

    image = fopen(input_path,"rb");
    outputImage = fopen(output_path,"wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.");
        return 0;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* *rows = (unsigned char**)malloc(height * sizeof(unsigned char*));

    // Contadores de localidades (por byte)
    int localidades_leidas = 54;     
    int localidades_escritas = 54;   

    for (int i = 0; i < height; i++) {
        rows[i] = (unsigned char*)malloc(row_padded);
        fread(rows[i], sizeof(unsigned char), row_padded, image);
        localidades_leidas += row_padded;
    }

    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            int idx = j * 3;
            unsigned char r = rows[i][idx+2];
            unsigned char g = rows[i][idx+1];
            unsigned char b = rows[i][idx];
            unsigned char gray = 0.21*r + 0.72*g + 0.07*b;
            rows[i][idx] = gray;
            rows[i][idx+1] = gray;
            rows[i][idx+2] = gray;
        }
        fwrite(rows[i], sizeof(unsigned char), row_padded, outputImage);
        free(rows[i]);
        localidades_escritas += row_padded;
    }

    free(rows);
    fclose(image);
    fclose(outputImage);
    // Escribir en el reporte
    report = fopen("report.txt", "a");  
    if (report != NULL) {
        fprintf(report, "Imagen mirror_vertical_gray: %s\n", input_path);
        fprintf(report, "Localidades leídas: %d\n", localidades_leidas);
        fprintf(report, "Localidades escritas: %d\n\n", localidades_escritas);
        fclose(report);
    } else {
        perror("No se pudo abrir el archivo de reporte");
    }
    return 0;
}

EXPORT extern int mirror_vertical_color(char input_path[80], char name_output[80]){
    printf("\nEn espejo respecto a la vertical a color\n");
    FILE *image, *outputImage, *report;
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s", name_output);
    snprintf(output_path, sizeof(output_path), "%s.bmp", name_output);

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char** rows = (unsigned char**)malloc(height * sizeof(unsigned char*));

    // Contadores de localidades (por byte)
    int localidades_leidas = 54;     
    int localidades_escritas = 54;   

    for (int i = 0; i < height; i++) {
        rows[i] = (unsigned char*)malloc(row_padded);
        fread(rows[i], sizeof(unsigned char), row_padded, image);
        localidades_leidas += row_padded;
    }

    for (int i = height - 1; i >= 0; i--) {
        fwrite(rows[i], sizeof(unsigned char), row_padded, outputImage);
        free(rows[i]);
        localidades_escritas += row_padded;
    }

    free(rows);
    fclose(image);
    fclose(outputImage);
    // Escribir en el reporte
    report = fopen("report.txt", "a");  
    if (report != NULL) {
        fprintf(report, "Imagen mirror_vertical_color: %s\n", input_path);
        fprintf(report, "Localidades leídas: %d\n", localidades_leidas);
        fprintf(report, "Localidades escritas: %d\n\n", localidades_escritas);
        fclose(report);
    } else {
        perror("No se pudo abrir el archivo de reporte");
    }
    return 0;
}

//Desenfoque
EXPORT extern int blur_image_color(char input_path[80], char name_output[80], int kernel_size) {
    if (kernel_size < 55 || kernel_size > 155 || kernel_size % 2 == 0) {
        printf("Kernel inválido. Debe ser impar y entre 55 y 155.\n");
        return 1;
    }

    printf("\nAplicando blur con kernel %dx%d\n", kernel_size, kernel_size);

    FILE *image, *outputImage, *report;
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s", name_output);
    snprintf(output_path, sizeof(output_path), "%s.bmp", name_output);

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);

    unsigned char** input_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));
    unsigned char** output_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));

    // Contadores de localidades (por byte)
    int localidades_leidas = 54;    
    int localidades_escritas = 54;   

    for (int i = 0; i < height; i++) {
        input_rows[i] = (unsigned char*)malloc(row_padded);
        output_rows[i] = (unsigned char*)malloc(row_padded);
        fread(input_rows[i], sizeof(unsigned char), row_padded, image);
        localidades_leidas += row_padded;
    }

    int k = kernel_size / 2;

    // Paso intermedio: blur horizontal
    unsigned char** temp_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));
    for (int y = 0; y < height; y++) {
        temp_rows[y] = (unsigned char*)malloc(row_padded);
        for (int x = 0; x < width; x++) {
            int sumB = 0, sumG = 0, sumR = 0, count = 0;

            for (int dx = -k; dx <= k; dx++) {
                int nx = x + dx;
                if (nx >= 0 && nx < width) {
                    int idx = nx * 3;
                    sumB += input_rows[y][idx + 0];
                    sumG += input_rows[y][idx + 1];
                    sumR += input_rows[y][idx + 2];
                    count++;
                }
            }

            int index = x * 3;
            temp_rows[y][index + 0] = sumB / count;
            temp_rows[y][index + 1] = sumG / count;
            temp_rows[y][index + 2] = sumR / count;
        }

        for (int p = width * 3; p < row_padded; p++) {
            temp_rows[y][p] = input_rows[y][p];
        }
    }

    // Paso final: blur vertical
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumB = 0, sumG = 0, sumR = 0, count = 0;

            for (int dy = -k; dy <= k; dy++) {
                int ny = y + dy;
                if (ny >= 0 && ny < height) {
                    int idx = x * 3;
                    sumB += temp_rows[ny][idx + 0];
                    sumG += temp_rows[ny][idx + 1];
                    sumR += temp_rows[ny][idx + 2];
                    count++;
                }
            }

            int index = x * 3;
            output_rows[y][index + 0] = sumB / count;
            output_rows[y][index + 1] = sumG / count;
            output_rows[y][index + 2] = sumR / count;
        }

        for (int p = width * 3; p < row_padded; p++) {
            output_rows[y][p] = temp_rows[y][p];
        }
    }

    // Escritura y liberación de memoria
    for (int i = 0; i < height; i++) {
        fwrite(output_rows[i], sizeof(unsigned char), row_padded, outputImage);
        free(input_rows[i]);
        free(output_rows[i]);
        free(temp_rows[i]);
        localidades_escritas += row_padded;
    }

    free(input_rows);
    free(output_rows);
    free(temp_rows);
    fclose(image);
    fclose(outputImage);

    // Reporte
    report = fopen("report.txt", "a"); 
    if (report != NULL) {
        fprintf(report, "Imagen blur_image_color: %s\n", input_path);
        fprintf(report, "Localidades leídas: %d\n", localidades_leidas);
        fprintf(report, "Localidades escritas: %d\n\n", localidades_escritas);
        fclose(report);
    } else {
        perror("No se pudo abrir el archivo de reporte");
    }

    return 0;
}



//FUNCIONES POR LOTES
//Funciones para múltiples imágenes
EXPORT extern int process_images_gray(const char *input_dir, const char *output_dir_gray) {
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
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_gray", output_dir_gray, base_filename);
        printf("Procesando escala de grises: %s -> %s\n", input_path, output_path_gray);

        if (gray_scale(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar escala de grises para %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

EXPORT extern int process_images_mirror_horizontal_gray(const char *input_dir, const char *output_dir_gray) {
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
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_horizontal_gray", output_dir_gray, base_filename);
        printf("Procesando volteado horizontal a byn: %s -> %s\n", input_path, output_path_gray);

        if (mirror_horizontal_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado horizontal a byn para %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

EXPORT extern int process_images_mirror_horizontal_color(const char *input_dir, const char *output_dir_gray) {
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
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_horizontal_color", output_dir_gray, base_filename);
        printf("Procesando volteado horizontal a color: %s -> %s\n", input_path, output_path_gray);

        if (mirror_horizontal_color(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado horizontal a color para %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

EXPORT extern int process_images_mirror_vertical_gray(const char *input_dir, const char *output_dir_gray) {
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
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_vertical_gray", output_dir_gray, base_filename);
        printf("Procesando volteado verticalmente a byn: %s -> %s\n", input_path, output_path_gray);

        if (mirror_vertical_gray(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado verticalmente a byn para %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

EXPORT extern int process_images_mirror_vertical_color(const char *input_dir, const char *output_dir_gray) {
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
        snprintf(output_path_gray, sizeof(output_path_gray), "%s/%s_vertical_color", output_dir_gray, base_filename);
        printf("Procesando volteado verticalmente a color: %s -> %s\n", input_path, output_path_gray);

        if (mirror_vertical_color(input_path, output_path_gray) != 0) {
            fprintf(stderr, "Error al procesar volteado verticalmente a color para %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

EXPORT extern int process_images_blur_color(const char *input_dir, const char *output_dir_blur, int kernel_size) {
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

        snprintf(output_path_blur, sizeof(output_path_blur), "%s/%s_blur_%dx%d", output_dir_blur, base_filename, kernel_size, kernel_size);

        printf("Procesando desenfoque: %s -> %s\n", input_path, output_path_blur);

        if (blur_image_color(input_path, output_path_blur, kernel_size) != 0) {
            fprintf(stderr, "Error aplicando desenfoque a %s\n", input_path);
        }
    }

    closedir(dir);
    return 0;
}

