#include <fcntl.h>
#include <unistd.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "render.h"
#include "sid.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int sid_serial_fd = -1;

int selected_octave = 4;
bool running = true;

void handle_keydown(SDL_KeyboardEvent* key) {
    SDL_Scancode sc = key->keysym.scancode;
    switch (sc) {
        case SDL_SCANCODE_ESCAPE: running = false; break;
        case SDL_SCANCODE_Z: play_note(0 + selected_octave * 12); break;
        case SDL_SCANCODE_S: play_note(1 + selected_octave * 12); break;
        case SDL_SCANCODE_X: play_note(2 + selected_octave * 12); break;
        case SDL_SCANCODE_D: play_note(3 + selected_octave * 12); break;
        case SDL_SCANCODE_C: play_note(4 + selected_octave * 12); break;
        case SDL_SCANCODE_V: play_note(5 + selected_octave * 12); break;
        case SDL_SCANCODE_G: play_note(6 + selected_octave * 12); break;
        case SDL_SCANCODE_B: play_note(7 + selected_octave * 12); break;
        case SDL_SCANCODE_H: play_note(8 + selected_octave * 12); break;
        case SDL_SCANCODE_N: play_note(9 + selected_octave * 12); break;
        case SDL_SCANCODE_J: play_note(10 + selected_octave * 12); break;
        case SDL_SCANCODE_M: play_note(11 + selected_octave * 12); break;
        case SDL_SCANCODE_COMMA: play_note(12 + selected_octave * 12); break;
        case SDL_SCANCODE_UP: if (selected_octave < 7) selected_octave++; break;
        case SDL_SCANCODE_DOWN: if (selected_octave > 0) selected_octave--; break;
        default: break;
    }
}

void handle_keyup(SDL_KeyboardEvent* key) {
    SDL_Scancode sc = key->keysym.scancode;
    switch (sc) {
        case SDL_SCANCODE_Z: stop_note(0 + selected_octave * 12); break;
        case SDL_SCANCODE_S: stop_note(1 + selected_octave * 12); break;
        case SDL_SCANCODE_X: stop_note(2 + selected_octave * 12); break;
        case SDL_SCANCODE_D: stop_note(3 + selected_octave * 12); break;
        case SDL_SCANCODE_C: stop_note(4 + selected_octave * 12); break;
        case SDL_SCANCODE_V: stop_note(5 + selected_octave * 12); break;
        case SDL_SCANCODE_G: stop_note(6 + selected_octave * 12); break;
        case SDL_SCANCODE_B: stop_note(7 + selected_octave * 12); break;
        case SDL_SCANCODE_H: stop_note(8 + selected_octave * 12); break;
        case SDL_SCANCODE_N: stop_note(9 + selected_octave * 12); break;
        case SDL_SCANCODE_J: stop_note(10 + selected_octave * 12); break;
        case SDL_SCANCODE_M: stop_note(11 + selected_octave * 12); break;
        case SDL_SCANCODE_COMMA: stop_note(12 + selected_octave * 12); break;
        default: break;
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window* window = SDL_CreateWindow("SIDdler",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  sid_serial_fd = open("/dev/cu.usbmodem6666101", O_WRONLY | O_NOCTTY);
  if (sid_serial_fd < 0) {
    perror("Failed to open SID serial port");
  }

  if (!load_charset("charset.bin", renderer)) {
    fprintf(stderr, "Failed to load charset.bin\n");
    return 1;
  }

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
      else if (event.type == SDL_KEYDOWN) handle_keydown(&event.key);
      else if (event.type == SDL_KEYUP) handle_keyup(&event.key);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_text(renderer, 10, 10, "SIDdler v2 - Press keys to play notes");

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_Quit();
  return 0;
}
