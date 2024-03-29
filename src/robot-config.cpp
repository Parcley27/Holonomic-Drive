#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// Motors
motor frontRight = motor(PORT1, ratio18_1, true);
motor backRight = motor(PORT2, ratio18_1, false);
motor backLeft = motor(PORT3, ratio18_1, false);
motor frontLeft = motor(PORT4, ratio18_1, true);

// Sensors
distance frontDistance = distance(PORT11);
distance rightDistance = distance(PORT12);
distance backDistance = distance(PORT13);
distance leftDistance = distance(PORT14);

// Controller
controller mainController = controller(primary);


// Called from main.cpp
// Used to set motor stopping mode
void vexcodeInit(void) {
  frontRight.setStopping(brake);
  backRight.setStopping(brake);
  backLeft.setStopping(brake);
  frontLeft.setStopping(brake);
  
}