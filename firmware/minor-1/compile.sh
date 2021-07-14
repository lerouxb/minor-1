#!/bin/sh

set -x

arduino-cli compile -b "megaTinyCore:megaavr:atxy4:bodmode=disabled,bodvoltage=2v6,chip=1614,clock=10internal,eesave=enable,millis=enabled,resetpin=UPDI,serialevent=no,startuptime=8,uartvoltage=5v" -e
