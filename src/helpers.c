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