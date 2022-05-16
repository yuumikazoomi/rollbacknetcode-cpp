
#ifndef _entity_h
#define _entity_h
#include <random>
#include <SDL.h>
#include <deque>
#include <iterator>
#include <memory>
#include <vector>
#include <netinterface.h>
enum Direction{
    kInputNone = 0,
    kInputRight = 1,
    kInputDown,
    kInputLeft,
    kInputUp
};
typedef struct SVector2{
    int x;
    int y;
}Vector2;
class Entity{
private:
    uint32_t score;
    Vector2 position ;
    int speed;
public:
    Entity();
    uint32_t getscore();
    void setscore(uint32_t score);
    void incrementscore();
    const Vector2& getposition();
    void update(uint16_t input, int time);
    void forcesetposition(const Vector2& position);
};
#endif
