![Image](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/logo.png)

[![Donate](https://img.shields.io/badge/buy_me-one_beer-red.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D5XZ7QFLP8LXE) [![Build Status](https://travis-ci.org/jbrazio/ardufocus.svg?branch=master)](https://travis-ci.org/jbrazio/ardufocus) [![GitHub issues](https://img.shields.io/github/issues/jbrazio/ardufocus.svg)](https://github.com/jbrazio/ardufocus/issues) [![GitHub stars](https://img.shields.io/github/stars/jbrazio/ardufocus.svg)](https://github.com/jbrazio/ardufocus/stargazers) [![GitHub forks](https://img.shields.io/github/forks/jbrazio/ardufocus.svg)](https://github.com/jbrazio/ardufocus/network) [![Download beta](https://img.shields.io/badge/download-latest_beta-yellow.svg)](https://github.com/jbrazio/ardufocus/archive/master.zip)

Ardufocus is a full open source [Moonlite][moonlite] compatible automatic focus controller.
The source is still under heavy development so things move around a bit.

[moonlite]: https://www.focuser.com/


# Motivation
After buying the CCD, filter wheel and filters I was broke but still wanted to have an automatic focuser.


# Design Goals

## 1. Moonlite compatible
This was a very important part of the design as I didn't want to spend time and effort dealing with ASCOM and INDI drivers, the Moonlite focuser is a well known, reputable rock solid focuser. The serial protocol used by them was easily reversed engineered (plain ASCII) and most of it was already documented on the Internet.

## 2. Cheap
Another big point was to made it as cheap as possible recurring to as few parts as needed. That's the reason why the 28BYJ-48 stepper motor was chosen, out-of-the-box using the ULN2003 gives you a really cheap (less than 2€) focuser for medium loads (380gcm). If you require the focuser to driver heavier loads (800gcm) then the motor itself can be modded into a Bipolar stepper motor and driven by the A4988 step stick which will cost you less than 1€.

## 3. Builder friendly
Using off-the-shelf components such as the Arduino Nano and easily available parts Ardufocus is aimed to be build by anyone with a soldering iron and some patience, no degree in electronics required.


# Hardware
It was built on top of a standard ATmega 328 Arduino such as UNO, Pro or Nano; currently it does not support the Mega or any other ARM based board.

Currently it supports two main motor/controller configurations:
1. Bipolar stepper motor with a A4988 driver
2. Unipolar stepper motor with a ULN2003 driver

The option #1 (bipolar/A4988) will allow your focuser to drive bigger loads, the motor used can range from the [modified version] of the 28BYJ-48 to a NEMA17. If you're on a budget or have lighter loads then the option #2 (unipolar/ULN2003) combo can be used.

I have no issues driving an ASI1600MMC-Cool with a ZWO EFW8 and a 0.85x field flattener using the [modified version] of the 28BYJ-48 with a A4988 driver on an ED80 telescope.

[modified version]: https://github.com/jbrazio/ardufocus/blob/master/doc/28byj-48-bipolar-mod.pdf

## BOM

This BOM is to be used by the prefboard version of the controller. A PCB specific BOM will be published soon, in the meanwhile check the schematic for specific parts.

Common components:
- 1x [Arduino Nano]
- 1x [Electrolytic capacitor 10uF 25V]
- 1x [Electrolytic capacitor 100uF 50V]
- 1x [NTC 10K]
- 1x [Resistor 1/4W 10K]
- 1x [DC Power connector] (male, female pair)

Bipolar/A4988 components:
- 1x [28BYJ-48 stepper motor] (12V version)
- 1x [A4988 stepper motor driver module]

Unipolar/ULN2003 components:
- 1x [28BYJ-48 stepper motor]
- 1x [ULN2003 stepper motor driver module]

Optional components:

To connect your controller box to the motor you have two main options, either you use a direct connection and the controller will always be attached to your telescope or you use the following:
- 1x DB9 connector (male, female pair): This will allow the cable to be connected to the motor.
- 1x [GX12-4] or [GX12-5] connector (male, female pair): This will allow the cable to be connected to the controller box.

[Arduino Nano]: https://www.banggood.com/3Pcs-ATmega328P-Arduino-Compatible-Nano-V3-Improved-Version-No-Cable-p-1047429.html?p=6K0304655364201407WG
[Electrolytic capacitor 10uF 25V]: https://www.banggood.com/100pcs-0_1uF-100uF-10V-50V-10Value-Electrolytic-Capacitor-Assortment-Kit-Set-p-1085572.html?p=6K0304655364201407WG
[Electrolytic capacitor 100uF 50V]:https://www.banggood.com/120pcs-15-Value-50V-Electrolytic-Capacitor-1UF-2200UF-Assortment-Kit-p-1073956.html?p=6K0304655364201407WG
[NTC 10K]: https://www.banggood.com/10pcs-NTC-Thermistor-Temperature-Sensor-10K-OHM-MF52-103-3435-1-p-932790.html?p=6K0304655364201407WG
[Resistor 1/4W 10K]: https://www.banggood.com/Wholesale-600pcs-30-Kinds-Value-Metal-Film-Resistor-Assorted-Kit-20pcs-Each-Value-p-53320.html?p=6K0304655364201407WG
[DC Power connector]: https://www.banggood.com/1Pc-DC-Power-Jack-Socket-Power-Outlet-Power-Socket-Female-Panel-Mount-Connector-p-1177643.html?p=6K0304655364201407WG

[28BYJ-48 stepper motor]: https://www.ebay.com/itm/192000139737
[A4988 stepper motor driver module]: https://www.banggood.com/3D-Printer-A4988-Reprap-Stepping-Stepper-Step-Motor-Driver-Module-p-88765.html?p=6K0304655364201407WG
[ULN2003 stepper motor driver module]: https://www.banggood.com/ULN2003-4-Phase-5-Wire-Stepper-Motor-Driver-Module-Board-XH-5P-Interface-p-1026882.html?p=6K0304655364201407WG

[GX12-4]: https://www.banggood.com/Wire-Panel-Connector-Aviation-Plug-L90-GX12-Circular-Connector-Socket-Plug-p-1193231.html?p=6K0304655364201407WG
[GX12-5]: https://www.banggood.com/Electronic-Soldering-Iron-GX12-5-Connector-T12-Aviation-Head-Mini-Aviation-Male-DIY-Soldering-Kits-p-1153612.html?p=6K0304655364201407WG

## 3D printed objects
There are two main blocks of objects, one being telescope specific will need to have a motor support, a motor cover and the mechanical drive. The second is common among all implementations and it is the controller box where all the electronics are kept safe from the environment.

Have a look at [Thing #2446069][2446069] to download the latest version and read the print instructions.

[2446069]: https://www.thingiverse.com/thing:2446069

### Controller box
![Image](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/object-focuser-box-bottom.png)
![Image](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/object-focuser-box-top.png)

Download objects:
- [Box bottom]
- [Box top]

[Box bottom]: https://www.thingiverse.com/download:4516795
[Box top]: https://www.thingiverse.com/download:4516796

### Skywatcher ED80
I have designed the required mechanical parts to attach a 28BYJ-48 stepper to the Skywatcher ED80 telescope.

![Image](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/object-focuser-ed80.png)
![Image](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/object-focuser-ed80-cover.png)
![Image](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/object-focuser-ed80-gear.png)
![Image](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/object-focuser-ed80-pinion.png)

Download objects:
- [Stepper support]
- [Stepper cover]
- [Pinion]
- [Gear]

[Stepper support]: https://www.thingiverse.com/download:4429682
[Stepper cover]: https://www.thingiverse.com/download:4429679
[Pinion]: https://www.thingiverse.com/download:4429680
[Gear]: https://www.thingiverse.com/download:4429683

## Schematic and wiring

### THIS IS AN IMPORTANT NOTICE
The wiring of the focuser has changed since commit [8375767], you should remove any physical conections between the **AREF** and the **3.3V** pin, not doing so may damage your board.

[8375767]: https://github.com/jbrazio/ardufocus/commit/8375767da8008305e1cb2a93d049970c49c1482d

### Uploading the firmware
A capacitor is used on the Arduino's reset line to keep it high during serial connect/disconnect which is mandatory for normal operation but in order to flash the firmware to the board the capacitor must be disconnected, otherwise you need to syncronize a manual push of the reset button when uploading to the board.


### PCB and Schematic
For the complete Eagle files have a look at the [PCB](./doc/pcb/) folder, as an alternative you may perfer to build the [Preforated Board](./doc/fzz/) version.
Please note that the dual motor setup is a work in progress and is not yet supoprted by the firmware.

![PCB Top View](https://github.com/jbrazio/ardufocus/blob/master/doc/pcb/ardufocus-top.png)

![PCB Bottom View](https://github.com/jbrazio/ardufocus/blob/master/doc/pcb/ardufocus-bottom.png)

### Alternative designs

J Archbold (ozarchie) made an alternative PCB design which is [available here](https://easyeda.com/editor#id=4bba61b315ce478189830a0f22f85c1a|f9187ab81f2a4f3ba5474865568245bf).

----

### A4988 driver with a Bipolar motor
![Bipolar](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/schematic-bipolar_bb.png)
[Download the bipolar schematic](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/schematic-bipolar_schem.png)

To use this configuration you must edit the `config.h` file:
``` cpp
// You should only enable *ONE* of the following drivers
// The ULN2003 shall be used with the unmodded version of 28BYJ-48 or any other
// Unipolar stepper motor. The A4988 driver should be used with Bipolar stepper
// motors or the modded version of the 28BYJ-48 (see the doc/ folder).
#define USE_A4988_DRIVER
//#define USE_ULN2003_DRIVER
```

#### Pinout
![Bipolar](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/pinout-bipolar.png)

----

### ULN2003 driver with a Unipolar motor
![Unipolar](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/schematic-unipolar_bb.png)
[Download the unipolar schematic](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/schematic-unipolar_schem.png)

To use this configuration you must edit the `config.h` file:
``` cpp
// You should only enable *ONE* of the following drivers
// The ULN2003 shall be used with the unmodded version of 28BYJ-48 or any other
// Unipolar stepper motor. The A4988 driver should be used with Bipolar stepper
// motors or the modded version of the 28BYJ-48 (see the doc/ folder).
//#define USE_A4988_DRIVER
#define USE_ULN2003_DRIVER
```

#### Pinout
![Unipolar](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/pinout-unipolar.png)

# Software
Ardufocus is 100% compatible with the Moonlite focuser protocol thus the original single-motor drivers can be used for manual control of the unit, if you need to integrate with SGP, APT or any other ASCOM based software then the universal ASCOM driver should be installed.

- [Moonlite's manual focus control](https://focuser.com/media/Downloads/MoonLite_Software/NonAscom/MoonliteSingleFocuser_v1.4.zip)
- [Moonlite's Universal ASCOM driver](https://focuser.com/media/Downloads/MoonLite_Software/Ascom/MoonLite%20DRO%20Setup.zip)

## Example configuration

### Sequence Generator Pro
This is an quick example how to setup SGP to work with Ardufocus.

![SGP Equipament Profile](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/sgp-focus-01.jpg)
![SGP Auto Focus settings](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/sgp-focus-02.jpg)
![SGP Filter settings](https://github.com/jbrazio/ardufocus/blob/master/doc/wiki/sgp-focus-03.jpg)


# License
[GNU GPLv3](./LICENSE).
