using namespace vex;

// Include brain for main.cpp
extern brain Brain;

// Include motors for main.cpp
extern motor frontRight;
extern motor backRight;
extern motor backLeft;
extern motor frontLeft;

// Include sensors or main.cpp
extern distance frontDistance;
extern distance rightDistance;
extern distance backDistance;
extern distance leftDistance;

// Include controller for main.cpp
extern controller mainController;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
