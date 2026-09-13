#include "helpers.h"

int main (int argc, char *argv[])
{
    
    if (argc != 2)
    {
        printf("Usage ./ascii [PATH to image]");
        return 1;
    }

    image original;

    char *image_file = argv[1];

    original.data = stbi_load(
        image_file, 
        &original.width, 
        &original.height, 
        &original.channels, 
        3
    );

    if (original.data == NULL)
    {
        fprintf(stderr, "Error: %s\n", stbi_failure_reason());
        stbi_image_free(original.data);
        return 2;
    }

    signal(SIGWINCH, handle_sigwinch);

    tsize terminal = get_terminal_size();
    
    while (1)
    {

        // Main logic here
        if(terminal_resized)
        {
            terminal = get_terminal_size();
            terminal_resized = 0;
        }
    printf ("rows %d\n", terminal.rows);
    printf ("columns %d\n", terminal.columns);

        pause();
    }

    return 0;
}