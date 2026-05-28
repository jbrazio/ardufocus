---
layout: default
keywords:
comments: false

tags: [ manual ]

title: Getting started
description: >
  In this section you'll find basic information about Ardufocus. If you're first time user then
  you should read Getting Started section first.

micro_nav: true

page_nav:
    next:
        content: Hardware
        url: '/manual/hardware/'
---

## Description
Ardufocus is a stepper motor controller designed from the ground up for astronomy usage, our aim is
to enable you to have the most accurate Open Source focus controller for a fraction of the price of
the commercial ones.

Ardufocus is **Open Source Software** and **Open Source Hardware** which means you have access to all the
firmware, software and hardware needed to build or modify the platform according to your preferences.

Using off-the-shelf components such as the Arduino Nano and easily available parts Ardufocus is
aimed to be build by anyone with a soldering iron and some patience, no degree in electronics required.

Another big point was to made it as cheap as possible recurring to as few parts as needed. That's
the reason why the 28BYJ-48 stepper motor was chosen, out-of-the-box using the ULN2003 gives you a
really cheap focuser for medium loads. If you require the focuser to driver heavier loads then the
motor itself can be physically modded into a Bipolar stepper motor and driven by the A4988 step
stick which will cost you in overall less than $10.

The project started by being a Moonlite protocol compatible focuser but in order to leverage some of
the new features such as High Resolution (32-bit integers), I am currently developing it's own custom
set of ASCOM IFocuserV3 drivers.


## Contribute
Ardufocus is a *one-man-army* kind of a project which I develop on my free time, if you use it please
help out by [spreading the love](spam) or, if you prefer to assure it's long-term continuity consider
one of the options bellow:

[![Donate Patreon](https://img.shields.io/badge/Donate-Patreon-blue.svg?style=for-the-badge)](https://www.patreon.com/join/jbrazio)
[![Donate PayPal](https://img.shields.io/badge/Donate-Paypal-blue.svg?style=for-the-badge)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D5XZ7QFLP8LXE)

[spam]: #


## Key features
- Up to two independent steppers with only one controller
- Stepper position is kept between power cycles
- High resolution mode using 32-bit counters
- Multiple stepper drivers are supported
- Multiple acceleration profiles

### Additional features
- Moonlite protocol compatible
- Remote soft-reset of the microcontroller
- Enable/Disable of thee automatic reset on serial connection
- Allow motor direction inversion by software
- Automatic enter sleep mode when idle with use defined timeout
- User defined min, max and acceleration speeds
- Fine tune of NTC parameters


## Source code
Each Ardufocus module is located on it's own Github public repository:
- [Ardufocus firmware](https://github.com/jbrazio/ardufocus)
- [Ardufocus schematics](https://github.com/jbrazio/ardufocus-schematics)
- [Ardufocus ASCOM IFocuserV3 Driver](https://github.com/jbrazio/ardufocus-ascom)


## License
Ardufocus is licensed under [GNU GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html). <br />
Ardufocus PCB schematics are licensed under [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).
