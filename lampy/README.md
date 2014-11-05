LampY
=====

LampY is the software that is running on the arduino directly.  It handles receiving input
from the rotary encoder and push button, and switching modes perdiocally or via the dial.

Modes
=====

Currently Existing Modes:
------------------------

*off* - turns all the leds off, only available on the physical controls

*blue* - turns all the leds blue

*red* - turns all the leds red

*green* - turns all the leds green

*police* - alternating red/blue leds that switch state every 500ms

*randomdecay* - 25% chance a random led will turn on, and leds average with neighbors with a small
decay value. [video](http://instagram.com/p/vAZ_UmNppY)

*colorpalette* - cycles to a new cycling color palette every 15 seconds. [video](http://instagram.com/p/vBbWX-tpux/)
