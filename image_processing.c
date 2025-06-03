#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mpi.h>
#include <stdatomic.h>
#include "process_functions.h"

#define NUM_THREADS 4
#define MAX_IMAGES 1000

#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

void get_image_list(const char* dir_path, char image_list[MAX_IMAGES][256], int* total) {
    DIR* dir;
    struct dirent* entry;
    *total = 0;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error al abrir el directorio de entrada");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".bmp")) {
            snprintf(image_list[*total], 256, "%s/%s", dir_path, entry->d_name);
            (*total)++;
            if (*total >= MAX_IMAGES) break;
        }
    }
    closedir(dir);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <blur_ratio> <input_dir>\n", argv[0]);
        return 1;
    }

    int blur_ratio = atoi(argv[1]);
    const char* input_dir = argv[2];

    int rank, size;
    char hostname[256];
    char image_list[MAX_IMAGES][256];
    int total_images;

    const char *output_dir_gray = "Images/Result/Gray";
    const char *output_dir_horizontalgray = "Images/Result/HorizontalGray";
    const char *output_dir_horizontalcolor = "Images/Result/HorizontalColor";
    const char *output_dir_verticalgray = "Images/Result/VerticalGray";
    const char *output_dir_verticalcolor = "Images/Result/VerticalColor";
    const char *output_dir_blur = "Images/Result/Blur";

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    gethostname(hostname, sizeof(hostname));

    if (rank == 0) {
        MKDIR(output_dir_gray);
        MKDIR(output_dir_horizontalgray);
        MKDIR(output_dir_horizontalcolor);
        MKDIR(output_dir_verticalgray);
        MKDIR(output_dir_verticalcolor);
        MKDIR(output_dir_blur);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&blur_ratio, 1, MPI_INT, 0, MPI_COMM_WORLD);

    omp_set_num_threads(NUM_THREADS);
    double start_time = MPI_Wtime();

    if (rank == 0) {
        get_image_list(input_dir, image_list, &total_images);
    }

    MPI_Bcast(&total_images, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(image_list, MAX_IMAGES * 256, MPI_CHAR, 0, MPI_COMM_WORLD);

    int images_per_proc = total_images / size;
    int remainder = total_images % size;
    int start = rank * images_per_proc + (rank < remainder ? rank : remainder);
    int count = images_per_proc + (rank < remainder ? 1 : 0);

    printf("[RANK %d] Host %s procesando %d imágenes desde índice %d\n", rank, hostname, count, start);

    atomic_int processed_images;
    atomic_init(&processed_images, 0);

    #pragma omp parallel for schedule(static)
    for (int i = start; i < start + count; i++) {
        char* path = image_list[i];
        char* filename = strrchr(path, '/');
        if (!filename) continue;
        filename++;

        char outname[256];

        snprintf(outname, 256, "%s/%s", output_dir_gray, filename);
        gray_scale(path, outname);

        snprintf(outname, 256, "%s/%s", output_dir_horizontalgray, filename);
        mirror_horizontal_gray(path, outname);

        snprintf(outname, 256, "%s/%s", output_dir_horizontalcolor, filename);
        mirror_horizontal_color(path, outname);

        snprintf(outname, 256, "%s/%s", output_dir_verticalgray, filename);
        mirror_vertical_gray(path, outname);

        snprintf(outname, 256, "%s/%s", output_dir_verticalcolor, filename);
        mirror_vertical_color(path, outname);

        snprintf(outname, 256, "%s/%s", output_dir_blur, filename);
        blur_image_color(path, outname, blur_ratio);

        atomic_fetch_add(&processed_images, 1);
    }

    // Solo el proceso 0 imprime el progreso periódicamente
    if (rank == 0) {
        int last_reported = 0;
        while (last_reported < total_images) {
            int current = atomic_load(&processed_images);
            int porcentaje = (current * 100) / total_images;
            if (porcentaje > last_reported) {
                printf("PROGRESS %d\n", porcentaje);
                fflush(stdout);
                last_reported = porcentaje;
            }
            usleep(500000); // espera 0.5 segundos
        }
    }

    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;
    double max_elapsed;

    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    char report_name[64];
    sprintf(report_name, "report_proc%d.txt", rank);
    FILE *report = fopen(report_name, "w");
    fprintf(report, "Proceso %d en host %s completó en %.3lf segundos\n", rank, hostname, elapsed);
    fclose(report);

    if (rank == 0) {
        printf("\nTiempo total distribuido: %.3lf segundos\n", max_elapsed);
    }

    MPI_Finalize();
    return 0;
}