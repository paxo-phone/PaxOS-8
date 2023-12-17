#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "../CApp.hpp"

class Snake : public CApp
{
public:
    [[nodiscard]] const char* getAppName() const override { return "Snake"; }
    void onApplicationStart() override;
    
    private:
    bool gameover = false;
    int level;
    int speed;
    uint64_t snakeTimer;
    uint8_t snake[1201][3];
    uint8_t direction;
    uint8_t direction_;
    uint8_t interprete;
    uint16_t pointColor;
    uint8_t pointxy[2];       // init the point at randoms points
    uint16_t snake_color;
    uint16_t background_color;
    
    bool start(); // Returns a boolean indicating whether to restart the game or not.
    bool gameOver(); // Returns a boolean indicating whether to restart the game or not.
};

#endif
