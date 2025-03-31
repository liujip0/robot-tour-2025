#include <Romi32U4.h>
#include <LSM6.h>

const char TURN_LEFT = 'L';
const char TURN_RIGHT = 'R';
const char FORWARD = 'F';
const char BACK = 'B';
const char FIRST_FORWARD = '1';

const char movements[] = {
  FIRST_FORWARD,
  FORWARD,
  FORWARD,
  FORWARD,
  FORWARD
};

Romi32U4Encoders encoders;
Romi32U4Motors motors;
Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4Buzzer buzzer;
LSM6 imu;

char report[80];

int leftEncoder = 0;
int rightEncoder = 0;

const int RIGHT_MOTOR_SPEED = 100;
const int LEFT_MOTOR_SPEED = round(1.0595 * RIGHT_MOTOR_SPEED); // 1.077 // 1.065 // 1.0655
const int RIGHT_TURN_ENCODER_COUNTS = 737;
const int LEFT_TURN_ENCODER_COUNTS = 739;
const int FORWARD_ENCODER_COUNTS = 3250;
const int FIRST_FORWARD_ENCODER_COUNTS = round(0.6 * FORWARD_ENCODER_COUNTS);

void setup() {
  // put your setup code here, to run once:
  leftEncoder = encoders.getCountsAndResetLeft();
  rightEncoder = encoders.getCountsAndResetRight();

  imu.init();
  imu.enableDefault();
  imu.writeReg(LSM6::CTRL2_G, 0b10001000);
  imu.read();
  imu.g.x;

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.write("");
  // Serial.println();
  buttonA.waitForPress();
  buttonA.waitForRelease();
  for (int i = 0; i < sizeof(movements); i++) {
    switch (movements[i]) {
      case FIRST_FORWARD:
        first_forward();
        break;
      case FORWARD:
        forward();
        break;
      case BACK:
        back();
        break;
      case TURN_LEFT:
        left();
        break;
      case TURN_RIGHT:
        right();
        break;
    }
  }
}

void forward() {
  leftEncoder = encoders.getCountsAndResetLeft();
  rightEncoder = encoders.getCountsAndResetRight();
  leftEncoder = 0;
  rightEncoder = 0;
  motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
  buzzer.play("E");
  while (leftEncoder < FORWARD_ENCODER_COUNTS && rightEncoder < FORWARD_ENCODER_COUNTS) {
    leftEncoder = encoders.getCountsLeft();
    rightEncoder = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
}

void first_forward() {
  leftEncoder = encoders.getCountsAndResetLeft();
  rightEncoder = encoders.getCountsAndResetRight();
  leftEncoder = 0;
  rightEncoder = 0;
  motors.setSpeeds(LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
  buzzer.play("C");
  while (leftEncoder < FIRST_FORWARD_ENCODER_COUNTS && rightEncoder < FIRST_FORWARD_ENCODER_COUNTS) {
    leftEncoder = encoders.getCountsLeft();
    rightEncoder = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
}

void back() {
  leftEncoder = encoders.getCountsAndResetLeft();
  rightEncoder = encoders.getCountsAndResetRight();
  leftEncoder = 0;
  rightEncoder = 0;
  motors.setSpeeds(-LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
  buzzer.play("D");
  while (leftEncoder > -FORWARD_ENCODER_COUNTS && rightEncoder > -FORWARD_ENCODER_COUNTS) {
    leftEncoder = encoders.getCountsLeft();
    rightEncoder = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
}

void right() {
  leftEncoder = encoders.getCountsAndResetLeft();
  rightEncoder = encoders.getCountsAndResetRight();
  leftEncoder = 0;
  rightEncoder = 0;
  if (leftEncoder == 0) {
    buzzer.play("G");
  }
  motors.setSpeeds(LEFT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
  if (leftEncoder > 0) {
    buzzer.play("A");
  } else {
    buzzer.play("B");
  }
  while (leftEncoder < RIGHT_TURN_ENCODER_COUNTS && rightEncoder > -RIGHT_TURN_ENCODER_COUNTS) {
    leftEncoder = encoders.getCountsLeft();
    rightEncoder = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
}

void left() {
  leftEncoder = encoders.getCountsAndResetLeft();
  rightEncoder = encoders.getCountsAndResetRight();
  leftEncoder = 0;
  rightEncoder = 0;
  if (leftEncoder == 0) {
    buzzer.play("F");
  }
  motors.setSpeeds(-LEFT_MOTOR_SPEED, RIGHT_MOTOR_SPEED);
  if (leftEncoder >= 0) {
    buzzer.play("A");
  } else {
    buzzer.play("B");
  }
  while (leftEncoder > -LEFT_TURN_ENCODER_COUNTS && rightEncoder < LEFT_TURN_ENCODER_COUNTS) {
    leftEncoder = encoders.getCountsLeft();
    rightEncoder = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
}