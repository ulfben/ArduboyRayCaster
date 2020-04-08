#pragma once
#include "Utils.h"
namespace Cfg {		
	static constexpr auto VIEWPORT_WIDTH = 128;
	static constexpr auto VIEWPORT_HEIGHT = 64;
	static constexpr auto CELL_SIZE = 64; //width and height of a cell in the game world, must be a power of 2.   	
	static constexpr auto FOV_DEGREES = 60; //Field of View, in degrees. We'll need to break these into RAY_COUNT sub-angles and cast a ray for each angle. We'll be using a lookup table for that        	
	static constexpr auto START_POS_X = 1;
	static constexpr auto START_POS_Y = 7;
	static constexpr auto WALK_SPEED = 8;
	static constexpr auto ROTATION_SPEED = 16;		
	static constexpr auto TABLE_SIZE = static_cast<int>(VIEWPORT_WIDTH* (360.0f / FOV_DEGREES)); //how many elements we need to store the slope of every possible ray that can be projected.	
	static_assert(Utils::isPowerOfTwo(CELL_SIZE) && "Cell width and height must be a power-of-2");
};

//global constants derived from the config above:
static constexpr auto RAY_COUNT = Cfg::VIEWPORT_WIDTH; //one ray per column of screen space (horizontal resolution)        
//static constexpr auto TWO_PI = 2.0f * 3.141592654f; //defined by the Arduboy library
static constexpr auto ANGLE_360 = Cfg::TABLE_SIZE; //East (and total number of possible angles in a full rotation)
static constexpr auto ANGLE_180 = ANGLE_360 / 2; //West
static constexpr auto ANGLE_90 = ANGLE_360 / 4; //South
static constexpr auto ANGLE_270 = ANGLE_360 - ANGLE_90; //North
static constexpr auto ANGLE_0 = 0;  //back to East
static constexpr auto ANGLE_TO_RADIANS = (TWO_PI / ANGLE_360);
static constexpr auto HALF_FOV_ANGLE = Cfg::VIEWPORT_WIDTH / 2; // FoV/2 in angles (for table lookup) instead of degrees.    
static constexpr auto CELL_SIZE = Cfg::CELL_SIZE; //width and height of a cell in the game world, must be a power of 2.   
static constexpr auto OVERBOARD = CELL_SIZE / 2; // the absolute closest a player can get to a wall  
static constexpr auto VIEWPORT_HORIZON = Cfg::VIEWPORT_HEIGHT / 2;
