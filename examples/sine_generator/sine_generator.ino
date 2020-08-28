// this examples is to be used with processing/sine_generator.pde
#include <Tli493d.h>

Tli493d Tli493dMagnetic3DSensor = Tli493d();
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Tli493dMagnetic3DSensor.begin();
}

void loop() {
  Tli493dMagnetic3DSensor.updateData();
  Serial.print(Tli493dMagnetic3DSensor.getAzimuth());
  Serial.print("\t");
  if(Tli493dMagnetic3DSensor.getAzimuth()>0)
    Serial.println(Tli493dMagnetic3DSensor.getPolar());
  else
    Serial.println(-Tli493dMagnetic3DSensor.getPolar());
}