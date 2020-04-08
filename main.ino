#include <Arduboy2.h>
#include "Config.h"
#include "RayCaster.h"
#include "ViewPoint.h"
Arduboy2 arduboy;
Graphics _g{arduboy};
ViewPoint _viewPoint{ Cfg::START_POS_X, Cfg::START_POS_Y, ANGLE_0 };
RayCaster _ray;
void setup() {
    arduboy.begin();
    arduboy.setFrameRate(30);  
}

void loop() {  
  if (!arduboy.nextFrame()){
    return;
  }  
  _viewPoint.update(arduboy);
  _viewPoint.checkCollisions();
  _ray.renderView(_g, _viewPoint.x, _viewPoint.y, _viewPoint.angle);
}
