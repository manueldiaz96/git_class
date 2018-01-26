#include <PS2X_lib.h>  //for v1.6

#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

//Variables to work with H-bridge
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;
int valueI = 250;
int valueD = 250;

void setup() {

  Serial.begin(57600);

  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //  Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
   if(error == 1) //skip loop if no controller found
    return; 
  
  else { //DualShock Controller
    ps2x.read_gamepad(true, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");     

    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      adelante();
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
      giroDer();
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      giroIzq();
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      parar();
    }   

    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if(ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if(ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");        
    }

    if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
    if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
    if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
      Serial.println("Square just released");     

    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); 
    }     
  }
  delay(50);  
}

void adelante() {
  valueI = 170;
  valueD = 190;
  digitalWrite(M1, LOW);
  analogWrite(E1, valueI);
  digitalWrite(M2, LOW);
  analogWrite(E2, valueD);
  delay(850);
  digitalWrite(M1, LOW);
  analogWrite(E1, 0);
  delay(50);
  digitalWrite(M2, LOW);
  analogWrite(E2, 0);
  delay(500);
}

void giroIzq() {
  valueI = 250;
  valueD = 250;
  digitalWrite(M1, HIGH);
  analogWrite(E1, valueI);
  digitalWrite(M2, LOW);
  analogWrite(E2, valueD);
  delay(200);
  digitalWrite(M1, LOW);
  analogWrite(E1, 0);
  delay(50);
  digitalWrite(M2, LOW);
  analogWrite(E2, 0);
  delay(500);
  adelanteGiro();
}

void giroDer() {
  valueI = 250;
  valueD = 250;
  digitalWrite(M1, LOW);
  analogWrite(E1, valueI);
  digitalWrite(M2, HIGH);
  analogWrite(E2, valueD);
  delay(250);
  digitalWrite(M1, LOW);
  analogWrite(E1, 0);
  delay(50);
  digitalWrite(M2, LOW);
  analogWrite(E2, 0);
  delay(500);
  adelanteGiro();
}

void parar() {
  valueI = 0;
  valueD = 0;
  digitalWrite(M1, LOW);
  analogWrite(E1, valueI);
  digitalWrite(M2, LOW);
  analogWrite(E2, valueD);
  delay(200);
  digitalWrite(M1, LOW);
  analogWrite(E1, 0);
  delay(50);
  digitalWrite(M2, LOW);
  analogWrite(E2, 0);
  delay(500);
}
