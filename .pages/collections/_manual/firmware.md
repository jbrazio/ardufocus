---
layout: default
keywords:
comments: false

tags: [ manual ]

title: Firmware
description: >
  In this section you'll find all the information you need to know to build and flash
  Ardufocus into your Arduino board.

micro_nav: true

page_nav:
    prev:
        content: Build instructions
        url: '/manual/build-instructions/'

    next:
        content: ASCOM Driver
        url: '/manual/ascom-driver/'
---

## Introduction

If you have built the Moonlite compatible version then a capacitor is used on the Arduino's reset
line to keep it high during serial connect/disconnect which is mandatory for normal operation but
in order to flash the firmware to the board the capacitor must be disconnected, otherwise you need
to synchronize a manual push of the reset button when uploading to the board. Once again, this is
only necessary **if you have built the Moonlite compatible version**.

To enable a feature remove the comment tag `//` from the start of the line.
To disable a feature add  the comment tag `//` to the start of the line or delete the entire line.

```c++
// The option bellow is ENABLED
#define COOL_FEATURE_6000

// The option bellow is DISABLED
//#define COOL_FEATURE_6000
```
## Config options

### Overall
#### Persistent memory
Enable auto save of the focuser position uppon finishing each move. When
active the focuser will remember between power cycles the exact position
where it was.

```c++
#define USE_EEPROM
```

#### Remote reset
Remote reset is a non standard feature added to the Moonlite protocol which
allows you to reset the micro controller inside the focuser. Please test
this feature on your bench before deploying on the field. For this feature
to work the default boot loader may need to be changed otherwise the uC will
enter into an infinite loop state.

```c++
#define ENABLE_REMOTE_RESET
```

#### Serial DTR reset <sup>(beta)</sup>
Enable a subset of commands to control the status of the DTR auto-reset
feature on the Arduino boards. By default Ardufocus uses a cap between the
reset pin and ground to prevent the DTR signal to reset the board, which
happens every time a new serial connection is open to the board.
By having the cap between the reset pin and another board pin, enabling
this feature allows the DTR auto-reset to be controlled. This is useful
for firmware uploads, if the DTR auto-reset is disabled the user must
manually press the reset button when uploading new firmware.

```c++
#define ENABLE_DTR_RESET
```

This is the board pin which controls the enable/disable of the feature.

```c++
#define DTR_RESET_PINOUT 15
```

Activating this option will enable high resolution counters (32-bit) thus
becoming incompatible with the standard Moonlite protocol. You should enable
this if using a gearbox or having a millimetre threaded rod on the drive
mechanism.
```c++
#define HIGH_RESOLUTION_MODE
```

### Motor configuration

#### Driver
You should only enable *ONE* of the following drivers
The ULN2003 shall be used with the unmodded version of 28BYJ-48 or any other
Unipolar stepper motor. The A4988 driver should be used with Bipolar stepper
motors or the [modded version] of the 28BYJ-48, for more information see the
[hardware] section.

[modded version]: {{ '/howto/28BYJ-48-bipolar-hw-mod/' | relative_url }}
[hardware]: {{ '/manual/hardware/#stepper-motor' | relative_url }}

```c++
#define MOTOR1_USE_A4988_DRIVER
```

```c++
#define MOTOR1_USE_DRV8825_DRIVER
```

```c++
#define MOTOR1_USE_ULN2003_DRIVER
```

#### Pinout
Tells the firmware which pins should be used to control the driver.

```c++
//                    MS1, MS2, MS3, SLEEP, STEP, DIR
#define MOTOR1_PINOUT  12,  11,  10,     8,    7,   6
```

#### Direction
Activate the following directive if you'd like to invert the motor rotation
changing the focus direction.

```c++
#define MOTOR1_INVERT_DIRECTION
```

#### Sleep when idle
When active Ardufocus will cut the stepper motor current when idle, in theory
this could lead to less accuracy between movements but will keep the motor
cool. When disabling this flag make sure your motor does not overheat.

```c++
#define MOTOR1_SLEEP_WHEN_IDLE
```

#### Sleep timeout
When the previous directive is active, allows you to control the amount of
time the driver will wait, after stopping, before powering down the motor.
The idea behind is the system needs full accuracy between the AF point
sequence but can be powered down between AF runs. Thus you should set this
to wait a bit more than the time your system needs to take and measure each
AF point.


```c++
#define MOTOR1_SLEEP_TIMEOUT 15
```

#### Speed profile
Specify a custom speed profile for you motor model and driver combo.
The units are in steps/sec.

Usually a motor such as the 28BYJ-48 need lower speed limits:
 - Max speed: 250
 - Min speed: 25

NEMA17 motors allow you to use higher speed limits:
 - Max speed: 1000
 - Min speed: 250

```c++
#define MOTOR1_MAX_SPEED 150
#define MOTOR1_MIN_SPEED  25
```

#### Acceleration profile
When active Ardufocus will apply the selected acceleration profile to the
motor's speed. The objective is to help the system cope with heavier loads
such as FF + FW + CCD combos.

```c++
#define USE_LINEAR_ACCEL
```

```c++
#define USE_TRAPEZOID_ACCEL
```

```c++
#define USE_SMOOTHSTEP_ACCEL
```

The acceleration profile, independent of the method used, has at least two
main periods: the ramp-up period when the motor is gaining speed and the
ramp-down period when the motor is losing speed. This setting controls the
duration of each one of those periods, the default value is 250 steps for
each period if left undefined.

```c++
#define ACCEL_DURATION 250
```

When acceleration control is active this setting controls the minimum
required number of steps on a movement for the algorithm to kick in. Any
movement with less steps than this will be done at minimum speed without
any acceleration control. The default value is 10 steps of left undefined.

```c++
#define ACCEL_MIN_STEPS 10
```

### Temperature sensor

<div class="callout callout--warning">
  <p><strong>TODO</strong>Explain howto fine tune the values bellow.</p>
</div>

```c++
#define NTC_ADC_CHANNEL          0
#define NTC_NOMINAL_TEMP      25.0F
#define NTC_BCOEFFICIENT    3950.0F
#define NTC_NOMINAL_VAL    10000.0F
#define NTC_RESISTOR_VAL   10000.0F
```

Minimum value to be considered a valid reading (xxxC)

```c++
#define NTC_MIN_RAW_VALUE  50
```

Maximum value to be considered a valid reading (-23C)

```c++
#define NTC_MAX_RAW_VALUE 950
```

According to the Moonlite protocol the temperature probe should only be read
when the command `:C#` is received but some applications, such as SGP, seems
not to respect this and only call the get temperature command `:GT#` which
means the temperature will never get updated and the last read value is always
returned, either it is valid or not. Enabling the following option will force
the temperature gathering process on every temperature read command.

```c++
#define START_TEMP_CONVERSION_ON_EVERY_GET
```
