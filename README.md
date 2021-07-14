# minor-1

Credit card sized capacitive touch toy piano.

Just the absolute minimum:

* QT60140 16 key touch sensor
* ATtiny1614 microcontroller
* Piezo transducer
* MCP6001R op-amp as buffer for the piezo
* CR2032 coin cell battery
* SPDT power switch
* 13 keys, octave +/- and change sound touch buttons
* Currently unpopulated footprint for NOR flash
* P-channel MOSFET for completely cutting power

This uses the attiny's 8-bit DAC at 25khz to play notes. The microcontroller can shut down power to the op-amp/piezo and the flash chip when not in use. It also sleeps until a touch button is pressed or until there's activity on the serial port.

There's a simple browser page to log the button presses, read status and read/write the setups. Useful for configurating the touch controller. Could be useful in future for programming the NOR flash or configuring the sounds.

Still very much a work in progress, but it can already play tunes.
