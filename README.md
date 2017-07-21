# Ardufocus
Ardufocus is a full open source [Moonlite][moonlite] focuser clone.

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

# Contributing
## Contributing guidelines
We’d love you to help us improve this project. To help us keep this collection high quality, we
request that contributions adhere to the following guidelines.

- **Provide links to documentation** supporting the change you’re making.
  If documentation isn’t available to support your change, do the best you can to explain what
  you're aiming for.

- **Explain why you’re making a change**. Even if it seems self-evident, please take a sentence or
  two to tell us why your change or addition should happen. It’s especially helpful to articulate
  why this change applies to *everyone* who works with the applicable technology, rather than just
  you or your team.

- **One change/feature per pull request**. This helps keep pull requests and feedback focused on a
  specific topic or improvement.

In general, the more you can do to help us understand the change you’re making, the more likely
we’ll be to accept your contribution quickly.

## Contributing workflow
Here’s how we suggest you go about proposing a change to this project:

1. [Fork this project][fork] to your account.
2. [Create a branch][branch] for the change you intend to make.
3. Make your changes to your fork.
4. [Send a pull request][pr] from your fork’s branch to our `master` branch.

Using the web-based interface to make changes is fine too, and will help you by automatically
forking the project and prompting to send a pull request too.

[fork]: https://github.com/jbrazio/Ardufocus#fork-destination-box
[branch]: https://github.com/jbrazio/Ardufocus/branches
[pr]: https://github.com/jbrazio/Ardufocus/pulls


# License
[GNU GPLv3](./LICENSE).
