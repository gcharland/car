#include <SoftwareSerial.h>
#include <Motor.h>

#define leftFwd 5
#define leftBwd 6
#define rightFwd 10
#define rightBwd 9

#define HC05_RXD 2
#define HC05_TXD 3

Motor leftWheel(leftFwd, leftBwd);
Motor rightWheel(rightFwd, rightBwd);

SoftwareSerial BTSerial(HC05_TXD, HC05_RXD);

long distance;
int serial_byte, last_motion_command; // int en lien avec la lectire des commandes par Serial
int spd = 0;
bool isForward = false;

/*
 * *************
 * *** SETUP ***
 * *************
 */
void setup() {
  pinMode(leftFwd, OUTPUT);
  pinMode(leftBwd, OUTPUT);
  pinMode(rightFwd, OUTPUT);
  pinMode(rightBwd, OUTPUT);

  //Serial.begin(9600);
  BTSerial.begin(9600);
}

/*
 * ************
 * *** LOOP ***
 * ************
 */

void loop() {
  if (BTSerial.available() > 0) {
    serial_byte = BTSerial.read();

    //Serial.print("I recieved ");
    //Serial.println(serial_byte);

    if (serial_byte == 119 || serial_byte == 115 || serial_byte == 97 || serial_byte == 100 || serial_byte == 32) {
      readMotion();
    } else if (serial_byte == 113 || serial_byte == 101) {
      readSpeed();
    }
  }
}

void readMotion() {
  if (serial_byte == 119) {
    moveForward();
    isForward = true;
  } else if (serial_byte == 115) {
    moveBackward();
    isForward = false;
  } else if (serial_byte == 97) {
    turnLeft();
  } else if (serial_byte == 100) {
    turnRight();
  } else if (serial_byte == 32) {
    stopMotion();
  }
  last_motion_command = serial_byte;
}

void readSpeed() {
  if (serial_byte == 113 && spd > 5) {
    spd = spd - 5;
  } else if (serial_byte == 101 && spd < 249) {
    spd = spd + 5;
  }
  serial_byte = last_motion_command;
  readMotion();
}

void moveForward() {
  leftWheel.forward(spd);
  rightWheel.forward(spd);
  //Serial.print("Moving fwd at ");
  //Serial.println(spd);
}

void moveBackward() {
  leftWheel.backward(spd);
  rightWheel.backward(spd);
  //Serial.print("Moving bwd at ");
  //Serial.println(spd);
}

void turnLeft() {
  if (isForward) {
    leftWheel.forward(spd/2);
    rightWheel.forward(spd);
  } else {
    leftWheel.backward(spd/2);
    rightWheel.backward(spd);
  }
  //Serial.println("Turning left");
}

void turnRight() {
  if (isForward) {
    leftWheel.forward(spd);
    rightWheel.forward(spd/2);
  } else {
    leftWheel.backward(spd);
    rightWheel.backward(spd/2);
  }
  //Serial.println("Turning right");
}

void stopMotion() {
  leftWheel.brake();
  rightWheel.brake();
  //Serial.println("Braking");
}

