#include "helpers.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

volatile sig_atomic_t terminal_resized = 0;

void handle_sigwinch(int sig)
{
    (void)sig;
    terminal_resized = 1;
}

tsize get_terminal_size(void)
{
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    tsize size;
    size.rows = w.ws_row;
    size.columns = w.ws_col;
    return size;
}
image load_image (const char *image_file)
{
    int width, height, channels;

    unsigned char *loaded_data = stbi_load(
        image_file, 
        &width, 
        &height, 
        &channels, 
        0
    );

    if (loaded_data == NULL)
    {
        fprintf(stderr, "Error: %s\n", stbi_failure_reason());
        return (image) {0}; // If cant load image return empty image 
    }

    // Convert data to double in range 0 to 1

    size_t total_size = (size_t) width * height *channels;

    // Allocate memory for data
    double *data = calloc(total_size, sizeof(*data));

    if (data == NULL)
    {
        fprintf (stderr, ("Error: Failed to allocate memory for data!\n"));
        stbi_image_free(loaded_data);
        return (image) {0}; // Return empty image 
    }

    for (size_t i = 0; i < total_size; i++)
    {
        data[i] = loaded_data[i] / 255.0; // Divide by 255.0 because max value of 8-bit color is 255 
    }
    
    stbi_image_free(loaded_data);

    return (image) 
    {
        .width = (size_t) width,
        .height = (size_t) height,
        .channels = (size_t) channels,
        .data = data

    };
}

void free_image(image *image)
{
    if (image && image->data)
    {
        free(image->data);
        image->data = NULL;
        image->width = image->height = image->channels = 0;
    }
    
}
