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
  // Repeat forever
  while (true) {
    // Basic autonomous logic is as follows:
    // Go straight until next angle is less than 200mm away, then turn 45 degrees
    // Then go until the next wall is less than 100mm away, then turn another 45 degrees, then repeat

    // Set speed for autonomous portion
    // Can be from 0 ... 100
    const int speed = 50;

    // Will change through the loop to find robot heading
    // In degrees where 0 == straight forward
    int direction = 0;

    // Create array with robot sensors
    // Can be iterated through to use the right sensor at the rigt time
    distance sensor[4] = {frontDistance, rightDistance, backDistance, leftDistance};

    // Loop 4 times
    for (int i = 0; i < 4; i++) {

      // Prevent over 360
      direction = i * 90;

      // Drive until the sensor is less that 200mm away from an object
      while (sensor[i].objectDistance(mm) > 300) {
        drive(direction, speed);
      }

      // Change drive direction by 45 degrees
      direction += 45;

      // Drive until the sensor is less than 100mm away from an object
      while (sensor[i].objectDistance(mm) > 150) {
        drive(direction, speed);
      }
    }
  }
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

    
    // drive() function test, can also be used during user controll
    if (mainController.ButtonX.pressing()) {
      // angle, speed
      drive(0, 100);

    } else if (mainController.ButtonA.pressing()) {
      drive(90, 100);

    } else if (mainController.ButtonB.pressing()) {
      drive(180, 100);

    } else if (mainController.ButtonY.pressing()) {
      drive(270, 100);

    if (mainController.ButtonA.pressing()) {
      drive(0, 50);
    }
    //drive(0, 50);;

    // autonomous() function test, should not be used during user controll competition
    if (mainController.ButtonRight.pressing()) {
      autonomous();
    }
    
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
