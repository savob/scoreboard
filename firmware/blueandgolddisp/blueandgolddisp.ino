const byte pinSeg[] = {11, A1, 6, 8, 9, 10, 5, 7}; //A-G,dp
const byte pinDig[] = {12, 13, A0, 4};
const bool digOn = HIGH; // What state to have pins in to turn a digit on
const bool segOn = HIGH;   // What state to have pins in to turn on a segment

int buff[4]; // Display buffer
volatile byte currentDigit = 0; // Stores current digit for use in timer interrupt

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(pinDig[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(pinSeg[i], OUTPUT);
  }

  //set timer2 interrupt
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  
  // set compare match register for 500hz increments
  OCR2A = 249; // = (16*10^6) / (128 * 500) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);   // turn on CTC mode (Clears on timer compare)
  
  TCCR2B |= (1 << CS20 | 1 << CS22);   // Set CS20 and CS22 bit for 128 prescaler
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt


  // Preparing the lookup tables 
  // Messing around with numbers
  addDecimals(); // Create the number constrants with decimals 
  
  Serial.println("GO!");
}

void loop() {
  // Shift characters into buffer
  if (Serial.available()) {
    delay(1);
    int temp = Serial.parseInt();
    Serial.println(temp);

    // Break down number by digit, starting at thousands digit.
    int base = 1000;
    for (byte i = 0; i < 4; i++) {
      buff[i] = temp / base;
      temp = temp % base;
      base /= 10;
      //Serial.println(buff[i]);
    }
  }
  delay(100);
}


ISR(TIMER2_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
  dispDigit(currentDigit,convertNum(buff[currentDigit]));
  
  if (currentDigit == 3) currentDigit = 0; //Reset
  else currentDigit++;
}
