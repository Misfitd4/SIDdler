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
bool running = true;

void handle_keydown(SDL_KeyboardEvent* key) {
    switch (key->keysym.scancode) {
        case SDL_SCANCODE_A: play_note(48); break; // C3
        case SDL_SCANCODE_W: play_note(49); break; // C#3
        case SDL_SCANCODE_S: play_note(50); break; // D3
        case SDL_SCANCODE_E: play_note(51); break; // D#3
        case SDL_SCANCODE_D: play_note(52); break; // E3
        case SDL_SCANCODE_F: play_note(53); break; // F3
        case SDL_SCANCODE_T: play_note(54); break; // F#3
        case SDL_SCANCODE_G: play_note(55); break; // G3
        case SDL_SCANCODE_Y: play_note(56); break; // G#3
        case SDL_SCANCODE_H: play_note(57); break; // A3
        case SDL_SCANCODE_U: play_note(58); break; // A#3
        case SDL_SCANCODE_J: play_note(59); break; // B3
        case SDL_SCANCODE_K: play_note(60); break; // C4
        case SDL_SCANCODE_O: play_note(61); break; // C#4
        case SDL_SCANCODE_L: play_note(62); break; // D4
        case SDL_SCANCODE_P: play_note(63); break; // D#4
        case SDL_SCANCODE_SEMICOLON: play_note(64); break; // E4
        default: break;
    }
}

void handle_keyup(SDL_KeyboardEvent* key) {
    switch (key->keysym.scancode) {
        case SDL_SCANCODE_A: stop_note(48); break;
        case SDL_SCANCODE_W: stop_note(49); break;
        case SDL_SCANCODE_S: stop_note(50); break;
        case SDL_SCANCODE_E: stop_note(51); break;
        case SDL_SCANCODE_D: stop_note(52); break;
        case SDL_SCANCODE_F: stop_note(53); break;
        case SDL_SCANCODE_T: stop_note(54); break;
        case SDL_SCANCODE_G: stop_note(55); break;
        case SDL_SCANCODE_Y: stop_note(56); break;
        case SDL_SCANCODE_H: stop_note(57); break;
        case SDL_SCANCODE_U: stop_note(58); break;
        case SDL_SCANCODE_J: stop_note(59); break;
        case SDL_SCANCODE_K: stop_note(60); break;
        case SDL_SCANCODE_O: stop_note(61); break;
        case SDL_SCANCODE_L: stop_note(62); break;
        case SDL_SCANCODE_P: stop_note(63); break;
        case SDL_SCANCODE_SEMICOLON: stop_note(64); break;
        default: break;
    }
}
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window* window = SDL_CreateWindow("SIDdler",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  init_sid();  // ← add this if it’s missing!
  sid_serial_fd = open("/dev/cu.usbmodem6666101", O_WRONLY | O_NOCTTY);
  if (sid_serial_fd < 0) {
  }render_log("ERROR: open() failed: %s", strerror(errno));

  if (!load_charset("charset.bin", renderer)) {
    render_log("ERROR: Failed to load charset.bin");
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

    draw_text(renderer, 10, 10, "SIDdler piano layout active");
    render_log_flush(renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_Quit();
  return 0;
}
