#include "render.h"
#include <stdio.h>
#include <string.h>

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

static SDL_Texture* charset_texture = NULL;

bool load_charset(const char* path, SDL_Renderer* renderer) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;

    uint8_t buffer[2048 * 2];
    if (fread(buffer, 1, sizeof(buffer), f) != sizeof(buffer)) {
        fclose(f);
        return false;
    }
    fclose(f);

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 8, SDL_PIXELFORMAT_INDEX8);
    if (!surface) return false;

    SDL_Color palette[256];
    for (int i = 0; i < 256; ++i)
        palette[i] = (SDL_Color){i, i, i, 255};
    SDL_SetPaletteColors(surface->format->palette, palette, 0, 256);

    memset(surface->pixels, 0, surface->h * surface->pitch);
    for (int c = 0; c < 256; ++c) {
        int offset = (c < 128) ? c * 8 : (2048 + (c - 128) * 8);
        for (int y = 0; y < 8; ++y) {
            uint8_t b = buffer[offset + y];
            for (int x = 0; x < 8; ++x) {
                if (b & (1 << (7 - x)))
                    ((uint8_t*)surface->pixels)[(c / 16) * 8 * surface->pitch + y * surface->pitch + (c % 16) * 8 + x] = 255;
            }
        }
    }

    charset_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return charset_texture != NULL;
}

void render_char(SDL_Renderer* renderer, char c, int x, int y, SDL_Color fg) {
    if (!charset_texture) return;

    uint8_t petscii_index;

    if (c >= 'A' && c <= 'Z') {
        petscii_index = c - '@';  // '@' is first (ASCII 64), so 'A' is index 1, etc.
    } else if (c >= 'a' && c <= 'z') {
        petscii_index = (c - 'a' + 1) + 128;  // start at lowercase PETSCII (offset 2048)
    } else if (c == ' ') {
        petscii_index = 32;
    } else if (c >= '0' && c <= '9') {
        petscii_index = c;
    } else {
        petscii_index = '?';  // fallback for now
    }

    SDL_Rect src = {
        (petscii_index % 16) * 8,
        (petscii_index / 16) * 8,
        8,
        8
    };
    SDL_Rect dst = { x, y, 8, 8 };

    SDL_SetTextureColorMod(charset_texture, fg.r, fg.g, fg.b);
    SDL_RenderCopy(renderer, charset_texture, &src, &dst);
}
void render_string(SDL_Renderer* renderer, const char* str, int x, int y, SDL_Color fg) {
    while (*str) {
        render_char(renderer, *str++, x, y, fg);
        x += 8;
    }
}

void render_text(SDL_Renderer* renderer, const char* text, int x, int y) {
    SDL_Color white = {255, 255, 255, 255};
    render_string(renderer, text, x, y, white);
}

void draw_text(SDL_Renderer* renderer, int x, int y, const char* text) {
    SDL_Color white = {255, 255, 255, 255};
    render_string(renderer, text, x, y, white);
}
