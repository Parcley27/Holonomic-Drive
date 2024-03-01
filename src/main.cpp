/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

// Angle is the angle to drive on, where 0 is straight forward
// Speed is the speed to drive at, where 100 is full power and 0 is stopped
void drive(int angle, int speed) {
  // Use trig to find digital controller positions
  // Math is available here: https://www.desmos.com/calculator/ljddj1h4ck
  // hypoteneus length == speed
  // hypoteneus leg angle == angle

  double radianAngle = angle * M_PI / 180.0;

  // Find x distnace with trigonometry
  int digitalX = speed * sin(radianAngle);

  // Square hypoteneus and x vector
  int speedSquared = speed * speed;
  int xSquared = digitalX * digitalX;

  // Find last leg of vector triangle with pythagorean theorem
  int digitalY = speedSquared - xSquared;
  digitalY = sqrt(digitalY);

  // Adjust for "negative" degrees that should make the robot go backwards
  if (angle > 90 && angle < 270) {
    digitalY = -digitalY;
  }

  // Assign speeds to motors
  frontRight.setVelocity(-(digitalX - digitalY), pct);
  backRight.setVelocity(-(digitalX + digitalY), pct);
  backLeft.setVelocity(-(digitalX - digitalY), pct);
  frontLeft.setVelocity(-(digitalX + digitalY), pct);

  // Turn on motors
  frontRight.spin(forward);
  backRight.spin(forward);
  backLeft.spin(forward);
  frontLeft.spin(forward);

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            Autonomous Portion                             */
/*                                                                           */
/*       Used for the autonomous race with the help of drive() above         */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                User Control                               */
/*                                                                           */
/*            Used for the user controll obstacle course challenge           */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop

  while (1) {
    // Compute velocities
    // forwards, sideways, rotation
    // x, y, rotation
    int frontRightSpeed = mainController.Axis3.position() - mainController.Axis4.position() - (mainController.Axis1.position()/2);
    int backRightSpeed = -mainController.Axis3.position() - mainController.Axis4.position() + (mainController.Axis1.position()/2);
    int backLeftSpeed = mainController.Axis3.position() - mainController.Axis4.position() + (mainController.Axis1.position()/2);
    int frontLeftSpeed = -mainController.Axis3.position() - mainController.Axis4.position() - (mainController.Axis1.position()/2);

    // Set velocities
    frontRight.setVelocity(frontRightSpeed, pct);
    backRight.setVelocity(backRightSpeed, pct); 
    backLeft.setVelocity(backLeftSpeed, pct);
    frontLeft.setVelocity(frontLeftSpeed, pct);

    // turn on motors
    frontRight.spin(forward);
    backLeft.spin(forward);
    backRight.spin(forward);
    frontLeft.spin(forward);

    
    // Drive Test

    if (mainController.ButtonA.pressing()) {
      drive(0, 50);
    }
    //drive(0, 50);;

    // Motor Test
    /*
    frontRight.spin(forward);
    backRight.spin(forward);
    backLeft.spin(forward);
    frontLeft.spin(forward);
    */

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
