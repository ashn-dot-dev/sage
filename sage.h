#ifndef SAGE_H
#define SAGE_H

#include <stdint.h>

struct sage_pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct sage_image {
    int w;
    int h;
    struct sage_pixel* pixels;
};

void
sage_set_pixel(struct sage_image img, int x, int y, struct sage_pixel color);
struct sage_pixel
sage_get_pixel(struct sage_image img, int x, int y);

void
sage_fill(struct sage_image dst, struct sage_pixel color);
void
sage_blit(struct sage_image dst, int dx, int dy, struct sage_image src);
void
sage_blend(struct sage_image dst, int dx, int dy, struct sage_image src);

void
sage_init(struct sage_image framebuffer);
void
sage_fini(void);

void
sage_run(void (*tick)(void));

double
sage_now(void);

void
sage_present(void);

#define SAGE_RGBA(r, g, b, a) ((struct sage_pixel){(r), (g), (b), (a)})
#define SAGE_RGB(r, g, b) ((struct sage_pixel){(r), (g), (b), 0xff})

#define SAGE_BLACK SAGE_RGB(0x00, 0x00, 0x00)
#define SAGE_WHITE SAGE_RGB(0xff, 0xff, 0xff)

#endif /* SAGE_H */
