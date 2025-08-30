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

#define MAX_IMAGES 1000
#define INSTR_PER_LOC 20  // 20 instructions per location

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
        perror("Error opening the input directory");
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
        fprintf(stderr, "Use: %s <blur_ratio> <input_dir>\n", argv[0]);
        return 1;
    }

    int blur_ratio = atoi(argv[1]);
    const char* input_dir = argv[2];

    int rank, size;
    char hostname[256];
    char image_list[MAX_IMAGES][256];
    int total_images;

    const char *output_dirs[] = {
        "pictures/result/Gray",
        "pictures/result/HorizontalGray",
        "pictures/result/HorizontalColor",
        "pictures/result/VerticalGray",
        "pictures/result/VerticalColor",
        "pictures/result/Blur"
    };

    FILE *report_file1;
    char data[80] = "report.txt";
    report_file1 = fopen(data, "w");
    if(report_file1 == NULL){
        printf("Null archive");
        exit(1);
    }
    fclose(report_file1);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    gethostname(hostname, sizeof(hostname));

    if (rank == 0) {
        for (int i = 0; i < 6; i++) {
            MKDIR(output_dirs[i]);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&blur_ratio, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("[RANK %d] OpenMP using %d threads on %s\n", rank, omp_get_max_threads(), hostname);
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

    printf("[RANK %d] Host %s processing %d images from index %d\n", rank, hostname, count, start);

    atomic_int processed_images;
    atomic_init(&processed_images, 0);

    int local_reads = 0, local_writes = 0;

    #pragma omp parallel for reduction(+:local_reads, local_writes) schedule(static)
    for (int i = start; i < start + count; i++) {
        char* path = image_list[i];
        char* filename = strrchr(path, '/');
        if (!filename) continue;
        filename++;

        char outname[256];

        snprintf(outname, 256, "%s/%s", output_dirs[0], filename);
        gray_scale(path, outname);

        snprintf(outname, 256, "%s/%s", output_dirs[1], filename);
        mirror_horizontal_gray(path, outname);

        snprintf(outname, 256, "%s/%s", output_dirs[2], filename);
        mirror_horizontal_color(path, outname);

        snprintf(outname, 256, "%s/%s", output_dirs[3], filename);
        mirror_vertical_gray(path, outname);

        snprintf(outname, 256, "%s/%s", output_dirs[4], filename);
        mirror_vertical_color(path, outname);

        snprintf(outname, 256, "%s/%s", output_dirs[5], filename);
        blur_image_color(path, outname, blur_ratio);

        local_reads += 6;
        local_writes += 6;

        atomic_fetch_add(&processed_images, 1);
    }

    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;
    double max_elapsed;

    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    int total_reads, total_writes;
    MPI_Reduce(&local_reads, &total_reads, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_writes, &total_writes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    char report_name[64];
    sprintf(report_name, "report_proc%d.txt", rank);
    FILE *report = fopen(report_name, "w");
    fprintf(report, "Process %d on host %s completed in %.3lf seconds\n", rank, hostname, elapsed);
    fprintf(report, "Locations read: %d\n", local_reads);
    fprintf(report, "Locations written: %d\n", local_writes);
    fclose(report);

    FILE *report_file;
    if (rank == 0) {
        report_file = fopen("report.txt", "r");
        if (!report_file) {
            perror("Error reopening the report file for MIPS calculation");
            return 1;
        }

        long long total_localidades = 0;
        char linea[256];
        while (fgets(linea, sizeof(linea), report_file)) {
            if (strstr(linea, "Locations read") || strstr(linea, "Locations written")) {
                long long valor;
                if (sscanf(linea, "%*[^:]: %lld", &valor) == 1) {
                    total_localidades += valor;
                    if (total_localidades < 0) {
                        fprintf(stderr, "An overflow has occurred in the total locations.\n");
                        return -1;
                    }
                }
            }
        }

        fclose(report_file);

        long long instruccions = total_localidades * 20LL;
        double mips = instruccions / (max_elapsed * 1e6);
        double pixeles_por_seg = (double)(total_images * 6) / max_elapsed;

        printf("\n--- FINAL REPORT ---\n");
        printf("Total distributed time: %.3lf seconds\n", max_elapsed);
        printf("Total locations read: %d\n", total_localidades/2);
        printf("Total locations written: %d\n", total_localidades/2);
        printf("Pixels per second: %.2f\n", pixeles_por_seg);
        printf("Estimated MIPS (20 instructions per location): %.10f\n", mips);

        FILE *final_report = fopen("report_global.txt", "w");
        fprintf(final_report, "Total distributed time: %.3lf seconds\n", max_elapsed);
        fprintf(final_report, "Total locations read: %d\n", total_localidades/2);
        fprintf(final_report, "Total locations written: %d\n", total_localidades/2);
        fprintf(final_report, "Pixels per second: %.2f\n", pixeles_por_seg);
        fprintf(final_report, "Total estimated instructions: %lld\n", instruccions);
        fprintf(final_report, "Estimated MIPS: %.2f\n", mips);
        fclose(final_report);
    }

    MPI_Finalize();
    return 0;
}