# Musical Arduino

Some test sketches for experimenting with (many) buttons and MIDI output with an Arduino Uno.

Below are some random notes taken during research.

## Pinout for the XD100 modwheel

Disclaimer: This was just found through trial and error, but seems to work.

- Purple: seems unimportant
  - (the one pin separated from the others)
- Green: Voltage output
- Blue: Voltage input
- Yellow: Ground

## PCF8574 I/O expander

- Texas Instruments datasheet: <https://www.ti.com/lit/ds/symlink/pcf8574.pdf>
- Mouser datasheet (much nicer formatting imho): <https://www.mouser.co.uk/datasheet/2/302/PCF8574_PCF8574A-3139141.pdf>
- Adafruit documentation: <https://learn.adafruit.com/adafruit-pcf8574> (we love Adafruit)

### I2C address

Its default I2C address is `0x20` (plus an extra bit to determine if it's a read or write operation, but we don't need to worry about that).

Note that the "A" variant of the board uses a different set of addresses. We are using the non-A variant (from [Amazon](https://www.amazon.co.uk/dp/B07DK7VPPL)).

The address can be changed by pulling the `A0`, `A1`, and/or `A2` pins up to V<sub>DD</sub> (i.e. the operational voltage (which can be 2.5-6.0 V)). See [Page 5 of the Mouser datasheet](https://www.mouser.co.uk/datasheet/2/302/PCF8574_PCF8574A-3139141.pdf#page=5) for details!

### Libraries

We have a bunch of options, it seems.

- <https://github.com/xreef/PCF8574_library/> (what we are using at the moment)
- <https://github.com/RobTillaart/PCF8574>
- <https://github.com/adafruit/Adafruit_PCF8574>
