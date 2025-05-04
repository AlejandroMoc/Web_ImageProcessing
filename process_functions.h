#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///FUNCIONES

//Escala gris
extern void gray_scale(char input_path[40], char name_output[40]) {
    printf("\nEn escala de grises\n");
    FILE *image, *outputImage;
    char output_path[100] = "./";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    unsigned char r, g, b, pixel;
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    while(!feof(image)){
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        if (feof(image)) break;
        pixel = 0.21*r + 0.72*g + 0.07*b;
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
    }

    fclose(image);
    fclose(outputImage);
}

//Mirror horizontal
extern void mirror_horizontal_gray(char input_path[40], char name_output[40]) {
    printf("\nEn espejo respecto a la horizontal en escala de grises\n");
    FILE *image, *outputImage;
    char output_path[100] = "./";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* row = (unsigned char*)malloc(row_padded);
    unsigned char* gray_row = (unsigned char*)malloc(row_padded);

    for (int i = 0; i < height; i++) {
        fread(row, sizeof(unsigned char), row_padded, image);
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
    }

    free(row);
    free(gray_row);
    fclose(image);
    fclose(outputImage);
}

extern void mirror_horizontal_color(char input_path[40], char name_output[40]) {
    printf("\nEn espejo respecto a la horizontal a color\n");
    FILE *image, *outputImage;
    char output_path[100] = "./";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* row = (unsigned char*)malloc(row_padded);
    unsigned char* mirrored_row = (unsigned char*)malloc(row_padded);

    for (int i = 0; i < height; i++) {
        fread(row, sizeof(unsigned char), row_padded, image);
        for (int j = 0; j < width; j++) {
            int src_idx = j * 3;
            int dst_idx = (width - 1 - j) * 3;
            mirrored_row[dst_idx]     = row[src_idx];
            mirrored_row[dst_idx + 1] = row[src_idx + 1];
            mirrored_row[dst_idx + 2] = row[src_idx + 2];
        }
        fwrite(mirrored_row, sizeof(unsigned char), row_padded, outputImage);
    }

    free(row);
    free(mirrored_row);
    fclose(image);
    fclose(outputImage);
}

//Mirror vertical
extern void mirror_vertical_gray(char input_path[80], char name_output[80]){
    printf("\nEn espejo respecto a la vertical en escala de grises\n");
    FILE *image, *outputImage;
    char output_path[100] = "./";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");

    image = fopen(input_path,"rb");
    outputImage = fopen(output_path,"wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* *rows = (unsigned char**)malloc(height * sizeof(unsigned char*));

    for (int i = 0; i < height; i++) {
        rows[i] = (unsigned char*)malloc(row_padded);
        fread(rows[i], sizeof(unsigned char), row_padded, image);
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
    }

    free(rows);
    fclose(image);
    fclose(outputImage);
}

extern void mirror_vertical_color(char input_path[80], char name_output[80]){
    printf("\nEn espejo respecto a la vertical a color\n");
    FILE *image, *outputImage;
    char output_path[100] = "./";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.\n");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char** rows = (unsigned char**)malloc(height * sizeof(unsigned char*));

    for (int i = 0; i < height; i++) {
        rows[i] = (unsigned char*)malloc(row_padded);
        fread(rows[i], sizeof(unsigned char), row_padded, image);
    }

    for (int i = height - 1; i >= 0; i--) {
        fwrite(rows[i], sizeof(unsigned char), row_padded, outputImage);
        free(rows[i]);
    }

    free(rows);
    fclose(image);
    fclose(outputImage);
}

//Blur
extern void blurred_kernel(char input_path[40], char name_output[40]) {
    printf("\nEfecto de desenfoque con un kernel de 55 hasta 155\n");
}