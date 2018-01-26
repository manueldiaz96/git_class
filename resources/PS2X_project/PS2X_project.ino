#include <PS2X_lib.h>  //for v1.6

#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

void setup() {

  PS2X ps2x; // create PS2 Controller Class

  //right now, the library does NOT support hot pluggable controllers, meaning
  //you must always either restart your Arduino after you connect the controller,
  //or call config_gamepad(pins) again after connecting the controller.

  int error = 0;
  byte type = 0;
  byte vibrate = 0;

}

void loop() {
  // put your main code here, to run repeatedly:

}
