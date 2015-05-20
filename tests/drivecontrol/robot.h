#ifndef ROBOT_H
#define ROBOT_H

// Ardumower robot 

#include <Arduino.h>
#include "drivers.h"
#include "adcman.h"
#include "buzzer.h"
#include "led.h"
#include "motorcontrol.h"
#include "modelrc.h"
#include "behavior.h"
#include "arbitrator.h"


class RobotControl
{
  public:
    unsigned long loopCounter;
    DriveForwardBehavior driveForwardBehavior;  
    HitObstacleBehavior  hitObstacleBehavior;
    Arbitrator arbitrator;  
    RobotControl();    
    virtual void run();
    virtual void setup();
    virtual void loop();
  private:
    
};


extern RobotControl Robot;  

  
#endif
