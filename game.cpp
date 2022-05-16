
#include <game.h>
Game::Game(bool host) : state(host),level(&state){
    this->host = host;
    if(!net.makesocketbind()){
    }
}
void Game::update(){
    state.update();
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
