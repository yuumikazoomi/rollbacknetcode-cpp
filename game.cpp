
#include <game.h>
Game::Game(bool host) : state(host),level(&state){
    this->host = host;
    if(host){
        if(!net.makesocketbind()){
            //maybe show an error
        }
    }else{
        if(!net.makesocket()){
            net.setremoteaddress("0.0.0.0",6789);
            //maybe show an error
        }
    }
    
}
void Game::update(){
    //check input
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                //quit
                break;
        }
    }
    
    //check network
    auto netcallback = [](const NIRelayPacket& packet,NITransferSize size,bool& availabledata,bool& error){
        if(availabledata){
            printf("we got data!\n");
        }else if(error){
            printf("we got an error!\n");
        }
    };
    net.poll(netcallback);
    
    //update
    state.update();
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
