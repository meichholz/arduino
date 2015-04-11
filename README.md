# arduino
Sketches, library code  and glue stuff for Arduino deployment

## Environment

For now, I use an Arduino Uni clone, and the original Arduino 1.6.3 tool.

## Usage with Gentoo

The Arduino ebuild in portage is fairly outdated and - worse - crashes.

The "Linux-32-bit"-Tarball, however, can be dropped anywhere (e.g. /opt), and
the binary runs straight away (/opt/arduino-1.6.3/arduino).

## Vim knows "arduino"

nuff said. file type detection is a miracle.

## Rough Estimates

* `digitalWrite()` takes approx. 5 microseconds, or roughly 80 clock cycles at 16 MHz.
* The `loop()` accounts for some 1 micro second itself.
* practically the limit for bitbanging *fairly* below 100 kHz.


