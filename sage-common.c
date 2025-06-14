#include "sage.h"

void
sage_set_pixel(struct sage_image img, int x, int y, struct sage_pixel color)
{
    if (x < 0 || x >= img.w || y < 0 || y >= img.h) {
        return;
    }

    img.pixels[y * img.w + x] = color;
}

struct sage_pixel
sage_get_pixel(struct sage_image img, int x, int y)
{
    if (x < 0 || x >= img.w || y < 0 || y >= img.h) {
        return SAGE_BLACK;
    }

    return img.pixels[y * img.w + x];
}

void
sage_fill(struct sage_image dst, struct sage_pixel color)
{
    for (int i = 0; i < dst.w * dst.h; ++i) {
        dst.pixels[i] = color;
    }
}

void
sage_blit(struct sage_image dst, int dx, int dy, struct sage_image src)
{
    for (int src_y = 0; src_y < src.h; ++src_y) {
        int dst_y = dy + src_y;
        if (dst_y < 0 || dst_y >= dst.h) {
            continue;
        }

        for (int src_x = 0; src_x < src.w; ++src_x) {
            int dst_x = dx + src_x;
            if (dst_x < 0 || dst_x >= dst.w) {
                continue;
            }

            struct sage_pixel src_px = src.pixels[src_y * src.w + src_x];
            dst.pixels[dst_y * dst.w + dst_x] = src_px;
        }
    }
}

void
sage_blend(struct sage_image dst, int dx, int dy, struct sage_image src)
{
    for (int src_y = 0; src_y < src.h; ++src_y) {
        int dst_y = dy + src_y;
        if (dst_y < 0 || dst_y >= dst.h) {
            continue;
        }

        for (int src_x = 0; src_x < src.w; ++src_x) {
            int dst_x = dx + src_x;
            if (dst_x < 0 || dst_x >= dst.w) {
                continue;
            }

            struct sage_pixel src_px = src.pixels[src_y * src.w + src_x];
            struct sage_pixel dst_px = dst.pixels[dst_y * dst.w + dst_x];
            uint8_t inv_alpha = 0xff - src_px.a;
            struct sage_pixel res_px = (struct sage_pixel){
                .r = (src_px.r * src_px.a + dst_px.r * inv_alpha) / 0xff,
                .g = (src_px.g * src_px.a + dst_px.g * inv_alpha) / 0xff,
                .b = (src_px.b * src_px.a + dst_px.b * inv_alpha) / 0xff,
                .a = 0xff,
            };
            dst.pixels[dst_y * dst.w + dst_x] = res_px;
        }
    }
}
