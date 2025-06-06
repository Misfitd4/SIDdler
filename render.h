#ifndef RENDER_H
#define RENDER_H

#include <SDL.h>
#include <stdbool.h>

bool load_charset(const char* path, SDL_Renderer* renderer);
void render_char(SDL_Renderer* renderer, char c, int x, int y, SDL_Color fg);
void render_string(SDL_Renderer* renderer, const char* str, int x, int y, SDL_Color fg);
void render_text(SDL_Renderer* renderer, const char* text, int x, int y);
void draw_text(SDL_Renderer* renderer, int x, int y, const char* text);

void render_log(const char *fmt, ...);
void render_log_flush(SDL_Renderer *renderer);
void render_log_border(SDL_Renderer *renderer, int x, int y, int w, int h);

#endif
