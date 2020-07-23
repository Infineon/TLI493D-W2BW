/**
* For basic setup just create a Tli493d() object. If you want to use the wake up mode please use Tli493d_w2b6(). Also
* the setUpdateRate() method is slightly different for different variants
*/


#include <Tli493d.h>

Tli493d Tli493dMagnetic3DSensor = Tli493d();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Tli493dMagnetic3DSensor.begin();
  Tli493dMagnetic3DSensor.enableTemp();
}

void loop() {
  Tli493dMagnetic3DSensor.updateData();

  Serial.print(Tli493dMagnetic3DSensor.getX());
  Serial.print(" ; ");
  Serial.print(Tli493dMagnetic3DSensor.getY());
  Serial.print(" ; ");
  Serial.println(Tli493dMagnetic3DSensor.getZ());
  
  delay(500);
}

