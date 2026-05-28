---
layout: default
keywords:
comments: false

tags: [ howto ]

title: A4988 motor current tuning
description: >
  This tutorial will teach you how to fine tune the stepper motor current using the built-in
  potentiometer, a screw driver and a multimeter.

micro_nav: true
---

## Introduction
The A4988 stepper driver, originally made by Pololu, will interrupt the current to the motor for a
little while if the it gets too hot. If you haven't tuned your motor current at all, it may happen
that the motor doesn't move correctly and starts pulsating, it will stop as soon as you reduce the
current by rotating the pot.. this is clearly a sign you need to fine tune the motor current.

## Reference voltage
The A4988 allows you to set a target current anywhere between some mA up to a bit less than 2A, this
is accomplished by adjusting what is called the `Vref` (Reference Voltage) when turning the pot on a
clockwise direction the `Vref` voltage will increase and decrease when rotating it counterclockwise.

The actual value of `Vref` can be calculated using the formula:<br />
`Vref = Imax * 8 * Rs`

<div class="callout callout--info">
  <p><strong>DRV8825</strong> The equation for calculating <code class="highlighter-rouge">Vref</code>
  for this board is <code class="highlighter-rouge">Vref = Imax /2</code> when having a <b>0.1</b> Ω
  sensing resistor.</p>
</div>

## Sensing resistor
But first we need to find out the proper value of the current sensing resistor `Rs` as it's exact
value is dependent of the A4988 board manufacturer. By looking at the number marked on top of `R5`
you may use a tool such as [this one] to find out it's exact value in Ohms.
If you dont find `R5` search for `S1` or `S2`.

![A4988 resistor value]({{ '/assets/images/pages/a4988-vref-resistor.png' | relative_url }})

Original Pololu boards have a **0.05** Ω resistor while most generic brands use **0.1** Ω.

Let's take the **0.1** Ω sensing resistor value as an example, just replace `Rs` on the formula like
so `Vref` using `Vref = Imax * 8 * 0.1`.

[this one]: https://www.hobby-hour.com/electronics/smdcalc.php

## A word about heat
The step stick is such a a small board, this is both it's strongest and weakest point. The small
form factor makes it an smart choise for projects where overall bulk size matters but also makes it
run hot as chilli. Some vendors provide you a stick-in heatsink, the performance improvement using
it is rather questionable so your best friend is de-rating the motor current.

If you have a motor which max current is spec at 2A, you should ran it at around 70% to 80% of that,
this will not only make your step stick happier but also the motor will run cooler.

## Adjusting the current
Let's take a 1.5A motor which we want to run at 80% of its rating:<br />
`Imax = 1.5 * 0.8 <=> Imax = 1.2A`

Let's also take a generic step stick having the sensing resistor (`Rs`) value of 0.1 Ω, take the
initial formula `Vref = Imax * 8 * Rs` and replace `Imax` and `Rs`.

`Vref = 1.2A * 8 * 0.1 <=> Vref = 0.96`

Using a multimeter measure the voltage across the top of the potentiometer head and any ground point
on the board.

![A4988 resistor value]({{ '/assets/images/pages/a4988-vref-test.png' | relative_url }})

Adjust the pot until you measure, in this example, 0.96V.
Now you have your step stick properly adjusted for your motor's current.

<sup>All images on this document are copyright reprap.org and licensed under the GNU Free
Documentation License.</sup>
