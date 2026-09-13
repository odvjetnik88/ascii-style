#ifndef HELPERS_H
#define HELPERS_H
#include <math.h>
#include "../include/stb_image.h"
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define CHAR_RATIO 2.0
#define PRINT_CHARS " .-=+*x#$&X@"
#define N_CHARS (sizeof(PRINT_CHARS) - 1)
#define M_PI 3.14159265358979323846

extern volatile sig_atomic_t terminal_resized;


typedef struct
{
    int rows;
    int columns;
} tsize;


typedef struct
{
    size_t width;
    size_t height;
    size_t channels;
    double *data;
} image;

// Function Protoypes

void handle_sigwinch(int sig);
tsize get_terminal_size(void);

image load_image (const char *image_file);
void free_image(image *image);
image resize_image(image *original, size_t max_width, size_t max_height, double char_ratio);




#endif