#pragma once
#include "Utils.h"
#include "Config.h"
#include "LevelData.h"
#include "Graphics.h"
#include "LUT.h"
class RayCaster {    
    struct RayStart {
        float intersection = 0.0f; //the first possible intersection point
        int boundary = 0; // the next intersection point   
        int delta = 0; // the amount needed to move to get to the next cell position
        int next_cell = 0; //cell delta, to move left / right or up / down
    };
    struct RayEnd {
        float distance = 0.0f; // the distance of intersection from the player
        int boundary = 0; // record intersections with cell boundaries        
        int intersection = 0; // used to save exact intersection point with a wall         
        bool operator <(const RayEnd& that) const noexcept { return distance < that.distance; };
    };    
    
    static constexpr auto WALL_BOUNDARY_COLOR = BLACK;
    static constexpr auto VERTICAL_WALL_COLOR = WHITE;
    static constexpr auto HORIZONTAL_WALL_COLOR = WHITE;         
    //The MAGIC_CONSTANT must be an even power-of-two >= WORLD_SIZE. Used to quickly round our position to nearest cell wall using bitwise AND.
    static constexpr auto MAGIC_CONSTANT = (Utils::isPowerOfTwo(WORLD_SIZE) ? WORLD_SIZE : Utils::nextPowerOfTwo(WORLD_SIZE))- Cfg::CELL_SIZE;
  
    RayStart initHorizontalRay(const int x, const int y, const int view_angle) const noexcept {        
        const auto FACING_RIGHT = (view_angle < ANGLE_90 || view_angle >= ANGLE_270);        
        const int x_bound = FACING_RIGHT ? CELL_SIZE + (x & MAGIC_CONSTANT) : (x & MAGIC_CONSTANT); //round x to nearest CELL_WIDTH (power-of-2), this is the first possible intersection point. 
        const int x_delta = FACING_RIGHT ? CELL_SIZE : -CELL_SIZE; // the amount needed to move to get to the next vertical line (cell boundary)
        const int next_cell_direction = FACING_RIGHT ? 0 : -1;  //x coordinates increase to the left, and decrease to the right      
        const float yi = pgm_read_float(&tan_table[view_angle]) * (x_bound - x) + y; // based on first possible vertical intersection line, compute Y intercept, so that casting can begin                                
        return RayStart{ yi, x_bound, x_delta, next_cell_direction };
    }

    RayStart initVerticalRay(const int x, const int y, const int view_angle) const noexcept {
        const auto FACING_DOWN = (view_angle >= ANGLE_0 && view_angle < ANGLE_180);
        const int y_bound = FACING_DOWN ? CELL_SIZE  + (y & MAGIC_CONSTANT) : (y & MAGIC_CONSTANT); //Optimization: round y to nearest CELL_HEIGHT (power-of-2) 
        const int y_delta = FACING_DOWN ? CELL_SIZE : -CELL_SIZE; // the amount needed to move to get to the next horizontal line (cell boundary)
        const int next_cell_direction = FACING_DOWN ? 0 : -1; //remember: y coordinates increase as we move down (south) in the world, and decrease towards the top (north)               
        const float xi = pgm_read_float(&inv_tan_table[view_angle]) * (y_bound - y) + x; // based on first possible horizontal intersection line, compute X intercept, so that casting can begin              
        return RayStart{ xi, y_bound, y_delta, next_cell_direction };
    }

    RayEnd findVerticalWall(const int x, const int y, const int view_angle) const noexcept  {
        auto [yi,  x_bound, x_delta, next_x_cell] = initHorizontalRay(x, y, view_angle); // cast a ray horizontally, along the x-axis, to intersect with vertical walls
        RayEnd result;
        while (x_bound > -1 && x_bound < WORLD_SIZE) {
            const int cell_x = ((x_bound + next_x_cell) / CELL_SIZE); //arduboy does not have a barrel shifter
            const int cell_y = static_cast<int>(yi) / CELL_SIZE;                   
            if (!isWall(cell_x, cell_y)) {
                yi += pgm_read_float(&y_step[view_angle]); // compute next Y intercept
                x_bound += x_delta; // move to next possible intersection points
                continue;
            }
            result.distance = (yi - y) * pgm_read_float(&inv_sin_table[view_angle]); // compute distance to hit
            result.boundary = x_bound; // record intersections with cell boundaries
            result.intersection = static_cast<int>(yi);
            return result;                        
        }        
        return result;          
    }  
  
