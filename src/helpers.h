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

extern volatile sig_atomic_t running;



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
void handle_sigint(int sig);

tsize get_terminal_size(void);

image load_image (const char *image_file);

void free_image(image *image);

double *get_pixel(image *image, size_t x, size_t y);
void set_pixel(image* image, size_t x, size_t y, const double* new_pixel);

void average_pixels(image *image, double *average, size_t x1, size_t x2, size_t y1, size_t y2);

image resize_image(image *original, size_t max_width, size_t max_height, double char_ratio);
image convert_to_grayscale(image *original);

char get_ascii_char(double grayscale);

void print_ascii_image(image *img);






#endif