#include "helpers.h"

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage ./ascii [PATH to image]");
        return 1;
    }
    
    const char *image_file = argv[1];

    image original = load_image(image_file);
    if(!original.data)
    {
        free_image(&original);
        fprintf(stderr, "Error: Failed to load image!\n");
        return 1;
    }

    signal(SIGWINCH, handle_sigwinch);

    tsize terminal = get_terminal_size();

    size_t max_height = terminal.rows;
    size_t max_width = terminal.columns;

    while (1)
    {

        // Main logic here
        if(terminal_resized)
        {
            terminal = get_terminal_size();
            terminal_resized = 0;
        }
        max_height = terminal.rows;
        max_width = terminal.columns;
        
        image resized = resize_image(&original, max_width, max_height, CHAR_RATIO);
        print_ascii_image(&resized);
        printf("max_width: %zu, max_height: %zu\n", max_width, max_height);
        free_image(&resized);
        pause();
    }
    free_image(&original);
    return 0;
}
