#!/bin/sh

set -x


pyupdi -d tiny1614 -c /dev/cu.usbserial-0001 -f build/megaTinyCore.megaavr.atxy4/minor-1.ino.hex -b 225000
pyupdi -d tiny1614 -c /dev/cu.usbserial-0001 -r
