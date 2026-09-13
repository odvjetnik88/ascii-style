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