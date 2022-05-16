#include <entity.h>

Entity::Entity(){
    score = 0;
    speed = 7;
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
    /*
    switch (input){
        case kDirectionRight:
            position.x += speed * time;
            break;
        case kDirectionDown:
            position.y += speed * time;
            break;
        case kDirectionLeft:
            position.x -= speed * time;
            break;
        case kDirectionUp:
            position.y -= speed * time;
            break;
        default:
            break;
    }
     */
}
void Entity::forcesetposition(const Vector2& position){
    this->position = position;
}
