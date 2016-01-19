// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

// Number of buttons to handle
const int buttonsCount = 6;

// Arduino PINs to use
const int pins[buttonsCount] = {
    7, 
    8,
    9,
    10,
    11,
    12,
};

// Debounce delay
const long debounceDelay = 50;

bool status[buttonsCount] = {HIGH};
long lastDebounces[buttonsCount] = {0};

// Number of autofire to handle
const int buttonsCountAuto = 2;

// Simulate press delay
const long pressDelay = 100;

// Autofire buttons
const int autoButtons[buttonsCountAuto] = {
    4,
    5,
};

//Potentiometers
const int potarPins[buttonsCountAuto]{
  A0,
  A1,
};
int potarValue[buttonsCountAuto] = {}; //store the value coming from the potentiometer
// Store last time
unsigned long previousMillis[buttonsCountAuto] = {0};

int incomingInt = 0; // store incoming serial data from Mouse/JoystickUSB/TrackBall on Raspberry Pi
int x1 = 0, y1 = 0;

void setup() {
      Serial.begin(9600);
      Serial1.begin(9600);
      
      if (testAutoSendMode){
        Joystick.begin();
      }
      else {
        Joystick.begin(false);
      }
  
      // set up input buttons
      for (int i = 0; i < buttonsCount; ++i) {
         pinMode(pins[i], INPUT_PULLUP);
      }

      // set up arcade joystick
      for (int j = 2; j < 6; ++j) {
         pinMode(j, INPUT_PULLUP);
      }
    
}


void loop() {

     int x0 = defineJoystickX(4,5);
     int y0 = defineJoystickY(2,3);

     //int buf1 = x1;
     //int buf2 = y1;
     // send data only when you receive data:
     if (Serial1.available() == 2) {
          // read incoming 2 bytes
          incomingInt = Serial1.read()*256 + Serial1.read();
          x1 = highByte(incomingInt);
          y1 = lowByte(incomingInt);
          x1 = x1 - 127;
          y1 = 127 - y1;
          if(abs(x1)<127 && abs(y1) < 127){
            x1 = 0;
            y1 = 0;
          }
          /*Serial.print("x1 = ");
          Serial.print(x1, DEC);
          Serial.print(", y1 = ");
          Serial.println(y1, DEC);*/
      } else if (Serial1.available() > 2){
          Serial.println("Problem too much data ...");
          flushSerial1();
          Serial.println("No More Problem !");
      } /*else if(Serial1.available() ==0){
        x1 = 0;
        y1 = 0;
      }*/
      /*if ((buf1==x1) && (buf2==y1)){
        x1=0;
        y1=0;
      }*/

     if((x0 != 0) || (y0 != 0)){
          Joystick.setXAxis(x0);
          Joystick.setYAxis(y0);
     } else if((x0 == 0) && (y0 == 0)){
          Joystick.setXAxis(x1);
          Joystick.setYAxis(y1);
     }
    
     //Buttons loop
     for (int i = 0; i < buttonsCount; ++i) { //i set button number
	       handleButton(i, digitalRead(pins[i]), millis());
     }

    // Autofire buttons loop
     for (int i = 0; i < buttonsCountAuto; ++i) {
        potarValue[i] = analogRead(potarPins[i]);
        if(potarValue[i] <= 1000){
          unsigned long period = definePeriod(potarValue[i]);
          handleAutoButton(autoButtons[i], i, millis(), period);
        }
     }
     
     /* Check function 
     int potarValue = analogRead(A2);
     //unsigned long period = definePeriod(potarValue);
     Serial.print("potarValue : ");
     Serial.println(potarValue);
     */

     if (!testAutoSendMode)
     {
      Joystick.sendState();
     }
}

/**
 * \brief Send a joystick button press/release if needed
 *
 * \param buttonNumber Button ID to handle
 * \param pinStatus PIN status (LOW/HIGH)
 * \param now Time in millis
 */
void handleButton(const int buttonNumber, const int pinStatus, const long now) {
	if (pinStatus != status[buttonNumber] && now - debounceDelay > lastDebounces[buttonNumber]) {
		status[buttonNumber] = pinStatus;
		if (pinStatus == LOW) {
      /*
      Serial.print("button : ");
      Serial.print(buttonNumber);
      Serial.println(" is pressed");
      */
			Joystick.pressButton(buttonNumber);
		} else {
			Joystick.releaseButton(buttonNumber);
		}
		lastDebounces[buttonNumber] = now;
	}
}

/**
 * \brief Autofire a button with a specific period
 *
 * \param buttonNumber Button ID to handle
 * \param previous Indicator of the previous time in the array
 * \param now Current time in millis
 * \param period Autofire period in millis
 */
void handleAutoButton(const int buttonNumber, const int previous, const long now, const long period) {
   if(now - previousMillis[previous] >= period) {
     previousMillis[previous] = now;
     Joystick.pressButton(buttonNumber);
   } else if(now - previousMillis[previous] >= debounceDelay){ //simulate press delay of a button
     Joystick.releaseButton(buttonNumber);
   }
}

/**
 * \brief Define period in millisecond from an analog value in input
 *
 * \param val Analog value
 */ 
unsigned long definePeriod(int val){
  return 2 * val;
}

/**
 * \brief Return Joystick X axis value (-127,0,127)
 */ 
int defineJoystickX(int pinRight, int pinLeft){
      if(!digitalRead(pinRight)){ // Right pin 4
        //Serial.println("right x = 127");
        return 127;
      }
      else if (!digitalRead(pinLeft)){ // Left pin 5
        //Serial.println("left x = -127");
        return -127;
      } else return 0;
}
  
/**
 * \brief Return Joystick Y axis value (-127,0,127)
 */ 
int defineJoystickY(int pinUp, int pinDown){
  if(!digitalRead(pinUp)){ // Up pin 2
    //Serial.println("up y = -127");
    return -127;
  }
  else if (!digitalRead(pinDown)){ // Down pin 3
    //Serial.println("down y= 127");
    return 127;
  } else return 0;
}

/**
 * \brief Flush the serial 1
 */ 
void flushSerial1(){
  while(Serial1.available()) {
      Serial1.read();
  }
}

