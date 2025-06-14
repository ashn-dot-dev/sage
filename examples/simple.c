#include <stdio.h>

#include <sage.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define SPRITE_SIZE 64

static struct sage_pixel framebuffer_pixels[SCREEN_W * SCREEN_H] = {0};
static struct sage_image framebuffer = {SCREEN_W, SCREEN_H, framebuffer_pixels};
static struct sage_pixel sprite_pixels[SPRITE_SIZE * SPRITE_SIZE] = {0};
static struct sage_image sprite = {SPRITE_SIZE, SPRITE_SIZE, sprite_pixels};

static int frame = 0;
static int position_x = 0;
static int position_y = SCREEN_H / 2 - SPRITE_SIZE / 2;
static int velocity_x = +2; /* pixels per tick */
static int velocity_y = -2; /* pixels per tick */

void
tick(void)
{
    double t0 = sage_now();

    // Clear framebuffer
    sage_fill(framebuffer, SAGE_BLACK);

    // Pixel rain
    for (int i = 0; i < SCREEN_W * SCREEN_H; ++i) {
        int x = (frame + i * 7) % SCREEN_W;
        int y = (frame + i * 11) % SCREEN_H;
        struct sage_pixel color = SAGE_RGB(
            (uint8_t)((frame % 0xff) + 0x00 + (i * 2)),
            (uint8_t)((frame % 0xff) + 0xff - (i * 2)),
            (uint8_t)(frame % 0xff));
        sage_set_pixel(framebuffer, x, y, color);
    }

    // Bouncing square sprite
    position_x += velocity_x;
    position_y += velocity_y;
    if (position_x <= 0 || position_x >= SCREEN_W - SPRITE_SIZE) {
        velocity_x *= -1;
    }
    if (position_y <= 0 || position_y >= SCREEN_H - SPRITE_SIZE) {
        velocity_y *= -1;
    }
    for (int y = 0; y < SPRITE_SIZE; ++y) {
        for (int x = 0; x < SPRITE_SIZE; ++x) {
            uint8_t r = (uint8_t)(x * (0xff / SPRITE_SIZE));
            uint8_t g = (uint8_t)(y * (0xff / SPRITE_SIZE));
            uint8_t b = 0x80;
            uint8_t a = 0xA0;
            struct sage_pixel color = SAGE_RGBA(r, g, b, a);
            sage_set_pixel(sprite, x, y, color);
        }
    }
    sage_blend(framebuffer, position_x, position_y, sprite);

    // Border
    for (int x = 0; x < SCREEN_W; ++x) {
        sage_set_pixel(framebuffer, x, 0, SAGE_WHITE);
        sage_set_pixel(framebuffer, x, SCREEN_H - 1, SAGE_WHITE);
    }
    for (int y = 0; y < SCREEN_H; ++y) {
        sage_set_pixel(framebuffer, 0, y, SAGE_WHITE);
        sage_set_pixel(framebuffer, SCREEN_W - 1, y, SAGE_WHITE);
    }

    // End of frame
    sage_present();
    frame += 1;

    double t1 = sage_now();
    printf("frame time: %.6f seconds\n", t1 - t0);
}

int
main(void)
{
    sage_init(framebuffer);
    sage_run(tick);
    sage_fini();
}
