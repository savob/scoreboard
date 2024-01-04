/*
   Note: there is no error checking/correction for inputs

   This code works with the seven segment display. It assumes
   the pins controlling it are sequential all offset by startPin
   The first seven are the segments as shown below:
             -A-
            F   B
             -G-
            E   C
             -D- (dp)
   The eigth pin is that decimal point, then the four for each
   digit (sequencially), and then the semicolon (digit 5)
*/

byte numbers[20] = {B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B00000111, B01111111, B01101111}; // Numbers in segment form A to G, lsb to msb

void addDecimals() {
  // Adds decimals to numbers array by appending it with the digit with decimal
  // E.g. numbers[1] is the chartacter for 1, numbers[11] is 1 with a deciaml point
  
  for (byte i = 0; i < 10; i ++) numbers[10 + i] = numbers[i] | B10000000;
}

void dispDigit(const byte digit, const byte segments) {

  
  // Set segments
  for (int i = 0; i < 8; i++) {
    digitalWrite(pinSeg[i], bitRead(segments, i) ? segOn : !segOn);
  }

    // Turn off all digits
  for (int i = 0; i < 4; i++) {
    digitalWrite(pinDig[i], !digOn);
  }
  
  // Turn on needed digit
  digitalWrite(pinDig[digit], digOn);
}

byte convertNum(byte number) {
  // Number segment powering
  byte numbers[16] = {B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B00000111, B01111111, B01101111}; // Numbers in segment form A to G, lsb to msb
  return numbers[number];
}
byte convertChar(char target) {
  byte x = 0;
  // Check for number
  if ((target < 58) && (target > 47)) return convertNum(target); // Return number is numeric
  
  target = bitClear(target, 5); // Upper case
  switch (target) {
    case ('A'):
      x = B1110111;
      break;
    case ('B'):
      x = B1111100;
      break;
    case ('C'):
      x = B1011000;
      break;
    case ('D'):
      x = B1011110;
      break;
    case ('E'):
      x = B1111001;
      break;
    case ('F'):
      x = B1110001;
      break;
    case ('H'):
      x = B1110101;
      break;
    case ('I'):
      x = B0000100;
      break;
    case ('J'):
      x = B0011110;
      break;
    case ('K'):
      x = B1110110;
      break;
    case ('L'):
      x = B0111000;
      break;
    case ('N'):
      x = B1010100;
      break;
    case ('O'):
      x = B1011100;
      break;
    case ('P'):
      x = B1110011;
      break;
    case ('Q'):
      x = B1100111;
      break;
    case ('R'):
      x = B1010000;
      break;
    case ('T'):
      x = B1000110;
      break;
    case ('U'):
      x = B0011100;
      break;
    case ('V'):
      x = B0011100;
      break;
    case ('Y'):
      x = B1101110;
      break;
    case ('X'):
      x = B1110110;
      break;
    default:
      x = 0; // Blank
      break;
  }
  return x;
}

void displayChar(byte digit, char input) {
  dispDigit(digit, convertChar(input));
}
