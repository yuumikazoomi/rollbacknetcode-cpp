#include <entity.h>

Entity::Entity(){
    score = 0;
    speed = 1;
    memset(&position,0,sizeof(Vector2));
    mInputPrevious = 0;
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
    if(input == 0){
        input = mInputPrevious;
    }else{
        mInputPrevious = input;
    }
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
