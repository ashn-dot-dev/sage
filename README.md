Simple Abstract Graphics Environment
====================================

The Simple Abstract Graphics Environment (SAGE) is a minimal software rendering
framework designed for educational purposes. The goal of SAGE is to provide a
straightforward API for drawing pixels onto a screen.

```c
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
    for (int i = 0; i < SCREEN_W * SCREEN_H; ++i) {
        framebuffer.pixels[i] = SAGE_BLACK;
    }

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
```

## Building and Running
Building and running SAGE applications requires the following dependencies:

+ C11 compiler ([gcc](https://gcc.gnu.org/) or [clang](https://clang.llvm.org/))
+ [sunder](https://github.com/ashn-dot-dev/sunder)
+ [raylib](https://github.com/raysan5/raylib) installed via [raylib-sunder](https://github.com/ashn-dot-dev/raylib-sunder) (raylib backend)
+ [emsdk](https://github.com/emscripten-core/emsdk) (web backend)

You can install these dependencies individually, or if you just want to skip
all of that and start writing code, you can run:

```sh
# Download and install dependencies locally (run once)
sh setup-local-environment.sh

# Set up environment for SAGE development (run each session)
. ./env
```

Build a SAGE application from a C or Sunder source file by running the
`sage-build` tool as `sage-build <backend> <source-file>`:

```sh
# Native build using the raylib backend (outputs examples/readme.c.raylib.out)
./sage-build raylib examples/readme.c

# WebAssembly build using the web backend (outputs examples/readme.c.web.html)
./sage-build web examples/readme.c
```

## License
All content in this repository, unless otherwise noted, is licensed under the
Zero-Clause BSD license.

See LICENSE for more information.
