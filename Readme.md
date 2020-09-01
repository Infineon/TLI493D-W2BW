# TLI493D-W2BW Shield2Go
[![Build Status](https://travis-ci.org/Infineon/TLE493D-3DMagnetic-Sensor.svg?branch=master)](https://travis-ci.org/Infineon/TLE493D-3DMagnetic-Sensor)

<img src="https://github.com/Infineon/Assets/blob/master/Pictures/TLI493D-W2BW_Shield2Go.png" width="200">

Getting started link: https://www.infineon.com/dgdl/Infineon-TLI493D-W2BW_Shield2Go-GettingStarted-v01_00-EN.pdf?fileId=5546d462737c45b9017395f019797123

Arduino library for Infineon's 3D magnetic sensor TLI493D-W2BW. Notice that it is enough to use the class **Tli493d** if you want to use the basic setup. The class **Tli493d-w2bw** offers additional device-specific features. 

## Summary
The 3D Magnetic Sensor 2GO is a budget-priced evaluation board equipped with the magnetic sensor [TLE493D](https://www.infineon.com/cms/en/product/sensor/magnetic-position-sensor/3d-magnetic-sensor/tle493d-w2b6-a0/) for three dimensional measurement combined with an ARM® Cortex™-M0 CPU. The 3D Magnetic Sensor 2GO has a complete set of on-board devices, including an on-board debugger. Build your own application and gadget with the 3D Magnetic Sensor 2GO.

## Usage
Please follow the example sketches in the /examples directory in this library to learn more about the usage of the library.

Currently you have to make sure which sensor type you are using. The default type is `A0`:
```
Tle493d(AccessMode_e mode = MASTERCONTROLLEDMODE, TypeAddress_e productType = TLE493D_A0);
```
## Known Issues
- Reset sequence not working for TLE493d-W2B6. (Requires further testing)
```
mInterface.bus->begin();
mInterface.bus->write(0xFF);
mInterface.bus->end();

mInterface.bus->begin();
mInterface.bus->write(0xFF);
mInterface.bus->end();

mInterface.bus->begin();
mInterface.bus->write(0x00);
mInterface.bus->end();

mInterface.bus->begin();
mInterface.bus->write(0x00);
mInterface.bus->end();
```

- TLE493D-A2B6 does not work with low power mode.

- Sensor does not respond after being reconnected.
  As mentioned in [this issue](https://github.com/Infineon/TLE493D-3DMagnetic-Sensor/issues/6), the sensor works after being flashed, but not after being powered off and powered on again.
  This is temporarily solved by twice writing out the configuration registers, in order to avoid unexpected INT pulse.

## Key Features and Benefits
* TLE493D-A2B6 and TLE493D-W2B6 (three dimensional magnetic sensor)
* XMC1100 (ARM® Cortex™-M0 based)
* On-board J-Link Lite Debugger (Realized with XMC4200 Microcontroller)
* Power over USB (Micro USB), ESD and reverse current protection

## Installation
### Integration of XMC in Arduino IDE
Please note that base of the Sensors 2GO is the XMC 2Go from Infineon. Therefore, please install (if not already done) also the [XMC-for-Arduino](https://github.com/Infineon/XMC-for-Arduino) implementation and choose afterwards **XMC2Go** from the **Tools**>**Board** menu in the Arduino IDE if working with this evaluation board.

### Integration of Library
Please download this repository from GitHub by clicking on the following field in the [releases](https://github.com/Infineon/TLE493D-W2B6-3DMagnetic-Sensor/releases) of this repository or directly [here](https://github.com/Infineon/TLE493D-W2B6-3DMagnetic-Sensor/releases/download/V1.0.1/TLE493D-W2B6-3DMagnetic-Sensor.zip):

![Download Library](https://raw.githubusercontent.com/infineon/assets/master/Pictures/DL_TLE493D_W2B6_Rel.PNG)

To install the 3D magnetic sensor 2GO library in the Arduino IDE, please go now to **Sketch** > **Include Library** > **Add .ZIP Library...** in the Arduino IDE and navigate to the downloaded .ZIP file of this repository. The library will be installed in your Arduino sketch folder in libraries and you can select as well as include this one to your project under **Sketch** > **Include Library** > **MagneticSensor3D**.

![Install Library](https://raw.githubusercontent.com/infineon/assets/master/Pictures/Library_Install_ZIP.png)

## Processing
This library supports the open-source software [Processing](https://processing.org/) for creating GUIs. It allows you to connect your evaluation board to a PC over serial communication and visualisation of the embedded system. Find out more on the Arduino homepage [here](http://playground.arduino.cc/Interfacing/Processing). The respective files are stored in the /processing folder of this repository.

## GUI Exe Examples

Download [here](https://github.com/Infineon/TLE493D-3DMagnetic-Sensor/releases/download/untagged-8291dcde7f3b02fb819d/GUI_exe_examples_V1.0.zip) additional examples with graphical interface as executable files. 

## Board Information, Datasheet and Additional Information
A PDF summarizing the features and layout of the 3D magnetic sensor 2GO board is stored on the Infineon homepage [here](https://www.infineon.com/dgdl/Infineon-3D%20Magnetic%20Sensor-PB-v03_00-EN.pdf?fileId=5546d46261d5e6820161e7571b2b3dd0).
The datasheet for the TLE493D-W2B6 can be found here [TLE493D-W2B6 Datasheet](https://www.infineon.com/dgdl/Infineon-Infineon-TLE493D-W2B6%20A0-A3%203D%20Magnetic%20Sensor-DS-v01_00-EN.pdf?fileId=5546d46261764359016189ec158943a4) while respective application notes are located here [Application Notes](https://www.infineon.com/cms/en/product/sensor/magnetic-position-sensor/3d-magnetic-sensor/tle493d-w2b6-a0/#!documents).
