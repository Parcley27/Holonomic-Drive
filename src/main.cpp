/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Pierce O and Luca B                                       */
/*    Created:      February 26, 2024                                         */
/*    Description:  Holonomic drive program for a VEX drivetrain competition  */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "vex.h"

// include cmath for trigonometry in drive()
#include <cmath>

using namespace vex;

// Global instance of competition
competition Competition;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*                   Run vexcodeInit to set motor directions                 */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

}

// Angle is the angle to drive on, where 0 is straight forward
// Speed is the speed to drive at, where 100 is full power and 0 is stopped
void drive(int angle, int speed) {
  // Use trig to find digital controller positions
  // Math is available here: https://www.desmos.com/calculator/ljddj1h4ck
  // hypoteneus length == speed
  // hypoteneus leg angle == angle

  // Correct for angles equal to or greater than a full circle
  if (angle >= 360) {
    angle = 0;
  }

  // Convert to radians
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

      // Drive until the sensor is less that 250mm away from an object
      while (sensor[i].objectDistance(mm) > 250) {
        drive(direction, speed);
      }

      // Change drive direction by 45 degrees
      direction += 45;

      // Drive until the sensor is less than 145mm away from an object
      while (sensor[i].objectDistance(mm) > 145) {
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
  while (1) {
    // Calculate the correct speed for each motor
    // Forwards speed +- Horizontal speed +- Rotation speed (depends on motor position and orientation)
    int frontRightSpeed = mainController.Axis3.position() - mainController.Axis4.position() - (mainController.Axis1.position()/2);
    int backRightSpeed = -mainController.Axis3.position() - mainController.Axis4.position() + (mainController.Axis1.position()/2);
    int backLeftSpeed = mainController.Axis3.position() - mainController.Axis4.position() + (mainController.Axis1.position()/2);
    int frontLeftSpeed = -mainController.Axis3.position() - mainController.Axis4.position() - (mainController.Axis1.position()/2);

    // Write velocites to motors
    frontRight.setVelocity(frontRightSpeed, pct);
    backRight.setVelocity(backRightSpeed, pct); 
    backLeft.setVelocity(backLeftSpeed, pct);
    frontLeft.setVelocity(frontLeftSpeed, pct);

    // Ensure the motors are on
    frontRight.spin(forward);
    backLeft.spin(forward);
    backRight.spin(forward);
    frontLeft.spin(forward);

    
    // drive() function test, but can also be used during user controll competition
    // Direction is matched with letter button positions on the controller
    if (mainController.ButtonX.pressing()) {
      // heading, speed
      drive(0, 100);

    } else if (mainController.ButtonA.pressing()) {
      drive(90, 100);

    } else if (mainController.ButtonB.pressing()) {
      drive(180, 100);

    } else if (mainController.ButtonY.pressing()) {
      drive(270, 100);

    }

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

    // Sleep the task for a short amount of time to prevent wasted resources.
    wait(20, msec);

  }
}

// Main function where everything happens inside
int main() {
  // Set up callbacks for autonomous and driver control periods
  // Allows the robot to be controlled with the competition controller
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run pre autonamous function for robot initialization
  pre_auton();

  // Run main forever
  while (true) {
    wait(100, msec);
  }
}
