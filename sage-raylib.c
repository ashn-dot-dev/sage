#define _POSIX_C_SOURCE 199309L /* clock_gettime */
#include <stdbool.h>
#include <time.h> /* clock_gettime */

#include <raylib.h>

#include "sage.h"
#include "sage-common.c"

static double
sage_now_raw(void);

static struct {
    bool is_init;
    double init_now;
    struct sage_image framebuffer;
    Texture2D framebuffer_texture;
} sage_state = {0};

void
sage_init(struct sage_image framebuffer)
{
    if (sage_state.is_init) {
        return;
    }

    InitWindow(framebuffer.w, framebuffer.h, "SAGE Application");
    SetTargetFPS(60);

    sage_state.framebuffer = framebuffer;
    sage_state.framebuffer_texture = LoadTextureFromImage((Image){
        .data = framebuffer.pixels,
        .width = framebuffer.w,
        .height = framebuffer.h,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    });

    sage_state.is_init = true;
    sage_state.init_now = sage_now_raw();
}

void
sage_fini(void)
{
    if (!sage_state.is_init) {
        return;
    }

    UnloadTexture(sage_state.framebuffer_texture);
    CloseWindow();
    sage_state.is_init = false;
}

void
sage_run(void (*tick)(void))
{
    while (!WindowShouldClose()) {
        tick();
    }
}

static double
sage_now_raw(void)
{
    struct timespec ts = {0};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    static long const NSEC_PER_SEC = 1000000000;
    return (double)ts.tv_sec + (double)ts.tv_nsec / (double)NSEC_PER_SEC;
}

double
sage_now(void)
{
    return sage_now_raw() - sage_state.init_now;
}

void
sage_present(void)
{
    if (!sage_state.is_init) {
        return;
    }

    if (WindowShouldClose()) {
        return;
    }

    UpdateTexture(
        sage_state.framebuffer_texture, sage_state.framebuffer.pixels);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        sage_state.framebuffer_texture,
        (Rectangle){0, 0, sage_state.framebuffer.w, sage_state.framebuffer.h},
        (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);
    EndDrawing();
}
