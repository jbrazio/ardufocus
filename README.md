# Ardufocus
Ardufocus is a full open source [Moonlite][moonlite] compatible focuser.

The source is still under heavy development so things move around a bit.

[moonlite]: https://www.focuser.com/


# Motivation
After buying the CCD, filter wheel and filters I was broke but still wanted to have an automatic
focuser.


# Design Goals

## 1. Moonlite compatible
This was a very important part of the design as I didn't want to spend time and effort dealing with
ASCOM and INDI drivers, the Moonlite focuser is a well known, reputable rock solid focuser. The
serial protocol used by them was easily reversed engineered (plain ASCII) and most of it was already
documented on the Internet.

## 2. Cheap
Another big point was to made it as cheap as possible recurring to as few parts as needed. That's
the reason why the 28BYJ-48 stepper motor was chosen, out-of-the-box using the ULN2003 gives you a
really cheap (less than 2€) focuser for medium loads (380gcm). If you require the focuser to driver
heavier loads (800gcm) then the motor itself can be modded into a Bipolar stepper motor and driven
by the A4988 step stick which will cost you less than 1€.

## 3. Builder friendly
Using off-the-shelf components such as the Arduino Nano and easily available parts Ardufocus is
aimed to be build by anyone with a soldering iron and some patience, no degree in electronics
required.


# Hardware
It was built on top of a standard ATmega 328 Arduino such as UNO, Pro or Nano; currently it does not
support the Mega or any other ARM based board.

## BOM

- 1x Arduino Nano
- 1x A4988 Stepper Motor Driver Module
- 1x Electrolytic capacitor 10uF
- 1x NTC 10K 5%
- 1x Resistor 1/4W 10K
- 1x DC Power connector (male, female pair)
- 1x DB9 connector (male, female pair)
- 1x 28BYJ-48 Stepper motor

## 3D printed parts
To download and print instructions for the 3D printed parts have a look at the [Thing #2446069][2446069].

[2446069]: https://www.thingiverse.com/thing:2446069

![ED80 gear](https://github.com/jbrazio/ardufocus/blob/master/doc/object-focuser-ed80.png)
![28byj-48 pinion](https://github.com/jbrazio/ardufocus/blob/master/doc/object-focuser-28byj-48.png)
![Skywatcher focuser part](https://github.com/jbrazio/ardufocus/blob/master/doc/object-focuser-skywatcher-v4.png)

## A4988 driver with a Bipolar motor
![Bipolar breadboard schematic](https://github.com/jbrazio/ardufocus/blob/master/doc/schematic-bipolar_bb.jpg)
[Download the schematic](https://github.com/jbrazio/ardufocus/blob/master/doc/schematic-bipolar_schem.jpg)

### Configuration

#### ardufocus.h
``` cpp
// You should only enable *ONE* of the following drivers
// The ULN2003 shall be used with the unmodded version of 28BYJ-48 or any other
// Unipolar stepper motor. The A4988 driver should be used with Bipolar stepper
// motors or the modded version of the 28BYJ-48 (see the doc/ folder).
#define USE_A4988_DRIVER
//#define USE_ULN2003_DRIVER
```

#### Sequence Generator Pro
![SGP Equipament Profile](https://github.com/jbrazio/ardufocus/blob/master/doc/sgp-focus-01.jpg)
![SGP Auto Focus settings](https://github.com/jbrazio/ardufocus/blob/master/doc/sgp-focus-02.jpg)
![SGP Filter settings](https://github.com/jbrazio/ardufocus/blob/master/doc/sgp-focus-03.jpg)


# License
[GNU GPLv3](./LICENSE).
