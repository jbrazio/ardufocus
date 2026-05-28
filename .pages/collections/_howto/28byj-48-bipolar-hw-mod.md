---
layout: default
keywords:
comments: false

tags: [ howto ]

title: 28BYJ-48 Bipolar Mod
description: >
  This tutorial will teach you how you can convert easily the Unipolar 28BYJ-48 stepper motor into
  a Bipolar one.

micro_nav: true
---

## Introduction
The [28BYJ-48] is out of the factory a Unipolar Stepper Motor which, nowadays, are not that common
anymore. Bipolar stepper motors are considered to be at least twice as efficient as an Unipolar motor
for the same amount of copper on the windings.

![Bipolar Stepper Motor]({{ '/assets/images/pages/diagram-bipolar-stepper.png' | relative_url }})
![Unipolar Stepper Motor]({{ '/assets/images/pages/diagram-unipolar-stepper.png' | relative_url }})

According to [research done by Jan Adriaensen] the [28BYJ-48] in an Unipolar configuration can produce
300g.cm of torque half stepped and 380g.cm when full stepped, after the Bipolar mod you can expect
the motor to produce **800g.cm** of torque, that's around 3x more efficient !

[research done by Jan Adriaensen]: http://www.jangeox.be/2013/10/stepper-motor-28byj-48_25.html

## Modding the motor
To perform the hardware mode is quite simple, just pry open the blue plastic cover having special
attention not to hurt yourself, there are two plastic tabs one on each side of the cover which need
to break free in order for the cover to come out. This may require a bit of force.

After the cover is out the PCB will be exposed, you'll see three main thick traces, take a hobby
knife and cut the middle trace by scrapping the PCB varnish and copper. In the end you should have
a 1-2mm gap on the trace.

![PCB mod]({{ '/assets/images/pages/diagram-238byj-48-bipolar-mod.png' | relative_url }})

The new motor windings will be **1** (Orange, Pink) and **2** (Yellow, Blue), you may cut the red
wire on unsolder it from the PCB.

| Color  | Winding | Side |
|--------|---------|------|
| Yellow | 2       | A    |
| Orange | 1       | A    |
| Red    | N/A     | N/A  |
| Pink   | 1       | B    |
| Blue   | 2       | B    |


## Driver
After performing the mod we can no longer use the [ULN2003] board to drive the stepper motor, we now
need a "H-Bridge" type of driver such as the [A4988] or the [DRV8825].

![A4988 schematic]({{ '/assets/images/pages/diagram-a4988.png' | relative_url }})

[A4988]: https://www.banggood.com/3Pcs-3D-Printer-A4988-Reprap-Stepping-Stepper-Step-Motor-Driver-Module-p-967057.html?p=6K0304655364201407WG
[DRV8825]: https://www.banggood.com/3Pcs-3D-Printer-Stepstick-DRV8825-Stepper-Driver-Reprap-4-Layer-PCB-p-1052018.html?p=6K0304655364201407WG
[ULN2003]: https://www.banggood.com/3pcs-ULN2003-Four-phase-Five-wire-Driver-Board-Electroincs-Stepper-Motor-Driver-Board-p-1352783.html?p=6K0304655364201407WG
[28BYJ-48]: https://www.banggood.com/KittenBot-2Pcs-28BYJ-48-5V-DC-Stepper-Motor-4-Phase-5-Line-for-Smart-Robot-DIY-Part-p-1287817.html?p=6K0304655364201407WG