    RayEnd findHorizontalWall(const int x, const int y, const int view_angle) const noexcept {
        auto [xi, y_bound, y_delta, next_y_cell] = initVerticalRay(x, y, view_angle); ///ast a ray vertically, along the y-axis, to intersect with horizontal walls
        RayEnd result;
        while (y_bound > -1 && y_bound < WORLD_SIZE) {
            const int cell_x = static_cast<int>(xi) / CELL_SIZE;   // the current cell that the ray is in             
            const int cell_y = ((y_bound + next_y_cell) / CELL_SIZE);
            if (!isWall(cell_x, cell_y)) {
                xi += pgm_read_float(&x_step[view_angle]); //compute next X intercept
                y_bound += y_delta;
                continue;
            }         
            result.distance = (xi - x) * pgm_read_float(&inv_cos_table[view_angle]);
            result.boundary = y_bound;
            result.intersection = static_cast<int>(xi);                                        
            return result;            
        }
        return result;
    }         
public:
    RayCaster() {} 
    void renderView(Graphics& g, const int x, const int y, int view_angle) const noexcept {
        // This function casts out RAY_COUNT rays from the viewer and builds up the display based on the intersections with the walls.
        // The distance to the first horizontal and vertical edge is recorded. The closest intersection is the one used to draw the display.
        // The inverse of that distance is used to compute the height of the "sliver" of texture that will be drawn on the screen                
        g.clearScreen();
        if ((view_angle -= HALF_FOV_ANGLE) < ANGLE_0) { // compute starting angle from player. Field of view is FOV angles, subtract half of that from the current view angle
            view_angle = ANGLE_360 + view_angle;
        }
        for (int ray = 0; ray < RAY_COUNT; ray++) {
            RayEnd xray = findVerticalWall(x, y, view_angle);  //cast a ray along the x-axis to intersect with vertical walls
            RayEnd yray = findHorizontalWall(x, y, view_angle); //cast a ray along the y-axis to intersect with horizontal walls
            auto color = WALL_BOUNDARY_COLOR;
            const float min_dist = (xray < yray) ? xray.distance : yray.distance;
            if (xray < yray) { // there was a vertical wall closer than a horizontal wall                
                if (xray.intersection % CELL_SIZE > 1) {
                    color = VERTICAL_WALL_COLOR;
                }             
            }
            else { // must have hit a horizontal wall first                            
                if (yray.intersection % CELL_SIZE > 1) {
                    color = HORIZONTAL_WALL_COLOR;
                }              
            }
            // height of the sliver is based on the inverse distance to the intersection. Closer is bigger, so: height = 1/dist. However, 1 is too low a factor to look good so we use a view-filter lookup-table
            const int height = static_cast<int>(pgm_read_float(&cos_table[ray]) / min_dist);
            const int clipped_height = (height > Cfg::VIEWPORT_HEIGHT) ? Cfg::VIEWPORT_HEIGHT : height;
            const int top = VIEWPORT_HORIZON - (clipped_height >> 1); //Optimization: height >> 1 == height / 2. slivers are drawn symmetrically around the viewport horizon.                         
            const int sliver_x = ray;       
            g.setColor(color);            
            g.drawVerticalLine(sliver_x, top, clipped_height - 1);    
            if (++view_angle == ANGLE_360) {
                view_angle = ANGLE_0; //reset angle back to zero
            }
        }
        g.present();
    }
};
