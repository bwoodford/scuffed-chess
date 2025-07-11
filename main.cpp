#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Simple SDL 3 Window",
        800, 600,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    bool running = true;
    SDL_Event event;

    // x, y, w, h
    SDL_FRect rect = {350, 250, 100, 100};

    int accel = 20;
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        running = false;
                    }

                    if (event.key.key == SDLK_UP) {
                        rect.y -= accel;
                    }
                    if (event.key.key == SDLK_DOWN) {
                        rect.y += accel;
                    }
                    if (event.key.key == SDLK_LEFT) {
                        rect.x -= accel;
                    }

                    if (event.key.key == SDLK_RIGHT) {
                        rect.x += accel;
                    }

                    break;
                default:
                    break;
            }
        }

        // Clear screen with a nice blue color
        SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255);
        SDL_RenderClear(renderer);

        // Draw a simple white rectangle in the center
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Small delay to prevent excessive CPU usage
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
