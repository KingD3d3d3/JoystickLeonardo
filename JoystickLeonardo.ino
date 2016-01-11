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

// Number of buttons to handle
const int joyCount = 4;
// Debounce delay Joystick
const long debounceDelayJoy = 125;
long lastDebouncesJoy[joyCount] = {0};

void setup() {
      Serial.begin(9600);
      
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
     
     int x = defineJoystickX(4,5);
     int y = defineJoystickY(2,3);
     Joystick.setXAxis(x);
     Joystick.setYAxis(y);

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
 * \param previous Previous time in millis
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

