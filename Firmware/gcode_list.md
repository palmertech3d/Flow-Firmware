# List of gcode commands for Flow Firmware
The Flow Firmware is designed around a custom flavor of gcode.

## Commands

REMINDER: When sending gcode commands, a carriage return (enter, ASCII 13) MUST be sent in order for the firmware to execute the command.

### G1 M\<motor\> S\<speed\>
Starts the motor given by M at speed S. Refer to motor list. If level winder motor selected, speed is not necessary. If no motor argument is given, the command is rejected. Speeds can only be given as integer values.

### G2 M\<motor\>
Stops the motor given by int M. Refer to motor list. If no motor argument is given, the command is rejected.

### G28
Homes the level winder.

### M104 B\<temp\>
Sets the extruder temp to B in Celsius. No argument will turn off the hotend.

### M106
Toggles the cooling fans on/off

### M303
Activates PID auto tune for the hotend

## M503
Reports current settings

## Motor List
- 0 - Extruder motor
- 1 - Rollers motor
- 2 - Level winder motor
- 3 - Winder motor

## Notes for Flow gcode developers
- Commands can be numbered 0-999 (must be integers)
- Commands can be created with up to three arguments, which can themselves have numbered parameters from 0-32767 (must be integers)
