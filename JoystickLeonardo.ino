// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

// Number of buttons to handle
const int buttonsCount = 6;

// Arduino PINs to use
const int pins[buttonsCount] = {
    4,
    7,
    8,
    9,
    10, //joystick button : SEL 
    12 
};

// Connections to joystick (analog) :
const int VERT = A0;
const int HORIZ = A1;

// Debounce delay
const long debounceDelay = 50;

bool status[buttonsCount] = {HIGH};
long lastDebounces[buttonsCount] = {0};

void setup() {
      
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
     
      for (int i = 0; i < buttonsCount; ++i) {
	  handleButton(i, digitalRead(pins[i]), millis());
      }
      
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
			Joystick.pressButton(buttonNumber);
		} else {
			Joystick.releaseButton(buttonNumber);
		}
		lastDebounces[buttonNumber] = now;
	}
}
