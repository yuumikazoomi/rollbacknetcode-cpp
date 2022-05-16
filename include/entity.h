
#ifndef _entity_h
#define _entity_h
#include <random>
#include <SDL.h>
#include <deque>
#include <iterator>
#include <memory>
#include <vector>
#include <netinterface.h>
enum Direction
{
    kDirectionRight =1,
    kDirectionDown,
    kDirectionLeft,
    kDirectionUp
};
typedef struct SVector2{
    int x;
    int y;
}Vector2;
class Entity
{
private:
    uint32_t score;
    uint16_t direction ;
    Vector2 position ;
    int speed;
public:
    Entity();
    uint32_t getscore();
    void setscore(uint32_t score);
    void incrementscore();
    const Vector2& getposition();
    void update(int time);
    uint16_t getdirection();
    void updatedirection(int directon);
    void forcesetposition(const Vector2& position);
};
#endif
