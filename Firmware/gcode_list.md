# List of gcode commands for Flow Firmware
The Flow Firmware is designed around a custom flavor of gcode.

## Commands

### G1 M\<motor\> S\<speed\>
Starts the motor given by int M at speed S. Refer to motor list. If level winder motor is started, speed is not necessary. If no motor argument is given, the command is rejected.

### G2 M\<motor\>
Stops the motor given by int M. Refer to motor list. If no motor argument is given, the command is rejected.

### G28
Homes the level winder.

### M104 B\<temp\>
Sets the extruder temp to B in celsius. No argument will turn off the hotend.

### M106 S\<speed\>
Sets the cooling fan speed to speed S. No argument will turn off the fan.


## Motor List
- 0 - Extruder motor
- 1 - Rollers motor
- 2 - Level winder motor
- 3 - Winder motor

## Notes for Flow gcode developers
- Commands can be numbered 0-999
- Commands can be created with up to three arguments, which can themselves have numbered parameters from 0-99999
