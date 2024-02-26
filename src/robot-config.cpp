#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// Motors
motor frontRight = motor(PORT1, ratio18_1, false);
motor backRight = motor(PORT2, ratio18_1, false);
motor backLeft = motor(PORT3, ratio18_1, false);
motor frontLeft = motor(PORT4, ratio18_1, false);

// Sensors
distance front = distance(PORT11);
distance right = distance(PORT12);
distance back = distance(PORT13);
distance left = distance(PORT14);

// Controller
controller mainController = controller(primary);


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}