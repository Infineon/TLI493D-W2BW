# TLI493D-W2BW - 3D magnetic sensor

[![Build Status](https://travis-ci.org/Infineon/TLE493D-3DMagnetic-Sensor.svg?branch=master)](https://travis-ci.org/Infineon/TLE493D-3DMagnetic-Sensor)

<img src="https://github.com/Infineon/Assets/blob/master/Pictures/TLI493D-W2BW_sensor.jpg" height="300">

Arduino library for Infineon's wafer level package 3D magnetic sensor [TLI493D-W2BW](https://www.infineon.com/cms/en/product/sensor/magnetic-sensors/magnetic-position-sensors/3d-magnetics/tli493d-w2bw-a0/).

## TLI493D-W2BW Shield2Go 
The TLI493D-W2BW sensor comes mounted on the [TLI493D-W2BW Shield2Go](https://www.infineon.com/cms/en/product/evaluation-boards/s2go_3d_tli493dw2bw-a0/) to provide an easy to use evaluation environment. The Shield2Go can directly be stacked on an [XMC2Go kit](https://www.infineon.com/cms/de/product/evaluation-boards/kit_xmc_2go_xmc1100_v1/) compatible with this library. For a basic board description refer to the [TLI493D-W2BW Shield2Go Quick starting guide](https://www.infineon.com/dgdl/Infineon-TLI493D-W2BW_Shield2Go-GettingStarted-v01_00-EN.pdf?fileId=5546d462737c45b9017395f019797123).

<img src=https://github.com/Infineon/Assets/blob/master/Pictures/TLI493D-W2BW_pinout.png height=500>

## Summary
The TLI493D-W2BW is a new wafer level package low power 3D magnetic sensor. 

### Key features
- 3D (X, Y, Z) magnetic flux density sensing of ±160 mT
Programmable flux resolution down to typ. 65 µT
- Extremely small form factor: typ. 1.13 mm * 0.93 mm * 0.59 mm
- X-Y angular measurement mode
- Power down mode with 7 nA (typ.) power consumption
- 12-bit data resolution for each measurement direction plus 10-bit temperature sensor
- Variable update frequencies and power modes (configurable during operation)
- Temperature range Tj = -40°C…125°C, supply voltage range = 2.8 V…3.5 V
- Triggering by external microcontroller possible via I2 C protocol
- Interrupt signal to indicate a valid measurement to the microcontroller

### Benefits
- Component reduction due to 3D magnetic measurement principle
- Small sensor form factor allows for very compact system designs
- Wide application range addressable due to high flexibility
- Platform adaptability due to device configurability
- Very low system power consumption due to Wake-Up mode
- Disturbance of smaller stray fields are neglectable compared to the high magnetic flux measurement range

## Usage
You can specify the sensors operating mode and which sensor type you are using. The default is MASTERCONTROLLEDMODE with type `A0`:
```
Tle493d(AccessMode_e mode = MASTERCONTROLLEDMODE, TypeAddress_e productType = TLE493D_A0);
```
If the VDD of the sensor can be controlled by a GPIO of the microcontroller - as it is the case with the Shield2Go - you can specify this pin and its power-on-state. When you do not specify a pin the default is NO_POWER_PIN, if you don't specify a power level, the default is HIGH:
```
Tle493d(int powerPin = NO_POWER_PIN, bool powerLevel = HIGH);
```
When you want to select the accessmode and a power pin, you can use one of the following combined constructors:
```
Tli493d(int resetPin, bool powerLevel = HIGH, AccessMode_e mode = MASTERCONTROLLEDMODE, TypeAddress_e productType = TLI493D_A0);
Tli493d(AccessMode_e mode = MASTERCONTROLLEDMODE, TypeAddress_e productType = TLI493D_A0, int resetPin = NO_POWER_PIN, bool powerLevel = HIGH);
```

In order to initially configure the sensor and start the communication, you need to use the function _begin()_. Without argument or with the argument **false** the sensor will not be reset at the beginning. This means, if e.g. the temperature measurement was disabled before and you upload a new sketch that does not turn it on explicitly, the temperature measurement will stay inactive. The argument **true** resets the sensor before configuration, so in the previous scenario, the temperature measurement would be enabled afterwards as it is enabled by factory settings.
```
begin();
begin( bool reset );
```
After the communication has been setup successfully by the _begin()_-command, the sensor can be configured by different functions. Please follow the example sketches in the /examples directory in this library to learn more about the possible configurations.
When the configuration was done, the sensor data can be read via _updateData()_.
Afterwards _getBx()_, _getBy()_, _getBz()_ and _getTemp()_ calculate the magnetic field and temperature out of the raw sensor values.

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
