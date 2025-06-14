/* examples/readme.c */
#include <sage.h>

#define SCREEN_W 640
#define SCREEN_H 480

static struct sage_pixel framebuffer_pixels[SCREEN_W * SCREEN_H] = {0};
static struct sage_image framebuffer = {SCREEN_W, SCREEN_H, framebuffer_pixels};

void
tick(void)
{
    // Clear the framebuffer.
    sage_fill(framebuffer, SAGE_BLACK);

    // Write a sparse swirl of dots into the framebuffer.
    for (int y = 0; y < SCREEN_H; ++y) {
        for (int x = 0; x < SCREEN_W; ++x) {
            int hash = (x * 73 + y * 151 + x * y * 37) % 65536;
            if (hash % 777 < 11) {
                uint8_t r = ((double)x / (double)SCREEN_W) * 0xFF;
                uint8_t b = ((double)y / (double)SCREEN_H) * 0xFF;
                struct sage_pixel color = SAGE_RGB(r, 0x00, b);
                sage_set_pixel(framebuffer, x, y, color);
            }
        }
    }

    // Draw the framebuffer onto the screen.
    sage_present();
}

int
main(void)
{
    // Platform-specific initialization.
    sage_init(framebuffer);

    // Repeatedly run `tick` until a platform-specific signal to quit the
    // application is received.
    sage_run(tick);

    // Platform-specific finalization.
    sage_fini();
}
