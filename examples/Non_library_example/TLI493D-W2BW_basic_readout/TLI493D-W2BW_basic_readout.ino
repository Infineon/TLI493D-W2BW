/*****************************************************************************
 *
 * Copyright (C) 2020 Infineon Technologies AG (INFINEON). All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/
 
/* ---------------------------------------------------------------- */
/* Very basic sketch to read out data from the TLI493D-W2BW sensor  */
/* ---------------------------------------------------------------- */

#include <Wire.h>       // default I²C library

#define DELAY 500       // value in ms, change to modify update rate
#define INT_PIN 9       // XMC pin connected to the sensor interrupt


void setup() {
  // Init I2C peripheral
  Wire.begin();
  Wire.setClock(400000);

  // Configure interrupt pin
  pinMode(INT_PIN, INPUT);

  // Reset sensor
  Wire.beginTransmission(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  // Configure sensor
  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0x10);             // Register address
  Wire.write(0b00000000);       // Config register: measure all channels, full range, odd CP parity
  Wire.write(0b00011001);       // MOD1 register: master controlled mode, one byte read command, enabled interrupt, no collision avoidance, odd FP parity
  Wire.endTransmission();
  
  // Init serial port for communication to the PC
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Bx By Bz T");
}

void loop() {
  // Trigger measurement (empty write command with set trigger bits)
  Wire.beginTransmission(0x35); // Sensor address
  Wire.write(0b00100000);       // trigger after I2c write frame is finished
  Wire.endTransmission();

  // Wait for interrupt
  while(digitalRead(INT_PIN));
  
  // Readout the first 7 data bytes
  uint8_t buf[7];
  Wire.requestFrom(0x35, 7);
  for (uint8_t i = 0; i < 7; i++) {
    buf[i] = Wire.read();
  }

  // Built 12 bit data 
  int16_t X = (int16_t)((buf[0] << 8) | (buf[4] & 0xF0)) >> 4;
  int16_t Y = (int16_t)((buf[1] << 8) | ((buf[4] & 0x0F) << 4)) >> 4;
  int16_t Z = (int16_t)((buf[2] << 8) | ((buf[5] & 0x0F) << 4)) >> 4;
  uint16_t T = (buf[3] << 4) | (buf[5] >> 4);

  /* --------------------------------------------------------- */
  /* Enter your application code here*/
  /* --------------------------------------------------------- */
  
  // Send via serial port to be displayed on a terminal 
  Serial.print(X);
  Serial.print(" ");
  Serial.print(Y);
  Serial.print(" ");
  Serial.print(Z);
  Serial.print(" ");
  Serial.print(T);
  Serial.println();

  delay(DELAY); // pause in ms
}
