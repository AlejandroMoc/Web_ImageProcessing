//Grayscale
extern void gray_scale(char input_path[40], char name_output[40]) {
    printf("\nEn escala de grises\n");
}

//Mirror horizontal
extern void mirror_horizontal_gray(char input_path[40], char name_output[40]) {
    printf("\nEn espejo respecto a la horizontal en escala de grises\n");
}

extern void mirror_horizontal_color(char input_path[40], char name_output[40]) {
    printf("\nEn espejo respecto a la horizontal a color\n");
}

//Mirror vertical
extern void mirror_vertical_gray(char input_path[80], char name_output[80]){
    printf("\nEn espejo respecto a la vertical en escala de grises\n");
    FILE *image, *outputImage, *lecturas, *fptr;
    char output_path[80] = "./";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");
    printf("%s\n", output_path);
    image = fopen(input_path,"rb");
    outputImage = fopen(output_path,"wb");

	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;

    unsigned char xx[54];
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("File size %li\n", tam);
    tam1 = tam;
    printf("Bits per px %li\n", bpp);
    printf("Image width %li\n",alto);
    printf("Image height %li\n",ancho);

    unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while(!feof(image)){
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_in[j] = pixel;
        j++;
    }

    printf("Read data: %d\n", j * 3);
    printf("Missing elements: %d\n", tam1 - (j * 3));
    
    for(int i = 0; i < ancho*alto; i++){
        fputc(arr_in [(ancho * alto) - i], outputImage);
        fputc(arr_in [(ancho * alto) - i], outputImage);
        fputc(arr_in [(ancho * alto) - i], outputImage);
        
    }
    free(arr_in);
    fclose(image);
    fclose(outputImage);

}

extern void mirror_vertical_color(char input_path[80], char name_output[80]){
    printf("\nEn espejo respecto a la vertical a color\n");
    FILE *image, *outputImage, *lecturas, *fptr;
    char output_path[80] = "./";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");
    printf("%s\n", output_path);
    image = fopen(input_path,"rb");
    outputImage = fopen(output_path,"wb");

	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;

    unsigned char xx[54];
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("File size %li\n", tam);
    tam1 = tam;
    printf("Bits per px %li\n", bpp);
    printf("Image width %li\n",alto);
    printf("Image height %li\n",ancho);

    unsigned char* arr_in_b = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while(!feof(image)){
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        
        arr_in_b[j] = b;
        arr_in_g[j] = g;
        arr_in_r[j] = r;
        j++;
    }

    printf("Read data: %d\n", j * 3);
    printf("Missing elements: %d\n", tam1 - (j * 3));
    
    for(int i = 0; i < ancho*alto; i++){
        
        fputc(arr_in_b [(ancho * alto) - i], outputImage);
        fputc(arr_in_g [(ancho * alto) - i], outputImage);
        fputc(arr_in_r [(ancho * alto) - i], outputImage);
        
    }
    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    fclose(image);
    fclose(outputImage);

}

//Blur
extern void blurred_kernel(char input_path[40], char name_output[40]) {
    printf("\nEfecto de desenfoque con un kernel de 55 hasta 155\n");
}