#include <Tli493d.h>

Tli493d sensor = Tli493d(5, LOW, Tli493d::LOWPOWERMODE);

void setup() {
  // setup sensor and serial communication with PC
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  // read data from sensor
  sensor.updateData();
  double x = sensor.getX();
  double y = sensor.getY();
  double z = sensor.getZ();
  // pole correction: if southpole faces sensor topside -> invert all components to have the same behavior as with the northpole
  if(z < 0)
  {
    x = -x;
    y = -y;
    z = -z;
  }
  //calculate br and theta
  double br = sqrt(x*x+y*y);
  double theta = atan2(br, z);

  //convert br, phi and theta to x- and y-outputs of the joystick
  double xVal = x * theta / br;
  double yVal = y * theta / br;

  //Write calculated values to the PC
  Serial.print("X: ");
  Serial.print(xVal);
  Serial.print("\tY: ");
  Serial.println(yVal);
  delay(10);
}
