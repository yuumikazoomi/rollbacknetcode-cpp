#include <entity.h>

Entity::Entity(){
    score = 0;
    speed = 10;
    memset(&position,0,sizeof(Vector2));
}
uint32_t Entity::getscore(){
    return score;
}
void Entity::setscore(uint32_t score){
    this->score = score;
}
void Entity::incrementscore(){
    ++this->score;
}
const Vector2 &Entity::getposition(){
    return position;
}
void Entity::update(uint16_t input,int time){
    
    switch (input){
        case kInputRight:
            position.x += speed * time;
            break;
        case kInputDown:
            position.y += speed * time;
            break;
        case kInputLeft:
            position.x -= speed * time;
            break;
        case kInputUp:
            position.y -= speed * time;
            break;
        default:
            break;
    }
     
}
void Entity::forcesetposition(const Vector2& position){
    this->position = position;
}
