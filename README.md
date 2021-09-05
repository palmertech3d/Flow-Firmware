# Flow Firmware

Repository for code for the Flow extruder.

## General Goals/Status

- [x] Extrude filament
- [ ] Measure filament diameter with an external sensor
- [ ] Extrude a consistent filament diameter
- [ ] Spool filament in a pretty way while extruding

## Requirements

- Read temperature
- Read filament diameter
- Control >=3 motor speeds at once
- Control speeds based on temperatures, filaments, possibly other parameters
- Be configurable at a high level with decent granularity
- Control various parameters with PID (and autotunable) control loops.
- Safety features, like temperature and motor current are monitored.
- Motor speeds should be configurable using real-world units
- Configurations like extrusion rate, temperatures, diameter should be configured with a single command
- Constant reporting of certain parameters to whatever is controlling the machine
- Parameters can change mid-extrusion
- Configuration can be stored in EEPROM
- Multiple temperature zones should be controllable
- Monitor stall of extruder motor
- Heater control and thermal runaway

## General Planning

- Extruder will use M8xx commands to do everything that only an extruder needs to do.
- There will be some sort of top-level motion manager, which handles closed loop control while extruding.
