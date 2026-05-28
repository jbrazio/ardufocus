---
layout: default
keywords:
comments: false

tags: [ manual ]

title: Build instructions
description: >
  It's time to get your hands dirty, learn which components you require for this
  project and learn howto build one for yourself.

micro_nav: true

page_nav:
    prev:
        content: Hardware
        url: '/manual/hardware/'

    next:
        content: Firmware
        url: '/manual/firmware/'
---

## Motor Connections
### Unipolar

The Unipolar motor connection is straight forward because both the motor and the **ULN2003** board usually
come with a **JST-XH** connector. But nevertheless the table bellow lists the correct connections between
the two components.

| Color  | Winding | Side | ULN2003 |
|--------|---------|------|---------|
| Blue   | 2       | B    | A       |
| Pink   | 1       | B    | B       |
| Yellow | 2       | A    | C       |
| Orange | 1       | A    | D       |
| Red    | N/A     | N/A  | +12V    |

![28BYJ-48 and ULN2003]({{ '/assets/images/pages/diagram-238byj-48-uln2003.png' | relative_url }})

Remember to enable `#define MOTOR1_USE_ULN2003_DRIVER` on your `config.h` file, for more information
check the [firmware page].

### Bipolar
Bipolar stepper motors are only supported by Ardufocus when using a step stick, due to the multiple
motors and connectors available on the market it's hard to specify which one will be the correct one
in use at your configuration but nevertheless there are some tricks that will ease your job finding
the correct connections.


#### NEMA17
All NEMA motors have only four wires, to find the correct windings just grab your multimeter and set
it for resistance measurement. Label each one of the wires from 1 to 4 and then take any random two
and measure the resistance between them, either you'll get a reading or a open circuit. Keep
measuring different wires until you get the two pairs that give you a reading between them and that's
your two coils.

Let's assume wire `1 and 3` form a pair and `2 and 4` form the other pair, to connect it to the step
stick connect `wire 1` to `2B`, `wire 3` to `2A`, `wire 2` to `1B` and `wire 4` to `1A`.


#### 28BYJ-48

The table bellow shows the connections between the **28BYJ-48** motor and the **A4988** step stick.

| Color  | Winding | Side |
|--------|---------|------|
| Blue   | 2       | B    |
| Yellow | 2       | A    |
| Orange | 1       | A    |
| Pink   | 1       | B    |
| Red    | N/A     | N/A  |

![28BYJ-48 and ULN2003]({{ '/assets/images/pages/diagram-238byj-48-a4988.png' | relative_url }})

Remember to enable `#define MOTOR1_USE_A4988_DRIVER` or `#define MOTOR1_USE_DRV8825_DRIVER` on your
`config.h` file, for more information check the [firmware page].


[firmware page]: {{ '/manual/firmware/#motor-configuration' | relative_url }}

## Breadboard

### 28BYJ-48 with ULN2003 driver
This build is the cheapest focuser you can have not only due it's bare bones approach but also due to
the it's minimal BOM, part count and availability of each item. **Use this configuration only with
unmodded 28BYJ-48 stepper motors**.

[![28BYJ-48 and ULN2003]({{ '/assets/images/pages/schematic-breadboard-unipolar.png' | relative_url }})]({{ '/assets/images/pages/schematic-breadboard-unipolar.png' | absolute_url }})

#### BOM
Check the required BOM on the table bellow. The **100nF** capacitor is only required if you intend
to use Ardufocus in a Moonlite compatibility mode i.e. use Moonlite's ASCOM drivers, if you intend
to use Ardufocus ASCOM drivers then this capacitor is not required.

| Description             | Specs        | Qt |
|-------------------------|--------------|----|
| 28BYJ-48 stepper motor  | 12V          | 1  |
| Arduino                 | Nano 3.0     | 1  |
| Driver break out board  | ULN2003      | 1  |
| Electrolytic Capacitor  | 100nF >12V   | 1  |
| NTC                     | 10K          | 1  |
| Resistor                | 10KΩ 5% 1/4W | 1  |

#### Settings
Edit your `config.h` file and set make sure the options bellow are enabled.

```c++
#define MOTOR1_USE_ULN2003_DRIVER
#define MOTOR1_PINOUT 12, 11, 10, 9
```

#### Pinout

| PIN   | Motor #1 | Motor #2 |
|-------|----------|----------|
| IN1   | D12      | D08      |
| IN2   | D11      | D07      |
| IN3   | D10      | D06      |
| IN4   | D09      | D05      |

### 28BYJ-48 with A4988 driver
This solution is not so much more expensive nor complicated but will give you much more torque from
the exact same motor when compared with the Unipolar build. **Use this configuration only with
[modded 28BYJ-48 stepper motors]({{ '/howto/28byj-48-bipolar-hw-mod/' | relative_url }}) or with
NEMA17 stepper motors**. The example bellow shows a modded 28BYJ-48 but the connections for a
NEMA17 are exactly the same.

[![28BYJ-48 and A4988]({{ '/assets/images/pages/schematic-breadboard-bipolar.png' | relative_url }})]({{ '/assets/images/pages/schematic-breadboard-bipolar.png' | absolute_url }})


#### BOM
Check the required BOM on the table bellow. The **100nF** capacitor is only required if you intend
to use Ardufocus in a Moonlite compatibility mode i.e. use Moonlite's ASCOM drivers, if you intend
to use Ardufocus ASCOM drivers then this capacitor is not required.

| Description             | Specs        | Qt |
|-------------------------|--------------|----|
| 28BYJ-48 stepper motor  | 12V          | 1  |
| Arduino                 | Nano 3.0     | 1  |
| Electrolytic Capacitor  | 100nF >12V   | 1  |
| NTC                     | 10K          | 1  |
| Resistor                | 10KΩ 5% 1/4W | 1  |
| Step Stick              | A4988        | 1  |

#### Settings
Edit your `config.h` file and set make sure the options bellow are enabled.

```c++
#define MOTOR1_USE_A4988_DRIVER
#define MOTOR1_PINOUT 12, 11, 10, 9, 8, 7
```

#### Pinout

| PIN   | Motor #1 | Motor #2 |
|-------|----------|----------|
| MS1   | D12      | A2       |
| MS2   | D11      | D6       |
| MS3   | D10      | D5       |
| RESET | NC       | NC       |
| SLEEP | D9       | D4       |
| STEP  | D8       | D3       |
| DIR   | D7       | D2       |

## PCB

<div class="callout callout--warning">
  <p><strong>TODO</strong> This section needs further improvement.</p>
</div>

![PCB Top](https://github.com/jbrazio/ardufocus-schematics/raw/master/ardufocus/ardufocus-top.png)

![PCB Botttom](https://github.com/jbrazio/ardufocus-schematics/raw/master/ardufocus/ardufocus-bottom.png)

#### BOM

#### Settings

#### Pinout

| PIN   | Motor #1 | Motor #2 |
|-------|----------|----------|
| IN1   | D12      | D08      |
| IN2   | D11      | D07      |
| IN3   | D10      | D06      |
| IN4   | D09      | D05      |

