/*****************************************************************************

  Firmwar for SIGI'S SPECIAL


   Copyright (C) 2019 Infineon Technologies AG (INFINEON). All rights reserved.

   THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
   OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
   INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

 ******************************************************************************/


/* ---------------------------------------------------------------- */
/* Very basic sketch to read out data from the TLE493D-W2B6 2Go kit */
/* ---------------------------------------------------------------- */

#include <Wire.h>
#include "math.h"


// Number of averages
#define N 100


// Some subroutines
void configure_X1_mode() {
  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0x10);             // Register address
  Wire.write(0b00000001);       // CONFIG register
  Wire.write(0b00010101);       // MOD1 register
  Wire.endTransmission();

  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0x14);             // Register address
  Wire.write(0b00000000);       // Reserved register
  Wire.endTransmission();
}

void configure_X2_mode() {
  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0x10);             // Register address
  Wire.write(0b00001000);       // CONFIG register
  Wire.write(0b00010101);       // MOD1 register
  Wire.endTransmission();

  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0x14);             // Register address
  Wire.write(0b00000000);       // Reserved register
  Wire.endTransmission();
}

void configure_X4_mode() {
  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0x10);             // Register address
  Wire.write(0b00001000);       // CONFIG register
  Wire.write(0b00010101);       // MOD1 register
  Wire.endTransmission();

  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0x14);             // Register address
  Wire.write(0b00000001);       // Reserved register
  Wire.endTransmission();
}


void measurement() {

  double X_sum = 0;
  double Y_sum = 0;
  double Z_sum = 0;

  for (int16_t i = 0; i < N; i++)
  {
    // Trigger measurement (empty write command with set trigger bits)
    Wire.beginTransmission(0x35); // Sensor address
    Wire.write(0b00100000);       // trigger after I2c write frame is finished
    Wire.endTransmission();

    delayMicroseconds(500);  // add due to not available clock stretching for TLI493D-W2BW shield2go

    // Readout data (readout starts once the measurement is complete due to sensor clock stretching)
    Wire.requestFrom(0x35, 7);
    char c0 = Wire.read();
    char c1 = Wire.read();
    char c2 = Wire.read();
    char c3 = Wire.read();
    char c4 = Wire.read();
    char c5 = Wire.read();
    char c6 = Wire.read();

    // Built 12 bit data
    int16_t X = (int16_t)((c0 << 8) | (c4 & 0xF0)) >> 4;
    int16_t Y = (int16_t)((c1 << 8) | ((c4 & 0x0F) << 4)) >> 4;
    int16_t Z = (int16_t)((c2 << 8) | ((c5 & 0x0F) << 4)) >> 4;
    uint16_t T = (c3 << 4) | (c5 >> 4);

    X_sum += X;
    Y_sum += Y;
    Z_sum += Z;
  }

  // Send via serial port to be displayed on a terminal
  Serial.print(",");
  Serial.print(X_sum/ N);
  Serial.print(",");
  Serial.print(Y_sum/ N);
  Serial.print(",");
  Serial.print(Z_sum/ N);
  Serial.print(",");
  Serial.println();
}


void setup() {
  // put your setup code here, to run once:

  // turn on sensor Vdd (connected to pin XMC pin P1.0)
  //pinMode(15, OUTPUT);
  //digitalWrite(15, HIGH);

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  pinMode(1, OUTPUT);
  digitalWrite(1 , HIGH);

  delay(50);

  // Init I2C peripheral
  Wire.begin();
  Wire.setClock(400000);

  // Reset sensor
  Wire.beginTransmission(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  // Configure sensor
  configure_X2_mode();

  // Init serial port for communication to the PC
  Serial.begin(115200);
  while (!Serial);
}


void serialEvent() {
  char inChar;
  while (Serial.available()) {
    inChar = (char)Serial.read();

    switch (inChar) {
      case '1':
        //Serial.println("Full range mode");
        configure_X1_mode();
        break;
      case '2':
        //Serial.println("Short range mode");
        configure_X2_mode();
        break;
      case '4':
        //Serial.println("Extra short range mode");
        configure_X4_mode();
        break;
      case 'm':
        measurement();
        break;
      default:
        break;
    }
  }
}



void loop() {
  if(Serial.available())
    serialEvent();

  // do nothing and wait for interrupt -> received serial event
}
