const SAGE = (function() {
    "use strict";

    const rgba = (r, g, b, a = 0xff) => {
        const rBits = ((r & 0xff) << 0x00);
        const gBits = ((g & 0xff) << 0x08);
        const bBits = ((b & 0xff) << 0x10);
        const aBits = ((a & 0xff) << 0x18);
        return rBits | gBits | bBits | aBits;
    };

    const rgb = (r, g, b) => rgba(r, g, b, 0xff);

    const r = (pixel) => (pixel >> 0x00) & 0xff;
    const g = (pixel) => (pixel >> 0x08) & 0xff;
    const b = (pixel) => (pixel >> 0x10) & 0xff;
    const a = (pixel) => (pixel >> 0x18) & 0xff;

    const BLACK = rgb(0x00, 0x00, 0x00);
    const WHITE = rgb(0xff, 0xff, 0xff);

    class Pixel {
        #r;
        #g;
        #b;
        #a;

        constructor(r, g, b, a = 0xff) {
            this.#r = r & 0xff;
            this.#g = g & 0xff;
            this.#b = b & 0xff;
            this.#a = a & 0xff;
        }

        get rgba() {
            const rBits = this.#r << 0x00;
            const gBits = this.#g << 0x08;
            const bBits = this.#b << 0x10;
            const aBits = this.#a << 0x18;
            return rBits | gBits | bBits | aBits;
        }

        get r() {
            return this.#r;
        }

        get g() {
            return this.#g;
        }

        get b() {
            return this.#b;
        }

        get a() {
            return this.#a;
        }
    }

    class Image {
        #w;
        #h;
        #pixels;

        constructor(w, h, pixels = null) {
            this.#w = w;
            this.#h = h;
            this.#pixels = pixels ?? new Uint32Array(w * h);
        }

        get w() {
            return this.#w;
        }

        get h() {
            return this.#h;
        }

        get pixels() {
            return this.#pixels;
        }

        setPixelRaw(x, y, rgba) {
            if (x < 0 || x >= this.w || y < 0 || y >= this.h) {
                return;
            }
            this.pixels[y * this.w + x] = rgba;
        }

        setPixel(x, y, color) {
            const rgba = color instanceof Pixel ? color.rgba : color;
            this.setPixelRaw(x, y, rgba);
        }

        getPixelRaw(x, y) {
            if (x < 0 || x >= this.w || y < 0 || y >= this.h) {
                return BLACK;
            }
            return this.pixels[y * this.w + x];
        }

        getPixel(x, y) {
            const rgba = this.getPixelRaw(x, y);
            const r = (rgba >> 0x00) & 0xff;
            const g = (rgba >> 0x08) & 0xff;
            const b = (rgba >> 0x10) & 0xff;
            const a = (rgba >> 0x18) & 0xff;
            return new Pixel(r, g, b, a);
        }

        fill(color = BLACK) {
            const pixel = color instanceof Pixel ? color.rgba : color;
            this.pixels.fill(pixel);
        }

        blit(dx, dy, src) {
            for (let srcY = 0; srcY < src.h; ++srcY) {
                const dstY = dy + srcY;
                if (dstY < 0 || dstY >= this.h) {
                    continue;
                }
                for (let srcX = 0; srcX < src.w; ++srcX) {
                    const dstX = dx + srcX;
                    if (dstX < 0 || dstX >= this.w) {
                        continue;
                    }
                    this.setPixelRaw(dstX, dstY, src.getPixelRaw(srcX, srcY));
                }
            }
        }

        blend(dx, dy, src) {
            for (let srcY = 0; srcY < src.h; ++srcY) {
                const dstY = dy + srcY;
                if (dstY < 0 || dstY >= this.h) {
                    continue;
                }

                for (let srcX = 0; srcX < src.w; ++srcX) {
                    const dstX = dx + srcX;
                    if (dstX < 0 || dstX >= this.w) {
                        continue;
                    }

                    const srcRgba = src.getPixelRaw(srcX, srcY);
                    const dstRgba = this.getPixelRaw(dstX, dstY);
                    const srcA = a(srcRgba);
                    const invA = 0xff - srcA;
                    const resRgba = rgb(
                        (r(srcRgba) * srcA + r(dstRgba) * invA) / 0xff,
                        (g(srcRgba) * srcA + g(dstRgba) * invA) / 0xff,
                        (b(srcRgba) * srcA + b(dstRgba) * invA) / 0xff);
                    this.setPixelRaw(dstX, dstY, resRgba);
                }
            }
        }
    }

    // Create a new canvas object with the provided width and height:
    //
    //      new Canvas(w, h)
    //
    // Create a new canvas object from an existing canvas element:
    //
    //      new Canvas(element)
    //      new Canvas("css-selector")
    class Canvas {
        #init
        #tick
        #element
        #framebuffer
        #context2d
        #imageData

        constructor(w, h = null) {
            this.#init = performance.now();
            this.#tick = null;

            if (typeof w === "number" && typeof h === "number") {
                this.#element = document.createElement("canvas");
                this.#element.width = w;
                this.#element.height = h;
                this.#element.style.border = "1px solid black";
                this.#element.style.imageRendering = "pixelated";
            }
            else if (w && w.tagName === "CANVAS") {
                this.#element = w;
            }
            else if (typeof w === "string") {
                const selector = w;
                this.#element = document.querySelector(selector);
                if (!this.#element) {
                    throw new Error(`Canvas element not found: ${selector}`);
                }
                if (this.#element.tagName !== "CANVAS") {
                    throw new Error(`Element is not a canvas: ${selector}`);
                }
            }
            else {
                throw new Error("Canvas constructor expects (width, height) or (element/selector)");
            }

            this.#framebuffer = new Image(this.w, this.h);
            this.#context2d = this.#element.getContext("2d");
            this.#imageData = this.#context2d.createImageData(this.w, this.h);
        }

        get w() {
            return this.#element.width;
        }

        get h() {
            return this.#element.height;
        }

        get framebuffer() {
            return this.#framebuffer;
        }

        now() {
            const MSEC_PER_SEC = 1000;
            return (performance.now() - this.#init) / MSEC_PER_SEC;
        }

        setPixel(x, y, color) {
            this.framebuffer.setPixel(x, y, color);
        }

        getPixel(x, y) {
            return this.framebuffer.getPixel(x, y);
        }

        fill(color = BLACK) {
            this.framebuffer.fill(color);
        }

        blit(dx, dy, src) {
            this.framebuffer.blit(dx, dy, src);
        }

        blend(dx, dy, src) {
            this.framebuffer.blend(dx, dy, src);
        }

        present() {
            const pixels = this.framebuffer.pixels;
            const data = this.#imageData.data;

            for (let i = 0; i < pixels.length; ++i) {
                const pixel = pixels[i];
                const base = i * 4;
                data[base + 0] = r(pixel);
                data[base + 1] = g(pixel);
                data[base + 2] = b(pixel);
                data[base + 3] = a(pixel);
            }

            this.#context2d.putImageData(this.#imageData, 0, 0);
        }

        run(tick) {
            if (!this.#init) {
                return;
            }

            this.#tick = tick;

            const loop = () => {
                if (!this.#init) {
                    return;
                }

                this.#tick();
                requestAnimationFrame(loop);
            };

            requestAnimationFrame(loop);
        }

        stop() {
            this.#init = null;
        }
    }

    return {
        rgba,
        rgb,
        r,
        g,
        b,
        a,
        BLACK,
        WHITE,
        Pixel,
        Image,
        Canvas,
    };
})();

if (typeof window !== "undefined") {
    window.SAGE = SAGE;
}
