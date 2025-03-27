#include <Romi32U4.h>
#include <LSM6.h>

char movements[] = {
  'L'
};

Romi32U4Encoders encoders;
Romi32U4Motors motors;
Romi32U4ButtonA buttonA;
Romi32U4Buzzer buzzer;
LSM6 imu;

char report[80];

int leftEncoder = 0;
int rightEncoder = 0;

int RIGHT_MOTOR_SPEED = 100;
int LEFT_MOTOR_SPEED = round(1.077 * RIGHT_MOTOR_SPEED);
int TURN_ENCODER_COUNTS = 770;
int FORWARD_ENCODER_COUNTS = 3250;

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

  buttonA.waitForPress();
  buttonA.waitForRelease();
  forward();
}

void loop() {
  // put your main code here, to run repeatedly:
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

void right() {
  leftEncoder = encoders.getCountsAndResetLeft();
  rightEncoder = encoders.getCountsAndResetRight();
  leftEncoder = 0;
  rightEncoder = 0;
  if (leftEncoder == 0) {
    buzzer.play("G");
  }
  motors.setSpeeds(RIGHT_MOTOR_SPEED, -RIGHT_MOTOR_SPEED);
  if (leftEncoder > 0) {
    buzzer.play("A");
  } else {
    buzzer.play("B");
  }
  while (leftEncoder < TURN_ENCODER_COUNTS && rightEncoder > -TURN_ENCODER_COUNTS) {
    leftEncoder = encoders.getCountsLeft();
    rightEncoder = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
}