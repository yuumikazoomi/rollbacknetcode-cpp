
#include <game.h>
Game::Game(bool host) : state(host),level(&state){
    this->host = host;
    if(host){
        if(!net.makesocketbind()){
            //maybe show an error
        }
    }else{
        if(!net.makesocket()){
            //maybe show an error
        }
    }
    
}
void Game::update(){
    auto netcallback = [](const NIRelayPacket& packet,NITransferSize size,bool& availabledata,bool& error){
        if(availabledata){
            printf("we got data!\n");
        }else if(error){
            printf("we got an error!\n");
        }
    };
    net.poll(netcallback);
    
    
    state.update();
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
