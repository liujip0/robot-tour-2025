#include <Romi32U4.h>

Romi32U4Encoders encoders;
Romi32U4Motors motors;

void setup() {
  // put your setup code here, to run once:
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

void loop() {
  // put your main code here, to run repeatedly:

}
