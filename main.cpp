#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    SDL_FRect dest;
    SDL_Color color;
} ColoredRect;

const int BOARD_SQUARES = 64;

typedef struct {
    ColoredRect squares[BOARD_SQUARES];
    int width;
    int height;
} ChessBoard;

ChessBoard board;

SDL_Texture* createTexture(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    if (!texture) return NULL;

    // Set texture as render target and fill with white
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    // Enable color modulation
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    return texture;
}

void initBoard() {

    int r = 255, g = 255, b = 255, a = 255;

    int squareWidth = 100, squareHeight = 100;

    int row = 0;

    for (int i = 0; i < BOARD_SQUARES; i++) {

        row = i/8;

        // Alternate colors between columns AND rows
        if (((i + row) % 2) != 0) {
            r = 0;
            g = 0;
            b = 0;
            a = 255;
        } else {
            r = 255;
            g = 255;
            b = 255;
            a = 255;
        }

        board.squares[i].dest.x = squareWidth * (i % 8);
        board.squares[i].dest.y = squareHeight * row;
        board.squares[i].dest.w = squareWidth;
        board.squares[i].dest.h = squareHeight;
        board.squares[i].color.r = r;
        board.squares[i].color.g = g;
        board.squares[i].color.b = b;
        board.squares[i].color.a = a;
    }

    board.width = 800;
    board.height = 800;

}

int main(int argc, char* argv[]) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    int windowWidth = 800, windowHeight = 600;

    SDL_Window* window = SDL_CreateWindow(
        "Simple SDL 3 Window",
        windowWidth, windowHeight,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = createTexture(renderer);
    if (!texture) {
        printf("Failed to create white texture\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool gaming = true;
    SDL_Event event;

    Uint64 last_time = SDL_GetTicks();
    int frame_count = 0;

    initBoard();

    while (gaming) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_WINDOW_RESIZED:
                    int width, height;
                    SDL_GetWindowSize(window, &width, &height);
                    break;
                case SDL_EVENT_QUIT:
                    gaming = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        gaming = false;
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < BOARD_SQUARES; i++) {
            SDL_SetTextureColorMod(texture, board.squares[i].color.r, board.squares[i].color.g, board.squares[i].color.b);
            SDL_SetTextureAlphaMod(texture, board.squares[i].color.a);
            SDL_RenderTexture(renderer, texture, NULL, &board.squares[i].dest);
        }

        SDL_RenderPresent(renderer);

        // FPS counter
        frame_count++;
        Uint64 current_time = SDL_GetTicks();
        if (current_time - last_time >= 1000) {
            printf("FPS: %d\n", frame_count);
            frame_count = 0;
            last_time = current_time;
        }

        // Small delay to prevent excessive CPU usage
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
