#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Game::Game() {
    isRunning = false;
    std::cout << "Creating game object" <<std::endl;
}
Game::~Game() {
    std::cout << "Destroying game" << std::endl;
}
void Game::Initialize() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
        std::cerr << "Error initializing SDL " << std::endl;
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    windowWidth = 800;
    windowHeight = 600;

    window = SDL_CreateWindow(NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
        );
    if (!window) {
        std::cerr << "Error creating SDL window " << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window,-1,0);

    if(!renderer) {
        std::cerr << "Error creating renderer " << std::endl;
        return;
    }

    SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

    isRunning = true;
}
void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while(SDL_PollEvent(&sdlEvent)) {
        switch( sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                break;

        }
    }

}

void Game::Setup() {
    // initialize game objects

}
void Game::Update(){}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer,21,21,21,255);
    SDL_RenderClear(renderer);

    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {10,10,32,32 };

    SDL_RenderCopy(renderer,texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void Game::Run(){
    while(isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}