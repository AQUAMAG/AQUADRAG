# Top Menu:
- [Continuous Drive](#continuous-drive) 
- [Move Set Distance](#move-set-distance) 
- [Go Home](#go-home) 
- [Reset Home](#reset-home)
## Continuous Drive:
- Inputs:
	- Speed in mm/sec
	- direction (fwd/back)
- Function:
	- will take a speed value and direction then set continuous motion until a Stop command is sent or the safety stop length is hit
	- Safety stop length will compute at ~3/4 of rail length and software will determine based on step/rev setting

## Move Set Distance:
 - Inputs:
	 - distance (mm)
	- Speed (mm/sec)
	- direction (fwd/back)
	- option to override safety stop
- Function:
	- will move the motor in the set speed for the set distance 
	- if safety stop is overridden allow whatever movement
## Go Home
- Inputs:
	- None
- Function:
	- Returns motor to the 0 setpoint that is either created when the system boots or when it is reset
## Reset Home:
- User Selects from the following:
	- Set home to current position
	- Set Home in steps
		- Asks how many steps
		- Asks what direction move
	- Set home in distance
		- Asks distance
		- Asks what direction to move
