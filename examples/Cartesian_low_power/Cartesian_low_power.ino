#include <Tli493d.h>

/**
* This example demonstrates the use of low power mode of sensor TLI493d-W2BW
*/

//Voltage level LOW at pin 5 switches on the sensor VDD; Operating mode is LOWPOWERMODE
Tli493d Tli493dMagnetic3DSensor = Tli493d(5, LOW, Tli493d::LOWPOWERMODE);

volatile bool interrupt = false;

void setup() {
	Serial.begin(9600);
	while (!Serial);
	Tli493dMagnetic3DSensor.begin();

	//The update rate is set to 3 (fastest is 0 and slowest is 7)
	Tli493dMagnetic3DSensor.setUpdateRate(3);
	//Interrupts will just be sent when any of the axis X,Y,Z sees a magnetic field higher than +/-3mT -> WakeUp-feature
	Tli493dMagnetic3DSensor.setWakeUpThresholdMT(3.0,-3.0,3.0,-3.0,3.0,-3.0);
	Tli493dMagnetic3DSensor.enableWakeUp();
	//Interrupt and Collision Avoidance are activated per default, so the next two lines are not really necessary
	Tli493dMagnetic3DSensor.enableInterrupt();
	Tli493dMagnetic3DSensor.enableCollisionAvoidance();

	//Sets measurement range to short: magnetic range is now +/- 100mT, but the sensitivity has increased to 15.4 LSB/mT
	Tli493dMagnetic3DSensor.setMeasurementRange(Tli493d::SHORT);

	//sensor interrupt is connected to pin 9
	pinMode(9, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(9), sensor_irq, FALLING);
  
}

void loop() {
	if(interrupt)
	{
		interrupt = false;

		Tli493dMagnetic3DSensor.updateData();

		Serial.print(Tli493dMagnetic3DSensor.getX());
		Serial.print(" ; ");
		Serial.print(Tli493dMagnetic3DSensor.getY());
		Serial.print(" ; ");
		Serial.println(Tli493dMagnetic3DSensor.getZ());

		delay(10);
	}
}

//This function is called, when the sensor sends an interrupt-pulse.
void sensor_irq() {
	interrupt = true;
}
