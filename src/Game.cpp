#include "Game.h"
#include <iostream>

Game::Game() {
    std::cout << "Creating game object" <<std::endl;
}
Game::~Game() {
    std::cout << "Destroying game" << std::endl;
}
void Game::Initialize(){}
void Game::Run(){}
void Game::ProcessInput(){}
void Game::Update(){}
void Game::Render(){}
void Game::Destroy(){}