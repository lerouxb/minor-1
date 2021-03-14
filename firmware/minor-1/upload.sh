#!/bin/sh

set -x

# pyupdi is faster than megaTinyCore's UPDI uploader (which is actually Microchip's uploader) but apparently it is not "production ready". Gives much faster development cycle times, so that's what I'm using during development
pyupdi -d tiny3216 -c /dev/cu.usbserial-DA01L36P -f build/megaTinyCore.megaavr.atxy6/minor-1.ino.hex -b 225000
pyupdi -d tiny3216 -c /dev/cu.usbserial-DA01L36P -r
