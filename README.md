Simple Abstract Graphics Environment
====================================

The Simple Abstract Graphics Environment (SAGE) is a minimal software rendering
framework designed for educational purposes. The goal of SAGE is to provide a
straightforward API for drawing pixels onto a screen.

SAGE is provided as a C library with [raylib](https://www.raylib.com/) and web
backend implementations, a [Sunder](https://github.com/ashn-dot-dev/sunder)
library binding to the C implementations, and a JavaScript library for native
web development.

## Example in C

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
```

## Example in HTML/JavaScript

```html
<!-- examples/readme.html -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SAGE Application</title>
    <link rel="stylesheet" type="text/css" href="../sage.css">
</head>
<body>
    <script src="../sage.js"></script>

    <canvas id="main" width="640" height="480"></canvas>
    <script type="text/javascript">
        const canvas = new SAGE.Canvas("#main");

        const tick = () => {
            // Clear the framebuffer.
            canvas.fill(SAGE.BLACK);

            // Write a sparse swirl of dots into the framebuffer.
            for (let y = 0; y < canvas.h; ++y) {
                for (let x = 0; x < canvas.w; ++x) {
                    const hash = (x * 73 + y * 151 + x * y * 37) % 65536;
                    if (hash % 777 < 11) {
                        const r = Math.floor((x / canvas.w) * 0xFF);
                        const b = Math.floor((y / canvas.h) * 0xFF);
                        const color = SAGE.rgb(r, 0x00, b);
                        canvas.setPixel(x, y, color);
                    }
                }
            }

            // Draw the framebuffer onto the screen.
            canvas.present();
        }

        canvas.run(tick);
    </script>
</body>
</html>
```

## Building and Running (C/Sunder)
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
# Native build using the raylib backend
./sage-build raylib examples/simple.c # outputs examples/simple.c.raylib.out
./sage-build raylib examples/simple.sunder # outputs examples/simple.sunder.raylib.out

# WebAssembly build using the web backend
./sage-build web examples/simple.c # outputs examples/simple.c.web.html
./sage-build web examples/simple.sunder # outputs examples/simple.sunder.web.html
```

## License
All content in this repository, unless otherwise noted, is licensed under the
Zero-Clause BSD license.

See LICENSE for more information.
