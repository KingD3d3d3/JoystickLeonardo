// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

// Number of buttons to handle
const int buttonsCount = 6;

// Arduino PINs to use
const int pins[buttonsCount] = {
    4, // L
    7, // R
    8,
    9,
    10, //joystick button : SEL 
    12, // Start
};

// Connections to joystick (analog) :
const int VERT = A0;
const int HORIZ = A1;

// Debounce delay
const long debounceDelay = 50;

bool status[buttonsCount] = {HIGH};
long lastDebounces[buttonsCount] = {0};

// Number of autofire to handle
const int buttonsCountAuto = 2;

// Autofire buttons
const int autoButtons[buttonsCountAuto] = {
    2,
    3,
};

//Potentiometers
const int potarPins[buttonsCountAuto]{
  A2,
  A3,
};
int potarValue[buttonsCountAuto] = {}; //store the value coming from the potentiometer

// Store last time
unsigned long previousMillis[buttonsCountAuto] = {0};

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
      
}


void loop() {
  
     // Arranging analog joystick raw values from [0;1023] to [-127;127]
     int x = map(analogRead(HORIZ), 0, 1023, -127, 127);
     int y = map(analogRead(VERT), 0, 1023, -127, 127);
     Joystick.setXAxis(x); 
     Joystick.setYAxis(y);
     
     for (int i = 0; i < buttonsCount; ++i) { //i set button number
	       handleButton(i, digitalRead(pins[i]), millis());
     }

     for (int i = 0; i < buttonsCountAuto; ++i) {
        /*
        potarValue[i] = analogRead(potarPins[i]);
        Serial.print("potarValue : ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.println(potarValue[i]);
        */
        potarValue[i] = analogRead(potarPins[i]);
        if(potarValue[i] <= 1000){
          unsigned long period = definePeriod(potarValue[i]);
          handleAutoButton(autoButtons[i], i, millis(), period);
        }
     }
     
     /* Check function 
     potarValue = analogRead(potarPin);
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
