const byte pinSeg[] = {11, A1, 6, 8, 9, 10, 5, 7}; //A-G,dp
const byte pinDig[] = {12, 13, A0, 4};
const bool digOn = HIGH; // What state to have pins in to turn a digit on
const bool segOn = HIGH; // What state to have pins in to turn on a segment

int displayedChar[4]; // Display buffer
volatile byte currentDigit = 0; // Stores current digit for use in timer interrupt

/* Segment labels
     -A-
    F   B
     -G-
    E   C
     -D- (dp)

   Characters are described in bytes in segment form A to G + DP, lsb to msb
*/
const byte charCount = 20;
byte characterList[charCount] = {B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B00000111, B01111111, B01101111};

// Masks are used to reset all segment and digit pins on each port but not other pins
byte portBClearMask, portCClearMask, portDClearMask;
// These store how to set pins for all characters on a port
byte portBSeg[charCount]; byte portCSeg[charCount]; byte portDSeg[charCount];
// Stores how to set pins in for each digit
byte portBDig[4]; byte portCDig[4]; byte portDDig[4];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(pinDig[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(pinSeg[i], OUTPUT);
  }

  // Set timer2 interrupt
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  // set compare match register for 500hz increments
  OCR2A = 90; // = (16*10^6) / (1024 * freq) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);   // turn on CTC mode (Clears on timer compare)

  TCCR2B |= (B111 << CS20);   // Set CS20 and CS22 bit for 128 prescaler
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt


  // Adds decimals to numbers array by appending it with the digit with decimal
  // E.g. characterList[1] is the chartacter for 1, characterList[11] is 1 with a deciaml point
  for (byte i = 0; i < 10; i ++) characterList[10 + i] = characterList[i] | B10000000;
  
  // Preparing the lookup tables
  // Messing around with numbers
  setupPortLUT(); // Set up look up table

  //  // LUT Printing for debugging
  //  Serial.println(portBClearMask, BIN);
  //  Serial.println(portCClearMask, BIN);
  //  Serial.println(portDClearMask, BIN);
  //
  //  for (byte i = 0; i < 10; i++) {
  //    Serial.println(i);
  //    Serial.println(portBSeg[i], BIN);
  //    Serial.println(portCSeg[i], BIN);
  //    Serial.println(portDSeg[i], BIN);
  //  }

  Serial.println("GO!");
}

void loop() {
  // Shift characters into buffer
  if (Serial.available()) {
    delay(10);
    int temp = Serial.parseInt();
    temp %= 10000;
    Serial.println(temp);

    // Break down number by digit, starting at thousands digit.
    int base = 1000;
    for (byte i = 0; i < 4; i++) {
      displayedChar[i] = temp / base;
      temp = temp % base;
      base /= 10;
      //Serial.println(displayedChar[i]);
    }
  }
  delay(100);
}

ISR(TIMER2_COMPA_vect) { // Timer2 interrupt, displays a character on the display
  // Display
  // Turn off all digits and segments
  PORTB &= portBClearMask;
  PORTC &= portCClearMask;
  PORTD &= portDClearMask;

  // Turn on required digit
  PORTB |= portBDig[currentDigit];
  PORTC |= portCDig[currentDigit];
  PORTD |= portDDig[currentDigit];

  // Set segments
  PORTB |= portBSeg[displayedChar[currentDigit]];
  PORTC |= portCSeg[displayedChar[currentDigit]];
  PORTD |= portDSeg[displayedChar[currentDigit]];

  if (currentDigit == 3) currentDigit = 0; //Reset
  else currentDigit++;
}

/* Look Up Table Setting
    to make things run fast I will sacrifice memeory and store the pin settings
    for all three ports require for each character present.

    This will require transforming our straightforward character codes into
    the three seperate bytes for each port with the right bits in the right places
    as well as preparing the masks needed to reset the pins needed.

    This will require plenty of bit shifting, masking, and loops. However all the
    results will be saved so it only needs to be calculated once, saving time down
    the line at the expense of

*/

void setupPortLUT() {
  // Set up the look up tables for the three ports to quickly set the ports

  // Clear reset masks
  // Set to 0xFF since AND is used to clear pins by ANDing pins to reset with a 0
  portBClearMask = 255;
  portCClearMask = 255;
  portDClearMask = 255;

  // Clear segment character values
  for (byte i = 0; i < charCount; i++) {
    portBSeg[i] = 0;
    portCSeg[i] = 0;
    portDSeg[i] = 0;
  }
  // Clear digit values
  for (byte i = 0; i < 4; i++) {
    portBDig[i] = 0;
    portCDig[i] = 0;
    portDDig[i] = 0;
  }

  // Time to set digit masks and values

  for (byte i = 0; i < 4; i++) {
    if (pinDig[i] < 8) {
      // Port D
      portDDig[i] |= digOn << pinDig[i];      // Add pin to pin set
      portDClearMask ^= digOn << pinDig[i];  // Toggle the reset mask where the pin is
    }
    else if (pinDig[i] > 13) {
      // Port C
      portCDig[i] |= digOn << (pinDig[i] - 14);
      portCClearMask ^= digOn << (pinDig[i] - 14);
    }
    else {
      // Port B
      portBDig[i] |= digOn << (pinDig[i] - 8);
      portBClearMask ^= digOn << (pinDig[i] - 8);
    }
  }

  // Time to do the same for the segments
  // Will use nested loops to go through all characters for a segment before going to the next segment
  for (byte i = 0; i < 8; i++) {
    byte pinNumber = 0; // Used to store pin number in port
    
    // Find port
    if (pinSeg[i] < 8) {
      // Port D
      pinNumber = pinSeg[i];
      portSegLUTSetting(pinNumber, i, &portDClearMask, portDSeg);
    }
    else if (pinSeg[i] > 13) {
      // Port C
      pinNumber = pinSeg[i] - 14;
      portSegLUTSetting(pinNumber, i, &portCClearMask, portCSeg);
    }
    else {
      // Port B
      pinNumber = pinSeg[i] - 8;
      portSegLUTSetting(pinNumber, i, &portBClearMask, portBSeg);
    }
  }
}

void portSegLUTSetting(byte pinNumber, byte segment, byte* portMask, byte portCharLUT[]) {
  // pinNumber is pin number for the segment on the arduino
  // Segment is which segment the pin corrisponds to (0 for A, 1 for B, etc.)
  // portMask is a reference to the mask used to reset this pin's port
  // portCharLUT[] is the array used to store the values to set pins for each character

  *portMask ^= segOn << pinNumber; // Set the port's reset mask to include the pin

  byte tempMask = 1 << segment; // A temporary mask to be used to extract the segment digit from base character codes

  for (byte j = 0; j < charCount; j++) {
    // Check if segment is on for that character, then set on or off accordingly
    if ((characterList[j] & tempMask) > 0) portCharLUT[j] |= segOn << pinNumber;
    else portCharLUT[j] |= !segOn << pinNumber;
  }

}
