#pragma once
static constexpr auto WORLD_ROWS = 16;
static constexpr auto WORLD_COLUMNS = WORLD_ROWS;
static constexpr auto WORLD_SIZE = (WORLD_ROWS * Cfg::CELL_SIZE); //width and height of the gameworld    
static constexpr auto FIRST_VALID_CELL = 1; //to shortcut collision testing we can check against the map boundary walls. Helps with mouse interaction and for Q&D respawn when stuck in a wall.
static constexpr auto LAST_VALID_CELL = WORLD_ROWS-2; //0 == wall, 15 == wall. so if our position is below or above the valid spaces, we can bail without performing lookup.
#define USE_BITMAP_LEVELDATA
#ifndef USE_BITMAP_LEVELDATA
static constexpr char WORLD[WORLD_ROWS][WORLD_COLUMNS] PROGMEM = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,1,1,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,1,1,1,1,1,1,1,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
inline constexpr bool isWall(int x, int y) noexcept {
    if (x < FIRST_VALID_CELL || y < FIRST_VALID_CELL
        || x > LAST_VALID_CELL || y > LAST_VALID_CELL) {
        return true;
    }
    return (WORLD[y][x] != 0);
}
#else //use bitmap representation for the map data. 
//this saves memory but might give up performance, as the Arduboy does not feature a barrel shifter.
// the expression ">> ((WORLD_COLUMNS - 1) - x)", therefore, could cost up to 15 cycles.
// ergo: ... need to figure out how to run perf tests on the Arduino. 
//the bit map was generated thusly: https://pastebin.com/fweGUGJE
static constexpr uint16_t  WORLD[WORLD_ROWS] PROGMEM = { 
    /*[ 0] 1111111111111111*/       0xffff,
    /*[ 1] 1000000000000001*/       0x8001,
    /*[ 2] 1001111100000001*/       0x9f01,
    /*[ 3] 1001000101010101*/       0x9155,
    /*[ 4] 1001000100000001*/       0x9101,
    /*[ 5] 1001001100000001*/       0x9301,
    /*[ 6] 1000000000000001*/       0x8001,
    /*[ 7] 1000000000000001*/       0x8001,
    /*[ 8] 1000000000000001*/       0x8001,
    /*[ 9] 1001100111111001*/       0x99f9,
    /*[10] 1001000000001001*/       0x9009,
    /*[11] 1001110000001001*/       0x9c09,
    /*[12] 1001000000001001*/       0x9009,
    /*[13] 1001111111101001*/       0x9fe9,
    /*[14] 1000000000000001*/       0x8001,
    /*[15] 1111111111111111*/       0xffff,
};
inline constexpr bool isWall(int x, int y) noexcept {
    if (x < FIRST_VALID_CELL || y < FIRST_VALID_CELL
        || x > LAST_VALID_CELL || y > LAST_VALID_CELL) {
        return true;
    }
    return (WORLD[y] >> ((WORLD_COLUMNS - 1) - x) & 0x01); 
}
#endif // !USE_BITMAP_LEVELDATA

static_assert(WORLD_ROWS == WORLD_COLUMNS && "The map must be square - WORLD_ROWS == WORLD_COLUMNS");
static_assert(Utils::isPowerOfTwo(WORLD_SIZE) && "World width and height must be a power-of-2");
static_assert(!isWall(FIRST_VALID_CELL, FIRST_VALID_CELL));
static_assert(!isWall(LAST_VALID_CELL, LAST_VALID_CELL));
static_assert(isWall(3, 3)); //test an arbitrary position that we know should be a wall 
