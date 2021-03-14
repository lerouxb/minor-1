#!/bin/sh

set -x

arduino-cli compile -b "megaTinyCore:megaavr:atxy6:bodmode=disabled,bodvoltage=1v8,chip=3216,clock=20internal,eesave=enable,millis=enabled,resetpin=UPDI,serialevent=no,startuptime=8,uartvoltage=5v" -e
