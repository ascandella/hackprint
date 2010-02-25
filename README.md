HackPrint: A basic framework for controlling a door strike with Arduino
-----------------------------------------------------------------------

This project written for a project involving an electic door strike and USB fingerprint reader.

## Requirements

* An Arduino or similar serial-compatible programmable controller.
* For fingerprint reading, a *nix compatible with libfprint (http://www.reactivated.net/fprint/wiki/Libfprint) is needed.
* Some basic electronic components (TIP 12X series transistor in particular)
* An external power supply capable of supplying the necessary current to your strike (I used a 5V/1.2A supply from monoprice: http://www.monoprice.com/products/product.asp?c_id=104&cp_id=10420&cs_id=1042001&p_id=2220&seq=1&format=2)
* A breadboard and some wires
* Ruby, as well as the 'serialport' and 'daemons', plus 'yaml' for loading configuration options

## Status

* This project is in early alpha. I'll update as I add more features and make it more robust.

## Usage

* Modify the hackprint.yml to match your USB and fifo settings.
* Make a FIFO queue with mkfifo <pipe name>.
* Load your Arduino with the provided sketch. It simply listens for a digit on the serial line, then sends a digital HIGH output on a pin for that many seconds.
* Connect this digital output with a 1 Kohm resistor to the "base" pin of a transistor capable of handling your strike's power needs (I used a TIP 121).
* Connect the strike to the +5V of the external power supply, then the other lead to the "collect" pin of the transistor.
* Connect the emitter to the ground of the power supply, also connect the ground of the Arduino to the ground rail of the power supply.
* Start the door-daemon, and try a "./unlock"
