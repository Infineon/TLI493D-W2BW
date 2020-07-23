#include <Tli493d_w2b6.h>

/**
* This example demonstrates the use of low power mode of sensor TLI493d-W2BW
*/


Tli493d Tli493dMagnetic3DSensor = Tli493d(Tli493d::LOWPOWERMODE);
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Tli493dMagnetic3DSensor.begin();
  // The highest adjustable range is used for all three directions, i.e., within half of the total value range INT is disabled 
  // this has no effect on the TLI493d-A2B6 sensor
  Tli493dMagnetic3DSensor.setWakeUpThreshold(1,-1,1,-1,1,-1);

  //The update rate is set to 3 (fastest is 0 and slowest is 7)
  Tli493dMagnetic3DSensor.setUpdateRate(3);
}

void loop() {
  Tli493dMagnetic3DSensor.updateData();

  Serial.print(Tli493dMagnetic3DSensor.getX());
  Serial.print(" ; ");
  Serial.print(Tli493dMagnetic3DSensor.getY());
  Serial.print(" ; ");
  Serial.println(Tli493dMagnetic3DSensor.getZ());

  delay(10);
}
