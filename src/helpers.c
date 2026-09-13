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

double* get_pixel(image* image, size_t x, size_t y) {
    return &image->data[(y * image->width + x) * image->channels];
}


// Sets pixel channel values to those of new_pixel
void set_pixel(image* image, size_t x, size_t y, const double* new_pixel) {
    double* pixel = get_pixel(image, x, y);
    for (size_t c = 0; c < image->channels; c++) {
        pixel[c] = new_pixel[c];
    }
}

void average_pixels(image *image, double *average, size_t x1, size_t x2, size_t y1, size_t y2)
    {
        for (size_t c = 0; c < image->channels; c++)
        {
            average[c] = 0.0;
        }

        for (size_t y = y1; y < y2; y++)
        {
            for (size_t x = x1; x < x2; x++)
            {
                double *pixel = get_pixel(image, x, y);
                for (size_t c = 0; c < image->channels; c++)
                {
                    average[c] += pixel[c];
                }
            }
        }

        double n_pixels = (double)(x2 - x1) * (y2 - y1);
        for (size_t c = 0; c < image->channels; c++)
        {
            average[c] /= n_pixels;
        }
    }

image resize_image(image *original, size_t max_width, size_t max_height, double char_ratio)
{
    size_t width, height;
    size_t channels = original->channels;

    size_t new_height = (original->height * max_width) / (original->width * char_ratio);

    if (new_height <= max_height)
    {
        height = new_height;
        width = max_width;
    }
    else
    {
        width = (original->width * max_height * char_ratio) / (original->height);
        height = max_height;
    }
    double *data = calloc(width * height * channels, sizeof(*data));
    
    if (data == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for resized image data!\n");
        return (image) {0}; // Return empty image
    }

    for (size_t j = 0; j < height; j++)
    {
        size_t cord_y1 = (j * original->height) / height;
        size_t cord_y2 = ((j + 1) * original->height) / height;
        for (size_t i = 0; i < width; i++)
        {
            size_t cord_x1 = (i * original->width) / width;
            size_t cord_x2 = ((i + 1) * original->width) / width;

            // Average the pixel values in the corresponding region of the original image
            average_pixels(original, &data[(i + j * width) * channels], cord_x1, cord_x2, cord_y1, cord_y2);

        }
    }

    return (image) 
    {
        .width = width,
        .height = height,
        .channels = channels,
        .data = data
    };
}
