# TLI493D-W2BW Shield2Go

[![Build Status](https://travis-ci.org/Infineon/TLE493D-3DMagnetic-Sensor.svg?branch=master)](https://travis-ci.org/Infineon/TLE493D-3DMagnetic-Sensor)

<img src="https://github.com/Infineon/Assets/blob/master/Pictures/TLI493D-W2BW_Shield2Go.png" height="400">                                                       <img src="https://github.com/Infineon/Assets/blob/master/Pictures/TLI493D-W2BW_sensor.jpg" height="200">

Arduino library for Infineon's 3D magnetic sensor [TLI493D-W2BW](https://www.infineon.com/cms/en/product/sensor/magnetic-sensors/magnetic-position-sensors/3d-magnetics/tli493d-w2bw-a0/).

For a basic board description refer to the [TLI493D-W2BW Shield2Go Quick starting guide](https://www.infineon.com/dgdl/Infineon-TLI493D-W2BW_Shield2Go-GettingStarted-v01_00-EN.pdf?fileId=5546d462737c45b9017395f019797123).


## Summary
The TLI493D-W2BW Shield2Go is a budget-priced evaluation board equipped with the magnetic sensor [TLI493D-W2BW](https://www.infineon.com/cms/en/product/sensor/magnetic-sensors/magnetic-position-sensors/3d-magnetics/tli493d-w2bw-a0/) for three dimensional magnetic measurements. It is intended to be combined with a [XMC2Go kit](https://www.infineon.com/cms/de/product/evaluation-boards/kit_xmc_2go_xmc1100_v1/).

## Usage
Please follow the example sketches in the /examples directory in this library to learn more about the usage of the library.

Notice that it is enough to use the class **Tli493d** if you want to use the basic setup. The class **Tli493d-w2bw** offers additional device-specific features. 

Currently you have to make sure which sensor type you are using. The default type is `A0`:
```
Tle493d(AccessMode_e mode = MASTERCONTROLLEDMODE, TypeAddress_e productType = TLE493D_A0);
```
### Known Issues
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
