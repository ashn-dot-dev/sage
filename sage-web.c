#include <stdbool.h>

#include <emscripten.h>
#include <emscripten/html5.h>

#include "sage.h"
#include "sage-common.c"

static double
sage_now_raw(void);

static struct {
    bool is_init;
    double init_now;
    struct sage_image framebuffer;
} sage_state = {0};

EM_JS(void, js_create_canvas, (int w, int h), {
    const canvas = document.createElement("canvas");
    canvas.id = "sage-canvas";
    canvas.width = w;
    canvas.height = h;
    canvas.style.border = "1px solid black";
    canvas.style.imageRendering = "pixelated";

    const existing = document.getElementById("sage-canvas");
    if (existing) {
        existing.remove();
    }

    document.body.appendChild(canvas);
})

EM_JS(void, js_update_canvas, (void* pixels, int w, int h), {
    const canvas = document.getElementById("sage-canvas");
    if (!canvas) {
        return;
    }

    const ctx = canvas.getContext("2d");
    if (!ctx) {
        return;
    }

    const imageData = ctx.createImageData(w, h);

    // ImageData pixel data is a Uint8ClampedArray in RGBA format. SAGE image
    // pixel data is already in RGBA format, so no conversion is necessary.
    const SIZEOF_PIXEL = 4; /* struct { u8 r; u8 g; u8 b; u8 a; } */
    const pixelArray = new Uint8ClampedArray(
        Module.HEAPU8.buffer, pixels, w * h * SIZEOF_PIXEL);
    imageData.data.set(pixelArray);

    ctx.putImageData(imageData, 0, 0);
})

void
sage_init(struct sage_image framebuffer)
{
    if (sage_state.is_init) {
        return;
    }

    sage_state.framebuffer = framebuffer;

    js_create_canvas(framebuffer.w, framebuffer.h);

    sage_state.is_init = true;
    sage_state.init_now = sage_now_raw();
}

void
sage_fini(void)
{
    if (!sage_state.is_init) {
        return;
    }

    sage_state.is_init = false;
}

void
sage_run(void (*tick)(void))
{
    static int const FPS = 0; /* requestAnimationFrame timing */
    static bool const SIMULATE_INFINITE_LOOP = true;
    emscripten_set_main_loop(tick, FPS, SIMULATE_INFINITE_LOOP);
}

static double
sage_now_raw(void)
{
    static long const MSEC_PER_SEC = 1000;
    return emscripten_get_now() / (double)MSEC_PER_SEC;
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

    js_update_canvas(
        sage_state.framebuffer.pixels,
        sage_state.framebuffer.w,
        sage_state.framebuffer.h);
}
