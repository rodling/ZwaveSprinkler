# ZwaveSprinkler
Sprinkler controler using one zwave dimmer to control multiple relays
.
This is a open source project to run sprinkler relays from my Z-Wave network.
A dimmer switch is use to drive a Opto-coulper that provides a pulse to a Arduino whose lenght is related to the dimmmer 
value set by my home automation system. The pulse is fed into Arduino and PulseIn reports the length. That lenght is filtered
and used to turn on just one relay.

More details will be added as I get time
