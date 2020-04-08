#pragma once
#include <Arduboy2.h>
struct Graphics{
    Arduboy2& _a;
    uint8_t color = WHITE;
    Graphics(Arduboy2& a) : _a(a) {};
    
    void clearScreen() noexcept {
        _a.clear();
    }
    void present() noexcept {
        _a.display();
    }
    void setColor(uint8_t c) noexcept {
       color = c; //arduboy is monochrome, so BLACK and WHITE are all we get. :)
    }
    void drawVerticalLine(int x, int y, int length) noexcept{
        _a.drawFastVLine(x, y, length, color);   
    }    
};
